
static ref CitadelPcLoggingModule GetCitadelPcLogger()
{
    return CitadelPcLoggingModule.Cast(CF_ModuleCoreManager.Get(CitadelPcLoggingModule));
}

[CF_RegisterModule(CitadelPcLoggingModule)]
class CitadelPcLoggingModule : CF_ModuleGame
{
    private int networkSync_LogLevel;

    ref CitadelPcLoggingSettings settings;

    FileHandle fileHandle;

    float dtime = 0;


    override void OnInit()
    {
        super.OnInit();

        EnableUpdate();
        EnableMissionStart();
        EnableMissionFinish();
    }

    override void OnMissionStart(Class sender, CF_EventArgs args)
    {
        super.OnMissionStart(sender, args);

        fileHandle = CreateNewLogFile();

        if(GetGame().IsServer())
        {
            settings = CitadelPcLoggingSettings.Load();

            networkSync_LogLevel = settings.logLevel;
            SynchLogLevel();       
        }
        else
        {
            GetRPCManager().AddRPC(ClassName(), "GetLogLevelResponse", this, SingeplayerExecutionType.Client);
        }
    }

    void GetLogLevelResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		 if (type != CallType.Client)
			 return;

		 Param1<int> data;
		 if (!ctx.Read(data))
            return;

		 networkSync_LogLevel = data.param1;
	}

    void SynchLogLevel()
    {
        GetRPCManager().SendRPC(ClassName(), "GetLogLevelResponse",  new Param1<int>(networkSync_LogLevel), true, NULL);
    }

    override void OnMissionFinish(Class sender, CF_EventArgs args)
    {
        super.OnMissionFinish(sender, args);

        if (fileHandle != 0)
        {
            CloseFile(fileHandle);
            fileHandle = 0;
        }
    }

    override void OnUpdate(Class sender, CF_EventArgs args)
    {
        auto update = CF_EventUpdateArgs.Cast(args);

        if(!GetGame().IsServer())
            return;

        if(!settings)
            return;

        dtime += update.DeltaTime;
        if(dtime >= settings.refreshRateInSeconds)
        {
            dtime = 0;

            settings = CitadelPcLoggingSettings.Load();

            networkSync_LogLevel = settings.logLevel;
            SynchLogLevel();
        }
    }


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
    
    string GenerateShortDateString()
    {
        int year, month, day;
        GetYearMonthDay(year, month, day);
        return "" + year + "_" + month + "_" + day;
    }

    string GenerateShortTimeString()
    {
        int hour, minute, second;
        GetHourMinuteSecond(hour, minute, second);
        return "" + hour + "_" + minute + "_" + second;
    }

    string GenerateFullTimestamp()
    {
        string dateStr = GenerateShortDateString();
        string timeStr = GenerateShortTimeString();

        return dateStr + "-" + timeStr;
    }

    FileHandle CreateNewLogFile()
    {
        MakeDirectoryIfNotExists();

        string filePath = string.Format(CITADEL_PC_LOGGER_LOG_FILE, GenerateFullTimestamp());

        fileHandle = OpenFile(filePath, FileMode.WRITE);

        if(fileHandle != 0)
        {
            FPrintln(fileHandle, "Creation Time: " + GenerateFullTimestamp());
            return fileHandle;
        }

        return null;
    }

    void Log(string content, CitadelPcLogLevel logLevel)
    {
        if(logLevel < networkSync_LogLevel)
            return;
        
        string timestamp = GenerateShortTimeString();
        FPrintln(fileHandle, timestamp + " | " + GetLogLevelString(logLevel) + " | " + content);
    }

    void LogInfo(string content)
    {
        Log(content, CitadelPcLogLevel.Info);
    }

    void LogWarning(string content)
    {
        Log(content, CitadelPcLogLevel.Warn);
    }

    void LogError(string content)
    {
        Log(content, CitadelPcLogLevel.Error);
    }

    void LogDebug(string content)
    {
        Log(content, CitadelPcLogLevel.Debug);
    }

    string GetLogLevelString(CitadelPcLogLevel logLevel)
    {
        switch(logLevel)
        {
             case CitadelPcLogLevel.Debug:
                return "DEBUG";
            case CitadelPcLogLevel.Info:
                return "INFO";
            case CitadelPcLogLevel.Warn:
                return "WARNING";
            case CitadelPcLogLevel.Error:
                return "ERROR";
            default:
                return "";
        }

        return "";
    }

}