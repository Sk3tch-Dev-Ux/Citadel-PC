class CitadelPcLoggingSettings 
{
    int logLevel = 1;
    int refreshRateInSeconds = 60;

    void MakeDirectoryIfNotExists()
    {
        if(!FileExist(CITADEL_PC_ROOT_FOLDER))
            MakeDirectory(CITADEL_PC_ROOT_FOLDER);

        if(!FileExist(CITADEL_PC_LOG_FOLDER))
            MakeDirectory(CITADEL_PC_LOG_FOLDER);

        if(!FileExist(CITADEL_PC_LOGGER_CONFIG_DIR))
            MakeDirectory(CITADEL_PC_LOGGER_CONFIG_DIR);
        
        if(!FileExist(CITADEL_PC_LOGGER_LOG_DIR))
            MakeDirectory(CITADEL_PC_LOGGER_LOG_DIR);
    }

    void Save()
    {
        JsonFileLoader<CitadelPcLoggingSettings>.JsonSaveFile(CITADEL_PC_LOGGER_CONFIG_FILE, this);
    }

    static ref CitadelPcLoggingSettings Load()
    {
        CitadelPcLoggingSettings settings = new CitadelPcLoggingSettings();

        settings.MakeDirectoryIfNotExists();

        if(FileExist(CITADEL_PC_LOGGER_CONFIG_FILE))
        {
            JsonFileLoader<CitadelPcLoggingSettings>.JsonLoadFile(CITADEL_PC_LOGGER_CONFIG_FILE, settings);
            return settings;
        }

        settings.Save();
        return settings;
    }

}