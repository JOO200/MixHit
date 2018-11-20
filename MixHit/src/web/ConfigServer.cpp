#include "ConfigServer.h"
#include "esp_log.h"
#include "cJSON.h"
#include <stdio.h>

#define CONFIG_PATH "/spiffs/config"

ConfigServer::ConfigServer() {
	mServer = new HttpServer();
}

void ConfigServer::start() {
	mServer->setRootPath(CONFIG_PATH);
	mServer->setDirectoryListing(true);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,
		"/",
		&ConfigServer::configIndex);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_GET,	// ConfigList
		"/configs",
		&ConfigServer::onConfigListRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_POST,
		"/config",
		&ConfigServer::onConfigRequest);
	mServer->addPathHandler(HttpRequest::HTTP_METHOD_POST,	// Config Save
		"/config_save",
		&ConfigServer::onConfigSaveRequest);
	mServer->start(8080);
}

/* STATIC */ void ConfigServer::onConfigListRequest(HttpRequest * request, HttpResponse * response)
{
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = CONFIG_PATH "/configs.json";
	response->sendFile(fileName, 4 * 1024);
	response->close();
}

/* STATIC */ void ConfigServer::configIndex(HttpRequest * request, HttpResponse * response)
{
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = "/spiffs/web/html/config.html";
	response->sendFile(fileName, 4 * 1024);
	response->close();
}

/* STATIC */ void ConfigServer::onConfigRequest(HttpRequest * request, HttpResponse * response)
{
	std::string json = request->getBody();
	cJSON * root = cJSON_Parse(json.c_str());
	if (root == NULL || !cJSON_HasObjectItem(root, "file")) {
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid Json");
		response->close();
		if (root != NULL) cJSON_Delete(root);
		return;
	}
	std::string file = cJSON_GetObjectItem(root, "file")->valuestring;
	response->setStatus(HttpResponse::HTTP_STATUS_OK, "OK");
	response->addHeader(HttpRequest::HTTP_HEADER_CONTENT_TYPE, "application/json");
	std::string fileName = CONFIG_PATH;
		fileName += file.c_str();
	response->sendFile(fileName, 4 * 1024);
	response->close();
	cJSON_Delete(root);
}

/* STATIC */ void ConfigServer::onConfigSaveRequest(HttpRequest * request, HttpResponse * response)
{
	std::string json = request->getBody();
	cJSON * root = cJSON_Parse(json.c_str());
	if (!cJSON_HasObjectItem(root, "path") || !cJSON_HasObjectItem(root, "config"))
	{
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Invalid Json");
		response->close();
		if (root != NULL) cJSON_Delete(root);
		return;
	}
	std::string path = cJSON_GetObjectItem(root, "path")->valuestring;
	cJSON * config = cJSON_GetObjectItem(root, "config");
	std::string configString = cJSON_Print(config);
	if (root != NULL) cJSON_Delete(root);	// Wir sind hier fertig mit allem JSON-Kram.
	FILE * f = fopen(path.c_str(), "wb");
	if (f == NULL) {
		response->setStatus(HttpResponse::HTTP_STATUS_BAD_REQUEST, "Error while writing file.");
		response->close();
		return;
	}
	fprintf(f, configString.c_str());
	fclose(f);
}
