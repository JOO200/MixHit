#include "RFID.h"


bool RFID::tagAvailable()
{
	//Depends on Interrupt usage
	return false;
}

bool RFID::writeData(RfidData data)
{
	uint8_t Sector0Buffer[32];
	uint8_t Sector1Buffer[48];
	uint8_t Sector2Buffer[16];

	//Sort uint32 into 4 uint8 for transfer
	for (int i = 0; i < 4; i++) {
		Sector0Buffer[i] = (uint8_t)(data.Bestellnummer >> 8 * (3 - i) && 0x000000FF); 
	}
	//Sort uint16 into 2 uint8 for transfer
	for (int i = 0; i < 2; i++) {
		Sector0Buffer[i+6] = (uint8_t)(data.CocktailNr >> 8 * (2 - i) && 0x00FF);
	}
	memcpy()




	//Transfer Date into buffer
	for (int i = 0; i < 8; i++) {
		Sector0Buffer[i + 8] = data.LieferDatum[i];
	}


	return false;
}

bool RFID::readData(RfidData & data)
{

	//temporary buffers for all data (data has checksums which we want to remove)
	uint8_t MiscBuffer[36];
	uint8_t StatusBuffer[18];
	uint8_t NameBuffer[18];
	uint8_t CocktailNameBuffer[36];
	uint8_t size = 18; // var for the read command to check buffer size. Buffer is always large enougth!

	//Get Data from Sector 0
	StatusCode status;
	status = (StatusCode) PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 3, &key, &(uid));
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
	status = (StatusCode) PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 7, &key, &(uid));
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

	//Get Data from Sector 2 This sector has a special key for access restriction! The key is hardcoded.
	status = (StatusCode) PCD_Authenticate(PICC_CMD_MF_AUTH_KEY_A, 11, &SecretKey, &(uid));
	if (status != STATUS_OK)
		return false;
	status = (StatusCode) MIFARE_Read(8, &StatusBuffer[0], &size);
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

	data.Status = StatusBuffer[0] << 8 || StatusBuffer[1];
	
	return true;
}

bool RFID::getDrinkStatus()
{
	return false;
}

bool RFID::setDrinkStatus()
{
	return false;
}
