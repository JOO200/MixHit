/*
 * ReservoirConfig.cpp
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#include "ReservoirConfig.h"

#include "esp_log.h"
#include "cJSON.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

#include "../include/File.h"

ReservoirConfig::ReservoirConfig()
{
	ESP_LOGI("ReservoirConfig", "Initialising new ReservoirConfig with default parameters.");
	// default:
}

ReservoirConfig::ReservoirConfig(cReservoir* reservoir)
{
	instance = reservoir;
}

void ReservoirConfig::setInstance(cReservoir* reservoir)
{
	instance = reservoir;
}

std::string ReservoirConfig::toJson()
{
	/*
	cJSON *root = cJSON_CreateObject();
	for(std::map<int, std::string>::iterator it = ingredientContainerMap.begin(); it != ingredientContainerMap.end(); ++it)
	{
		cJSON_AddNumberToObject(root, it->second.c_str(), it->first);
	}
	std::string json = cJSON_Print(root);
	cJSON_Delete(root);
	*/ // TODO: Mach mal fertig.
	return "";
}

void ReservoirConfig::toFile(std::string file)
{
	std::string json = toJson();
	FILE * f = fopen(file.c_str(), "wb");
	if(f == NULL) {
		ESP_LOGE("WiFi", "Error writing to file %s", file.c_str());
		return;
	}
	fprintf(f, json.c_str());
	fclose(f);
}

/* STATIC */ bool ReservoirConfig::fromFile(std::string file, cReservoir * reservoir)
{
	AdvFile * f = new AdvFile(file);
	std::string json = f->getContent();
	delete f;
	return ReservoirConfig::fromJson(json, reservoir);
}

#define JSON_NOT_NULL(arg, root) \
	if(arg == NULL) { \
		cJSON_Delete(root); \
		return false; \
	}

/* STATIC */ bool ReservoirConfig::fromJson(std::string json, cReservoir * reservoir)
{
	cJSON *root = cJSON_Parse(json.c_str());
	std::string apSSID, apPW, connectSSID, connectPW;
	cJSON *array = cJSON_GetObjectItem(root, "reservoirs");
	JSON_NOT_NULL(array, root);
	array = cJSON_GetObjectItem(root, "reservoir");
	JSON_NOT_NULL(array, root);
	cJSON *element;
	cJSON_ArrayForEach(element, array) {
		cJSON * id = cJSON_GetObjectItem(element, "id");
		cJSON * name = cJSON_GetObjectItem(element, "name");
		cJSON * b = cJSON_GetObjectItem(element, "b");
		cJSON * m = cJSON_GetObjectItem(element, "m");
		if(id == NULL || name == NULL || b == NULL || m == NULL) continue;
		SingleReservoir * res = new SingleReservoir(id->valueint, name->valuestring, b->valuedouble, m->valuedouble);
		reservoir->addReservoir(res);
	}
	return true;
}


