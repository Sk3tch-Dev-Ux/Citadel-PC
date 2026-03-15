class UtopiaPcSettings
{
    string version = UTOPIA_PC_VERSION;
    ref array<ref UtopiaGraphicCardLifetime> graphicCardLifetimes;
    ref map<string, ref UtopiaCoinSettings> coinSettings;

    void UtopiaPcSettings()
    {
        graphicCardLifetimes = new array<ref UtopiaGraphicCardLifetime>();
        coinSettings = new map<string, ref UtopiaCoinSettings>();
    }

    void LoadDefaultSettings()
    {
        // Load default graphic card lifetimes
        graphicCardLifetimes.Insert(new UtopiaGraphicCardLifetime("Utopia_Graphics_Card_660", 7200));
        graphicCardLifetimes.Insert(new UtopiaGraphicCardLifetime("Utopia_Graphics_Card_1080TI", 14400));
        graphicCardLifetimes.Insert(new UtopiaGraphicCardLifetime("Utopia_RTX_2080TI", 21600));
        graphicCardLifetimes.Insert(new UtopiaGraphicCardLifetime("Utopia_Graphics_Card_4090TI", 28800));

        // Load default coins (can be overridden via the config file)
        InsertDefaultCoinSettings();

        Save();
    }

    void InsertDefaultCoinSettings()
    {
        // Hash rates (MH/s) per GPU — higher = mines faster (currently informational, displayed in UI)
        map<string, float> bitcoinHashRates = new map<string, float>();
        bitcoinHashRates.Insert("Utopia_Graphics_Card_660",    2.0);
        bitcoinHashRates.Insert("Utopia_Graphics_Card_1080TI", 10.0);
        bitcoinHashRates.Insert("Utopia_RTX_2080TI",           18.0);
        bitcoinHashRates.Insert("Utopia_Graphics_Card_4090TI", 30.0);

        // Coins yielded per mining tick (MINING_TICK_INTERVAL seconds, default 60s)
        map<string, float> bitcoinYields = new map<string, float>();
        bitcoinYields.Insert("Utopia_Graphics_Card_660",    0.05);
        bitcoinYields.Insert("Utopia_Graphics_Card_1080TI", 0.10);
        bitcoinYields.Insert("Utopia_RTX_2080TI",           0.15);
        bitcoinYields.Insert("Utopia_Graphics_Card_4090TI", 0.25);

        // damageRates are unused for wear calculation — GPU wear is derived from the
        // lifetime values in graphicCardLifetimes so each GPU lasts exactly that many
        // seconds of continuous mining regardless of which coin is being mined.
        map<string, float> bitcoinDamageRates = new map<string, float>();
        bitcoinDamageRates.Insert("Utopia_Graphics_Card_660",    1.0);
        bitcoinDamageRates.Insert("Utopia_Graphics_Card_1080TI", 1.0);
        bitcoinDamageRates.Insert("Utopia_RTX_2080TI",           1.0);
        bitcoinDamageRates.Insert("Utopia_Graphics_Card_4090TI", 1.0);

        coinSettings.Insert("Bitcoin", new UtopiaCoinSettings("Bitcoin", bitcoinHashRates, bitcoinYields, bitcoinDamageRates, "Bitcoin_Item"));
    }



    UtopiaCoinSettings GetCoinSettings(string coinName)
    {
        return coinSettings.Get(coinName);
    }

    void Save()
    {
        JsonFileLoader<UtopiaPcSettings>.JsonSaveFile(UTOPIA_PC_CONFIG_FILE, this);
    }

    static UtopiaPcSettings Load()
    {
        UtopiaPcSettings settings = new UtopiaPcSettings();
        EnsureDirectoriesExist();

        if (FileExist(UTOPIA_PC_CONFIG_FILE))
        {
            JsonFileLoader<UtopiaPcSettings>.JsonLoadFile(UTOPIA_PC_CONFIG_FILE, settings);
            return settings;
        }

        settings.LoadDefaultSettings();
        return settings;
    }

    static void EnsureDirectoriesExist()
    {
        if (!FileExist(UTOPIA_PC_ROOT_FOLDER))
        {
            MakeDirectory(UTOPIA_PC_ROOT_FOLDER);
        }
        if (!FileExist(UTOPIA_PC_CONFIG_FOLDER))
        {
            MakeDirectory(UTOPIA_PC_CONFIG_FOLDER);
        }
        if (!FileExist(UTOPIA_PC_LOG_FOLDER))
        {
            MakeDirectory(UTOPIA_PC_LOG_FOLDER);
        }
    }

    int FindLifetimeForClassName(string className)
    {
        foreach(UtopiaGraphicCardLifetime graphicCardLifetime : graphicCardLifetimes)
        {
            if (CF_String.EqualsIgnoreCase(graphicCardLifetime.className, className))
                return graphicCardLifetime.lifetime;
        }
        return 0;
    }
}


class UtopiaCoinSettings
{
    string coinName;                     // Name of the cryptocurrency
    ref map<string, float> hashRates;    // Hash rates per GPU type for mining this coin
    ref map<string, float> yields;       // Coin yield per GPU type
    ref map<string, float> damageRates;  // Damage amount per GPU type
    string itemClass;                    // The class name of the item representing this coin in the game

    void UtopiaCoinSettings(string name, map<string, float> rates, map<string, float> yieldsMap, map<string, float> damageMap, string item)
    {
        coinName = name;
        hashRates = rates;
        yields = yieldsMap;
        damageRates = damageMap;
        itemClass = item;
    }

    float GetHashRate(string cardType)
    {
        return hashRates.Get(cardType);
    }

    float GetYield(string cardType)
    {
        return yields.Get(cardType);
    }

    float GetDamageRate(string cardType)
    {
        return damageRates.Get(cardType);
    }

    bool CanCardMine(string cardType)
    {
        return hashRates.Contains(cardType);
    }
}

