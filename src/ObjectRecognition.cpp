/*
 * ObjectRecognition.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: Charlie
 */


#include "ObjectRecognition.h"


EGGTYPE identify(HAL* hal) {

	//Illuminate the egg with a light
	hal->ledSet(LED_CLRA, true);
	hal->ledSet(LED_CLRB, false);

	//Read the ADC value
	SENSOR_DATA sa = hal->sensorRead(SENSOR_0);

	//Illuminate the egg with a different light
	hal->ledSet(LED_CLRA, false);
	hal->ledSet(LED_CLRB, true);

	//Read the ADC value again
	SENSOR_DATA sb = hal->sensorRead(SENSOR_0);

	//From the given ADC values, give our best guess value for the colour of the egg
	//TODO this

	return EGG_WHITE;

}
