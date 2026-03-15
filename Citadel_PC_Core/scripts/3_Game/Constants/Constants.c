const string CITADEL_PC_ROOT_FOLDER = "$profile:Citadel_PC\\";
const string CITADEL_PC_CONFIG_FOLDER = CITADEL_PC_ROOT_FOLDER + "Config\\";
const string CITADEL_PC_CONFIG_FILE = CITADEL_PC_CONFIG_FOLDER + "CitadelPcSettings.json";


const string CITADEL_PC_LOG_FOLDER = CITADEL_PC_ROOT_FOLDER + "Logging\\";
const string CITADEL_PC_LOGGER_CONFIG_DIR = CITADEL_PC_LOG_FOLDER + "Config\\";
const string CITADEL_PC_LOGGER_CONFIG_FILE = CITADEL_PC_LOGGER_CONFIG_DIR + "LoggingSettings.json";
const string CITADEL_PC_LOGGER_LOG_DIR = CITADEL_PC_LOG_FOLDER + "Logs\\";
const string CITADEL_PC_LOGGER_LOG_FILE = CITADEL_PC_LOGGER_LOG_DIR + "CitadelPc_%1.log";

const string CITADEL_PC_VERSION = "1.0";

ref TStringArray g_CitadelPcGraphicCardSlots = {
    "Graphics_Card_2",
	"Graphics_Card_1",
	"Graphics_Card_3",
};