/*
 * ObjectRecognition.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	This is responsible for identifying the type of the egg.
 *
 *	This could be done in a very primitive way (ie just manually defining thresholds on a single variable),
 *	or with more advanced methods (ie taking multiple readings from multiple sensors or under different
 *	lighting conditions, getting a better estimate of colour). The chosen method will depend on the results
 *	from testing.
 */

#ifndef OBJECTRECOGNITION_H_
#define OBJECTRECOGNITION_H_

#include "HAL.h"
#include "Common.h"

//Return the best guess at the egg type for the given sensor data
//Electrical failures (ie 0 or 255?) should result in an INDETERMINATE response
//This code might control one or more coloured LEDs that illuminates the egg, to give
//more information about what colour the egg might be
EGGTYPE identify(HAL* h);


#endif /* OBJECTRECOGNITION_H_ */
