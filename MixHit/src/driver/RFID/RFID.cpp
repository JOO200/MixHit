#include "RFID.h"

RFID::RFID(uint16_t intPin)
{
	pinMode(intPin, INPUT_PULLUP); // Pullup?
	attachInterrupt(digitalPinToInterrupt(intPin), handleInterrupt, FALLING); //check edge and create int function!


}

RFID::RFID(uint8_t adr, uint16_t intPin)
{
}

RFID::~RFID()
{
}

bool RFID::tagAvailable()
{
	return false;
}

bool RFID::writeData(RfidData data)
{
	return false;
}

bool RFID::readData(RfidData & data)
{
	return false;
}

bool RFID::getDrinkStatus()
{
	return false;
}

bool RFID::setDrinkStatus()
{
	return false;
}
