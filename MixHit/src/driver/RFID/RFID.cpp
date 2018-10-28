#include "RFID.h"
#include "../MixHit/src/mixer/cCocktailMixer.h"		//include Queue of Orders class to wrap RFID data



bool RFID::tagAvailable()
{
	//Depends on Interrupt usage
	return false;
}

bool RFID::writeData(RfidData data)
{
	StatusCode status;
	uint8_t Sector0Buffer[32];
	uint8_t Sector1Buffer[48];
	//Sort uint32 into 4 uint8 for transfer
	for (int i = 0; i < 4; i++) {
		Sector0Buffer[i] = (uint8_t)(data.Bestellnummer >> 8 * (3 - i) && 0x000000FF); 
	}
	//Sort uint16 into 2 uint8 for transfer
	for (int i = 0; i < 2; i++) {
		Sector0Buffer[i+6] = (uint8_t)(data.CocktailNr >> 8 * (2 - i) && 0x00FF);
	}
	//Transfer Date into buffer
	for(int i = 0; i < 8; i++) {
		Sector0Buffer[i + 8] = data.LieferDatum[i];
	}
	//Transfer of ml array
	memcpy(&Sector0Buffer[16], &data.mlProFlasche[0],16);

	memcpy(&Sector1Buffer[0], &data.Name[0], 16);
	memcpy(&Sector1Buffer[16], &data.NameCocktail[0], 32);

	// Authenticate using key B
	status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 3, &stdKey, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		return false;
	}
	for (int i = 0; i < 2; i++) {
		status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(i+2, &Sector0Buffer[i*16], 16);
		if (status != MFRC522::STATUS_OK) {
			return false;
		}
	}

	// Authenticate using key B
	status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 3, &stdKey, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(i+4, &Sector1Buffer[i*16], 16);
		if (status != MFRC522::STATUS_OK) {
			return false;
		}
	}



	return true;
}

bool RFID::readData(RfidData & data)
{

	//temporary buffers for all data (data has checksums which we want to remove)
	uint8_t MiscBuffer[36];
	uint8_t NameBuffer[18];
	uint8_t CocktailNameBuffer[36];
	uint8_t size = 18; // var for the read command to check buffer size. Buffer is always large enougth!

	//Get Data from Sector 0
	StatusCode status;
	status = (StatusCode) PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 3, &stdKey, &(uid));
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(1, &MiscBuffer[0], &size);
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(2, &MiscBuffer[18], &size);
	if (status != STATUS_OK)
		return false;
	PCD_StopCrypto1();
	//Get Data from Sector 1
	status = (StatusCode) PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, &stdKey, &(uid));
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(4, &NameBuffer[0], &size);
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(5, &CocktailNameBuffer[0], &size);
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(6, &CocktailNameBuffer[0], &size);
	if (status != STATUS_OK)
		return false;
	PCD_StopCrypto1();



	// Sort bytes into the correct data types
	data.Bestellnummer = MiscBuffer[0] << 24 || MiscBuffer[1] << 16 || MiscBuffer[2] << 8 || MiscBuffer[3];
	data.CocktailNr	   = MiscBuffer[6] << 8  || MiscBuffer[7];

	memcpy(&data.LieferDatum[0], &MiscBuffer[8], 8);

	for (int i = 0; i < 8; i++) {
		data.mlProFlasche[i] = MiscBuffer[i * 2 + 18] << 8 || MiscBuffer[i * 2 + 19];
	}
	memcpy(&data.Name, &NameBuffer, 16);

	memcpy(&data.NameCocktail[0] , &CocktailNameBuffer[0], 16);
	memcpy(&data.NameCocktail[16], &CocktailNameBuffer[18], 16); // two steps to remove Checksum 
	
	return true;
}

bool RFID::getDrinkStatus(void /*uint8_t status*/)		// pk: changed parameter to void to avoid collision with StatusCode auth_status (formerly status) in namespace 
{
	StatusCode auth_status;
	uint8_t buffer[18];
	uint8_t size = sizeof(buffer);
	//Get Data from Sector 2 This sector has a special key for access restriction! The key is hardcoded.
	auth_status = (StatusCode)PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 11, &secretKey, &(uid));
	if (auth_status != STATUS_OK)
		return false;
	auth_status = (StatusCode)MIFARE_Read(8, &buffer[0], &size);
	if (auth_status != STATUS_OK)
		return false;
	PCD_StopCrypto1();


	return true;
}

bool RFID::setDrinkStatus(uint8_t status)
{
	uint8_t[16] temp;
	memcpy(&temp, 0, 16);
	// Authenticate using key B
	status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 11, &secretKey, &(mfrc522.uid));
	if (status != MFRC522::STATUS_OK) {
		return false;
	}
	status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(8, &temp[0], 1);
	if (status != MFRC522::STATUS_OK) {
		return false;
	}

	return true;
}

bool RFID::addDrinkToMixerQueue(RfidData &data)
{
	String CocktailName = "";
	vector<String> CocktailNames;
	vector<int> IngriedentsAmounts;
	int OrderedAmount;						// 1: half, 2: full
	int OrderPrio = (data.Status & 0xFF);	// get the upper 8 bits of status
	int recvdOrderNr;
	int total_ml = 0;
	extern cCocktailMixer gCocktailMixer;

	//Precautions
	for (int i = 0; i < 32; i++)			// combine the 32 name fields of the RFID tag
	{
		CocktailName += String(data.NameCocktail[i]).c_str();
	}

	for (int i = 0; i < 8; i++)					// create the vectors
	{
		if (data.mlProFlasche != 0)				// if the ingredient gets used in this cocktail
		{
			switch (i)							// check which reservoir is looked at currently
			{
			case 0:
				CocktailNames.push_back("Ananas");	// the ingredient needs a name in order to be compared with the reservoir info
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));	// hand over the amount of ingredient
				break;
			case 1:
				CocktailNames.push_back("Maracuja");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 2:
				CocktailNames.push_back("Malibu");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 3:
				CocktailNames.push_back("Wodka");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 4:
				CocktailNames.push_back("Zitrone");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 5:
				CocktailNames.push_back("Grenadine");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 6:
				CocktailNames.push_back("Orange");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			case 7:
				CocktailNames.push_back("Banane");
				IngriedentsAmounts.push_back((int(data.mlProFlasche[i])));
				break;
			}
		}
	}

	for (int i = 0; i < 8; i++)				// calculate if the order was a big or small cocktail
	{
		total_ml += data.mlProFlasche[i];
	}

	total_ml < THRESHOLD_BIG_SMALL_COCKTAIL ? OrderedAmount = 1 : OrderedAmount = 2;

	//Step 1: wrap RFID data in Cocktail class structure
	cCocktail lCocktail(CocktailName,		// name of cocktail
		CocktailNames,						// names of ingridients
		IngriedentsAmounts);

	//Step 2: create an cOrder-object out of lCocktail
	cOrder lOrder(lCocktail,				// cCocktail-object
		OrderedAmount,						// amount
		OrderPrio);							// determine the priority queue

	//Step 3: add order to global mixer queue
	recvdOrderNr = gCocktailMixer.addOrderToQueue(lOrder);

	if(recvdOrderNr == -1)
		return false;						// could not place order
	else
	{
		data.CocktailNr = (uint32_t)recvdOrderNr;
		return true;						//write received order and return
	}
}
