/*
 * WiFiConfig.h
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#ifndef CRESERVOIR_H_
#define CRESERVOIR_H_

#include <string>
#include <map>
#include "../mixer/obj/cReservoir.h"

class ReservoirConfig {
public:
	ReservoirConfig();
	ReservoirConfig(cReservoir* reservoir);

	void setInstance(cReservoir* reservoir);

	std::string getString(int pID);
	int getID(std::string pString);

	static bool fromFile(std::string file, cReservoir * reservoir);
	static bool fromJson(std::string json, cReservoir * reservoir);
	std::string toJson();
	void toFile(std::string file);
private:
	cReservoir * instance;
};



#endif /* CRESERVOIR_H_ */
