/*
 * WiFiConfig.h
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#ifndef MAIN_WIFICONFIG_H_
#define MAIN_WIFICONFIG_H_

#include <string>
#include "../include/WiFi.h"

class WiFiConfig {
public:
	WiFiConfig();
	WiFiConfig(std::string connectSSID, std::string connectPW, std::string apSSID, std::string apPW);

	void handleWiFi(AdvWiFi * wifi);
	static WiFiConfig* fromFile(std::string file);
	static WiFiConfig* fromJson(std::string json);
	std::string toJson(void);
	void toFile(std::string file);
private:
	std::string mConnectSSID;
	std::string mConnectPW;

	std::string mApSSID;
	std::string mApPW;
};



#endif /* MAIN_WIFICONFIG_H_ */
