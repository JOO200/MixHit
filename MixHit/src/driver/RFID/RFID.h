#ifndef _RFID_H_
#define _RFID_H_

//#include "../mixer/Configuration.h"
//#include "I2C.h"
#include "C:\Users\Soeren\Documents\GitHub\MixHit\MixHit\src\driver\RFID\LL\MFRC522.h"

struct RfidData
{
	uint8_t  UID[4];
	uint8_t  Bestellnummer[2];
	uint8_t     CocktailNr;
	uint8_t     Status;
	uint8_t  mlProFlasche[8];
	uint8_t Name[16];
	uint8_t  LieferDatum[5];
	uint8_t     CRC;
};


class RFID
{
public:
	RFID(uint16_t intPin);
	RFID(uint8_t adr, uint16_t intPin);
	~RFID();

	bool tagAvailable();			
	bool writeData(RfidData data);
	bool readData(RfidData &data);
	bool getDrinkStatus();
	bool setDrinkStatus();



private:
	uint8_t adr = 0;
	uint16_t intPin = 0;
	
};





#endif
