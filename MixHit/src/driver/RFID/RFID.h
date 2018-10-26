#ifndef _RFID_H_
#define _RFID_H_

//#include "../mixer/Configuration.h"
//#include "I2C.h"
#include "C:\Users\Soeren\Documents\GitHub\MixHit\MixHit\src\driver\RFID\LL\MFRC522.h"

struct RfidData
{
	uint32_t  Bestellnummer;
	uint16_t  CocktailNr;
	uint16_t  Status;
	uint16_t  mlProFlasche[8];
	uint8_t  Name[16];
	uint8_t  NameCocktail[32];
	uint8_t  LieferDatum[8];
	uint8_t  CRC;
};


class RFID: public MFRC522
{
public:


	bool tagAvailable();			
	bool writeData(RfidData data);
	bool readData(RfidData &data);
	bool getDrinkStatus();
	bool setDrinkStatus();



private:
	uint8_t adr = 0;
	uint16_t intPin = 0;
	MIFARE_Key key;
	MIFARE_Key SecretKey;
};





#endif
