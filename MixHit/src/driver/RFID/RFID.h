#ifndef _CMAGNETSENSOR_H_
#define _CMAGNETSENSOR_H_

#include "../mixer/Configuration.h"
#include "I2C.h"
#include "MFRC522.h"


struct RfidData
{
	uint8_t[4]  UID;
	uint8_t[2]  Bestellnummer;
	uint8_t     CocktailNr;
	uint8_t     Status;
	uint8_t[8]  mlProFlasche;
	uint8_t[16] Name;
	uint8_t[5]  LieferDatum;
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
	
}





#endif
