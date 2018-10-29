#ifndef _RFID_H_
#define _RFID_H_

//#include "../mixer/Configuration.h"
//#include "I2C.h"
#include "LL/MFRC522.h"

#define THRESHOLD_BIG_SMALL_COCKTAIL 200		// Threshold for cocktail size: over -> big cocktail, under -> small cocktail;

struct RfidData
{
	uint32_t  Bestellnummer;		
	uint16_t  CocktailNr;
	uint16_t  Status;					// use the upper 8 bits to determine the priority of the cocktail (0, 1, 2)
	uint16_t  mlProFlasche[8];
	uint8_t  Name[16];
	uint8_t  NameCocktail[32];
	uint8_t  LieferDatum[8];
};


class RFID: public MFRC522
{
public:


	bool tagAvailable();			
	bool writeData(RfidData data);
	bool readData(RfidData &data);
	bool getDrinkStatus(uint8_t &status);
	bool setDrinkStatus(uint8_t status);
	bool changeSecretKey();
	bool addDrinkToMixerQueue(RfidData &data);



private:
	uint8_t adr = 0;
	uint16_t intPin = 0;
};





#endif
