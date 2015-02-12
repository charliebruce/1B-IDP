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

//The indeterminate case should hopefully never be used but exists for completeness - in this situation
//the logical behaviour would be to either deposit in the nearest bin, or put back/discard the egg.
enum EGGTYPE {
	EGG_WHITE,
	EGG_BROWN,
	EGG_SPECIAL,
	EGG_INDETERMINATE
};

//Return the best guess at the egg type for the given sensor data
EGGTYPE identify(SENSOR_DATA s);


#endif /* OBJECTRECOGNITION_H_ */
