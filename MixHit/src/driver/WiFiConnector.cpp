#include "WiFiConnector.h"
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <iostream>

#include "SPIFFS.h"
#include "FS.h"
#include "../driver/FileOperations.h"
#include "WiFi.h"

using namespace std;

static void WriteDefaultWiFiConfig()
{
	String pFileName = "/Default_Config.wtxt";
	writeFile(SPIFFS, pFileName, "Modus;AP_Mode;AP_Mode (Eigenes WiFi aufspannen) oder Connect_Mode (mit vorhandenem WiFi verbinden)\n\r");
	appendFile(SPIFFS, pFileName, "SSID;MixHit_WiFi;Name des Netzwerkes\n\r");
	appendFile(SPIFFS, pFileName, "Password;;Leer = kein Passwort\n\r");
}

WiFiConnector::WiFiConnector(String pConfigFile)
{
	mServer = WiFiServer(80, 200);

	String lFileContent = "";
	/*readFile(SPIFFS, pConfigFile, &lFileContent);
	if (lFileContent == "")
	{
		WriteDefaultWiFiConfig();
		readFile(SPIFFS, "/Default_WiFi_Config.wtxt", &lFileContent);
	}
	lFileContent.replace("\r", "");
	vector<String> Lines;
	split(&lFileContent, '\n', &Lines);
	vector<String>Line0;
	vector<String>Line1;
	vector<String>Line2;
	split(&Lines[0], ';', &Line0);
	split(&Lines[1], ';', &Line1);
	split(&Lines[2], ';', &Line2); */
	mMode = eConnectMode_AP; // (eWiFiConnectMode)Line0[1].toInt();
	mUsedSSID = "MyTestWLAN"; // Line1[1];
	mUsedPassword = ""; // Line2[1];
}


void WiFiConnector::start(void)
{
	unsigned long lStartTime = millis();
	unsigned long lMaxTimeToConnect = 10000; // 10sec
	switch (mMode)
	{
	case eConnectMode_Client:
		if (mUsedPassword == "")
		{
			WiFi.begin(mUsedSSID.c_str());
		}
		else
		{
			WiFi.begin(mUsedSSID.c_str(), mUsedPassword.c_str());
		}
		while (WiFi.status() != WL_CONNECTED && millis() - lStartTime <= lMaxTimeToConnect)
		{
			vTaskDelay(500);
			Serial.println("Connecting to WiFi..");
		}
		if (WiFi.status() == WL_CONNECTED)
		{
			Serial.println("Connected to the WiFi network");
			mOwnIp = WiFi.localIP().toString();
			Serial.println("local IP: " + mOwnIp);


			WiFi.begin();
			break;
		}
		else
		{
			Serial.println("Connection Failed - change Config pls. Starting AP now.");
			mUsedSSID = "esp32_" + mUsedSSID;
			// no break
		}
	case eConnectMode_AP:
#ifndef REGION_AP_MODE
		// WLAN Access Point aktivieren
		WiFi.softAP(mUsedSSID.c_str());
		// Webserver auf Port 80 starten

		mOwnIp = WiFi.softAPIP().toString();

		WiFi.begin();
		// WLAN-Stromsparmodus aktivieren
		esp_wifi_set_ps((wifi_ps_type_t)1);


#endif
		wifi_config_t lwifi_config;
		esp_wifi_get_config(WIFI_IF_AP, &lwifi_config);
		Serial.println("############### MaxCon:" + lwifi_config.ap.max_connection);
		lwifi_config.ap.max_connection = 10;
		esp_wifi_set_config(WIFI_IF_AP, &lwifi_config);
		break;
	default:
		Serial.println("Invalid WLAN config found.");
		return;
	}
	
	mOwnMac = WiFi.macAddress();
	Serial.println("SSID: " + mUsedSSID);
	Serial.println("IP-Adress: " + mOwnIp);
	Serial.println("MAC-Adressc" + mOwnMac);
}

boolean WiFiConnector::isStarted(void) {
	return WiFi.isConnected();
}
