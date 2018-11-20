#include "MyWebServer.h"
#include "../mixer/cCocktailMixer.h"
#include "../mixer/obj/cCocktail.h"
#include "../mixer/obj/cOrder.h"
#include "esp_log.h"

#define WEB_ROOT_PATH "/spiffs/web"

MyWebServer::MyWebServer() {
	mServer = new HttpServer();
}

void MyWebServer::start() {
	mServer->setRootPath(WEB_ROOT_PATH);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/index.html",
			&MyWebServer::indexRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/",
			&MyWebServer::indexRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
			"/cocktails",
			&MyWebServer::listRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_POST,
			"/cocktail",
			&MyWebServer::makeCocktail);
	mServer->start(80);
}

void MyWebServer::makeCocktail(HttpRequest * request, HttpResponse * response) {
	cCocktail * cocktail = cCocktail::fromJson(request->getBody());
	if(cocktail == NULL) {
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid JSON-Format");
	} else {
		response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	}
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	//Step 1: wrap RFID data in Cocktail class structure
	//Step 2: create an cOrder-object out of lCocktail
	cOrder lOrder = cOrder(*cocktail,				// cCocktail-object
		(cocktail->getFuel() > 200) ? 1 : 2,						// amount
		1);							// determine the priority queue

	//Step 3: add order to global mixer queue
	int receivedOrderNr = gCocktailMixer.addOrderToQueue(lOrder);
	response->sendData("{\"OrderNumber\":1}");	// TODO: Nicht immer 1 zurück senden.
	response->close();
	delete cocktail;
}

void MyWebServer::indexRequest(HttpRequest * request, HttpResponse * response) {
	std::string fileName = WEB_ROOT_PATH "/html/index_redirect.html"; // Build the absolute file name to read.
	response->sendFile(fileName, 4 * 1024);			// Send File with default buffer size
	response->close();
}

void MyWebServer::listRequest(HttpRequest * request, HttpResponse * response) {
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = "/spiffs/config/cocktails.json";
	response->sendFile(fileName, 4 * 1024);
	response->close();
}
