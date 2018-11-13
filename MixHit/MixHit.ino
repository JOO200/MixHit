/*
 Name:		MixHit.ino
 Created:	10/13/2017 3:48:42 PM
 Author:	User
*/

//#include <BluetoothSerial.h>
//#include <SPIFFS.h>
//#include <SparkFunSX1509.h>
//#include <Wire.h>
#include <dummy.h>
//#include "WiFi.h"
#include "src/mixer/Main_CocktailMixer.h"
#include "src/web/Main_WebServer.h"
#include "src/driver/RFID/RFID.h"

void loop_1(void * pvParameters);
void loop_2(void * pvParameters);
void loop_3(void * pvParameters);
#ifdef OPERATION_MODE_CM_IOT
void loop_4(void * pvParameters);
#endif

// the setup function runs once when you press reset or power the board
void setup() 
{
	#ifndef REGION_Serial_Start
	Serial.begin(115200);
	while (!Serial)
	{
		;
	}
	Serial.println("Serial_OK");
	#endif
	#ifndef REGION_SPIFFS_Start
	if (!SPIFFS.begin()) // Prueft, ob SPIFFS gestartet werden kann
	{ // Konnte nicht gestartet werden. Dies kann z.B. passieren, wenn auf dem verwendeten Controler noch kein SPIFFS vorhanden war.
		delay(1000);
		Serial.println("SPIFFS Mount FirstTime");
		if (!SPIFFS.begin(true)) // Erneut versuchen zu starten, diesmal mit beruecksichtigung, dass noch kein SPIFFS vorhanden war.
		{
			Serial.println("SPIFFS Mount Failed");
			return;
		}
	}
	Serial.println("SPIFFS_OK");
	#endif
	String lWerkseinstellungen = "";
	readFile(SPIFFS, "/Werkseinstellungen.xtxt", &lWerkseinstellungen);
	Serial.println(lWerkseinstellungen);
	int lLen = lWerkseinstellungen.length();
	if (lWerkseinstellungen != "no")
	{
		Serial.println("Lade Werkseinstellungen");
		gOLED.PrintFirstLine("Werkseinstellungen...");
		gOLED.DisplayLines();
		#ifndef REGION_DeleteAllFiles
		String lFiles = "";
		listDir(SPIFFS, "/", 0, &lFiles);
		vector<String> lFileNames;
		split(&lFiles, ';', &lFileNames);
		for (int i = 0; i < lFileNames.size(); i++)
		{
			deleteFile(SPIFFS, lFileNames[i]);
		}
		#endif
		WriteBootstrap();
		WriteDefaultConfigFile();
		WriteDefaultWiFiConfig();
		writeFile(SPIFFS, "/Config_Select.txt", "/Default_Config.txt"); // Default_Config als zu ladende Datei festlegen
		writeFile(SPIFFS, "/Config_Select.wtxt", "/Default_Config.wtxt"); // Default_Config als zu ladende Datei festlegen
		writeFile(SPIFFS, "/Werkseinstellungen.xtxt", "no");	
	}
	else
	{
		Serial.println("KEINE Werkseinstellungen");
	}
	gOLED.PrintFirstLine("");
	gOLED.DisplayLines();
	setup_WebServer();
	setup_CocktailMixer();
	xTaskCreatePinnedToCore(loop_1, "MixHit", 8192, NULL, 1, NULL, 1);
	xTaskCreatePinnedToCore(loop_2, "WEB", 16384, NULL, 1, NULL, 0);
	xTaskCreatePinnedToCore(loop_3, "OLED", 4096, NULL, 1, NULL, 0);
#ifdef OPERATION_MODE_CM_IOT
	TaskHandle_t RFIDTask;
	xTaskCreatePinnedToCore(loop_4, "RFID", 4096, NULL, 1, &RFIDTask, 0);
#endif
}

// the loop function runs over and over again until power down or reset
void loop() 
{
	delay(100);// loop_WebServer();
}
void loop_1(void * pvParameters)
{
	while (true)
	{
		loop_CocktailMixer();
	}
}
void loop_2(void * pvParameters)
{
	while (true)
	{
		loop_WebServer();
		/*if (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
			setup_WebServer();
		}*/
	}
}
void loop_3(void * pvParameters)
{
	while (true)
	{
		loop_OLED();
	}
}
#ifdef OPERATION_MODE_CM_IOT
void loop_4(void * pvParameters)
{

	// GPIO expander and OLED driver have already initialized the i2c bus
	RFID rfid1(RFID_READER_ADDR, RFID_READER_RST);
	rfid1.PCD_Init();   // Init MFRC522

	// check reade version
	byte version = rfid1.PCD_ReadRegister(rfid1.VersionReg);
	Serial.print("RFID Reader Version:");
	Serial.println(version,HEX);
	if (version == (byte)0x92) {
		loop_RFID(rfid1);
	}
	else {
		Serial.println("RFID: reader version unknown!");
		if (version == 0x00 || version == 0xFF) {
			Serial.println("RFID: reader does not respond!");
		}
	}		
}
#endif