#ifndef _WIFI_CONNECTOR_H
#define _WIFI_CONNECTOR_H

#include "../libs/MyString.h"
#include "WiFiServer.h"
#include "esp_wifi.h"

enum eWiFiConnectMode {
	eConnectMode_Invalid = 0,
	eConnectMode_AP = 1,
	eConnectMode_Client = 2
};

class WiFiConnector {
public:
	WiFiConnector(String pConfigFile);
	void start(void);
	boolean isStarted(void);

private:
	eWiFiConnectMode mMode;
	String mUsedSSID;
	String mUsedPassword;
	String mOwnIp;
	String mOwnMac;
	WiFiServer mServer;
};

#endif // _WIFI_CONNECTOR_H