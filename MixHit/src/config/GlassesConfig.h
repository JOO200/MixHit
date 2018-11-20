/*
 * GlassesConfig.h
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#ifndef MAIN_CONFIG_GLASSESCONFIG_H_
#define MAIN_CONFIG_GLASSESCONFIG_H_

#include "cGlasses.h"
#include <string>

class GlassesConfig
{
public:
	static bool fromJson(std::string file, cGlasses glasses);
};


#endif /* MAIN_CONFIG_GLASSESCONFIG_H_ */
