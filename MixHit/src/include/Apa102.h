/*
 * Apa102.h
 *
 *  Created on: Oct 22, 2017
 *      Author: kolban
 */

#ifndef COMPONENTS_CPP_UTILS_APA102_H_
#define COMPONENTS_CPP_UTILS_APA102_H_

#include "SmartLED.h"
#include "AdvSPI.h"

class Apa102: public SmartLED {
public:
	Apa102();
	virtual ~Apa102();
	void init();
	void show();
private:
	AdvSPI mySPI;
};

#endif /* COMPONENTS_CPP_UTILS_APA102_H_ */
