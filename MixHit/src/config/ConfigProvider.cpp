#include "ConfigProvider.h"
#include "cJSON.h"
#include "esp_log.h"
#include <stdint.h>
#include <stdio.h>
#include "WString.h"
#include "../driver/FileOperations.h"

#ifdef ESP_LOG
#define LOG
#else
#define LOG(TAG, ARGS) Serial.println(TAG ARGS);
#endif

/* STATIC */ void ConfigProvider::init()
{
	configMap.clear();
	Serial.println("ConfigProvider: Reading.");
	AdvFile file = AdvFile(config_select_path);
	std::string content = file.getContent();
	//ESP_LOGI("ConfigProvider", "%s", content.c_str());
	cJSON * root = cJSON_Parse(content.c_str());
	if (!cJSON_HasObjectItem(root, "configs")) {
		Serial.println("ConfigProvider: default config has invalid json format.");
		cJSON_Delete(root);
		return;
	}
	cJSON * arr = cJSON_GetObjectItem(root, "configs");
	cJSON * element;
	cJSON_ArrayForEach(element, arr) {
		if (cJSON_HasObjectItem(element, "Name") && cJSON_HasObjectItem(element, "Pfad")) {
			std::string name = cJSON_GetObjectItem(element, "Name")->valuestring;
			std::string path = cJSON_GetObjectItem(element, "Pfad")->valuestring;
			Serial.println("ConfigProvider: Config found");// %s with path %s found.", name.c_str(), path.c_str());
			for (uint8_t ii = 1; ii < eConfig_NumOfConfigs; ++ii) {
				if (name == names[ii]) {
					if (configMap.find((eConfig)ii) != configMap.end()) {
						Serial.println("ConfigProvider: Already saved Config");// %s. Doubled path %s", name.c_str(), path.c_str());
					} else {
						Serial.println("ConfigProvider: Saving config");// %s with path %s", name.c_str(), path.c_str());
						configMap.insert(std::pair<eConfig, std::string>((eConfig)ii, path));
					}

				}
			}
		}
		else {
			ESP_LOGW("ConfigProvider", "Invalid section found.");
		}
	}
	ESP_LOGI("ConfigProvider", "Finished.");
	cJSON_Delete(root);
}

/* STATIC */ std::string ConfigProvider::getPath(eConfig config)
{
	std::map<eConfig, std::string>::iterator it = configMap.find(config);
	if (it != configMap.end()) {
		return std::string("/spiffs/config/") + it->second;
	}
	return "";
}
