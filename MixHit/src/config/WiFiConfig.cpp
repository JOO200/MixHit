/*
 * WiFiConfig.cpp
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#include "WiFiConfig.h"
#include "esp_log.h"
#include "cJSON.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

#include "../include/File.h"
#include "../include/WiFi.h"

WiFiConfig::WiFiConfig()
{
	ESP_LOGI("WiFi", "Initialising new WiFiConfig with default parameters.");
	mConnectSSID = "MixHit-WiFi";
	mConnectPW = "";

	mApSSID = "MixHit-WiFi";
	mApPW = "";
}

WiFiConfig::WiFiConfig(std::string connectSSID, std::string connectPW, std::string apSSID, std::string apPW)
{
	mConnectSSID = connectSSID;
	mConnectPW = connectPW;
	mApSSID = apSSID;
	mApPW = apPW;
}

void WiFiConfig::handleWiFi(AdvWiFi * wifi)
{
	if(mConnectSSID != "") {
		ESP_LOGI("WiFi", "Connecting to %s, PW %s", mConnectSSID.c_str(), mConnectPW.c_str());
		if(wifi->connectAP(mConnectSSID, mConnectPW) == ESP_OK) {
			ESP_LOGI("WiFi", "Connected succesful to %s.", mConnectSSID.c_str());
			return;
		}
		ESP_LOGW("WiFi", "Connection to %s refused.", mConnectSSID.c_str());
	}
	ESP_LOGI("WiFi", "Starting AP");
	wifi->startAP(mApSSID, mApPW);
}

std::string WiFiConfig::toJson()
{
	cJSON *root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "mApSSID", mApSSID.c_str());
	cJSON_AddStringToObject(root, "mApPW", mApPW.c_str());
	cJSON_AddStringToObject(root, "mConnectSSID", mConnectSSID.c_str());
	cJSON_AddStringToObject(root, "mConnectPW", mConnectPW.c_str());
	std::string json = cJSON_Print(root);
	cJSON_Delete(root);
	return json;
}

void WiFiConfig::toFile(std::string file)
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

/* STATIC */ WiFiConfig* WiFiConfig::fromFile(std::string file)
{
	AdvFile * f = new AdvFile(file);
	std::string json = f->getContent();
	delete f;
	return WiFiConfig::fromJson(json);
}

/* STATIC */ WiFiConfig* WiFiConfig::fromJson(std::string json)
{
	cJSON *root = cJSON_Parse(json.c_str());
	std::string apSSID, apPW, connectSSID, connectPW;
	cJSON *workingTree = cJSON_GetObjectItem(root, "WiFi");
	if (workingTree == NULL) {
		cJSON_Delete(root);
		ESP_LOGE("WiFi", "Received Json File is not a WiFi Config.");
		return NULL;
	}
	cJSON *obj = cJSON_GetObjectItem(workingTree, "mApSSID");
	if(obj == NULL) {
		cJSON_Delete(root);
		ESP_LOGE("WiFi", "Received Json File don't habe mApSSID field.");
		return NULL;
	} else {
		apSSID = obj->valuestring;
	}

	obj = cJSON_GetObjectItem(workingTree, "mApPW");
	apPW = obj == NULL ? "" : obj->valuestring;

	obj = cJSON_GetObjectItem(workingTree, "mConnectSSID");
	connectSSID = obj == NULL ? "" : obj->valuestring;

	obj = cJSON_GetObjectItem(workingTree, "mConnectPW");
	connectPW = obj == NULL ? "" : obj->valuestring;

	cJSON_Delete(root);
	return new WiFiConfig(connectSSID, connectPW, apSSID, apPW);
}


