#ifndef _CONFIGSERVER_H_
#define _CONFIGSERVER_H_

#include "../include/HttpServer.h"

class ConfigServer {
public:
	ConfigServer();
	void start();
private:
	static void configIndex(HttpRequest * request, HttpResponse * response);
	static void onConfigRequest(HttpRequest * request, HttpResponse * response);
	static void onConfigListRequest(HttpRequest * request, HttpResponse * response);
	static void onConfigSaveRequest(HttpRequest * request, HttpResponse * response);
	HttpServer * mServer;
};

#endif // _CONFIGSERVER_H_
