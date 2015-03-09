/*
 * ObjectRecognition.cpp
 *
 *  Created on: 24 Feb 2015
 *      Author: Charlie
 */


#include "ObjectRecognition.h"

#include "Log.h"


EGGTYPE identify(HAL* hal) {

	//Illuminate the egg with a light
	hal->ledSet(LED_CLRA, true);
	hal->ledSet(LED_CLRB, false);

	//Let the value settle
	delay(1000);

	//Read the ADC value
	SENSOR_DATA sa = hal->sensorRead(SENSOR_EGG_LDR);

	//Illuminate the egg with a different light
	hal->ledSet(LED_CLRA, false);
	hal->ledSet(LED_CLRB, true);

	//Let the value settle
	delay(1000);

	//Read the ADC value again
	SENSOR_DATA sb = hal->sensorRead(SENSOR_EGG_LDR);

	//Switch the lights off
	hal->ledSet(LED_CLRA, false);
	hal->ledSet(LED_CLRB, false);

	//Read the egg "line" sensor value in the dark (no need for delay)
	SENSOR_DATA sc = hal->sensorRead(SENSOR_EGG_LDR);

	//TODO determine if weighing the egg using mechanical/lift timing is necessary

	//Based on the values of sa, sb, sc infer the type of egg
	DEBUG("[REC] Values: " << sa.intensity <<", "<< sb.intensity <<", "<<sc.intensity);

	//TODO implement

	return EGG_WHITE;

}
