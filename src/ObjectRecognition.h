/*
 * ObjectRecognition.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	This is responsible for identifying the type of the object, based on the raw data from the sensor(s).
 *
 *	This could be done in a very primitive way (ie just manually defining thresholds on a single variable),
 *	or with more advanced sampling (ie taking multiple readings from multiple sensors, getting a better
 *	estimate of colour). The data could then be grouped multi-dimensionally with "learning" samples. The
 *	choice of method should be determined by writing a simple threshold-based algorithm, and testing that
 *	algorithm over a sample of objects to determine reliability.
 */

#ifndef OBJECTRECOGNITION_H_
#define OBJECTRECOGNITION_H_

#include "HAL.h"
#include "Common.h"

//Return the best guess at the egg type for the given sensor data
//Electrical failures (ie 0 or 255?) should result in an INDETERMINATE response
EGGTYPE identify(HAL* h);


#endif /* OBJECTRECOGNITION_H_ */
