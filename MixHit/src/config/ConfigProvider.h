#ifndef CONFIG_PROVIDER_H_
#define CONFIG_PROVIDER_H_

#include <string>
#include <map>
#include "../include/File.h"

enum eConfig {
	eConfig_Unknown = 0,
	eConfig_WiFi,
	eConfig_Glasses,
	eConfig_Reservoir,
	eConfig_Cocktails,
	eConfig_NumOfConfigs
};

static const std::string names[] = {
	"INVALID",		// eConfig_Unknown
	"wifi",			// eConfig_WiFi
	"glasses",		// eConfig_Glasses
	"reservoir"		// eConfig_Reservoir
	"cocktails"		// eConfig_Cocktails
};

static const char* config_select_path = "/spiffs/config/configs.json";
static std::map<eConfig, std::string> configMap;

class ConfigProvider
{
public:
	static void init();
	static std::string getPath(eConfig config);
private:
	ConfigProvider();	// NO INSTANCES TO MAKE
};


#endif /* CONFIG_PROVIDER_H_ */
