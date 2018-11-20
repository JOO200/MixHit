#ifndef _MY_WEBSERVER_H_
#define _MY_WEBSERVER_H_

#include "../include/HttpServer.h"

//#define WEB_ROOT_PATH "/spiffs/web"

class MyWebServer {
public:
	MyWebServer();
	void start();
private:
	static void indexRequest(HttpRequest * request, HttpResponse * response);
	static void listRequest(HttpRequest * request, HttpResponse * response);
	static void makeCocktail(HttpRequest * request, HttpResponse * response);
	HttpServer * mServer;
};

#endif // _MY_WEBSERVER_H_
