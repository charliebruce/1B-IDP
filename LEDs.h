/*
 * LEDs.h
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 *
 *	Responsible for signalling the egg type to the judges.
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "HAL.h"
#include "Common.h"

void stopSignalling(void);
void signalEggType(EGGTYPE e);


#endif /* LEDS_H_ */
