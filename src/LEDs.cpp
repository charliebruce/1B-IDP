/*
 * LEDs.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 *
 *	Signal the type of the egg to the judge using the LEDs on the robot.
 *
 *	Specification:
 *	If a WHITE egg is detected, the lit pattern will be "001"
 *	If a BROWN egg is detected, the lit pattern will be "010"
 *	If a MULTI egg is detected, the lit pattern will be "011"
 *	If the type cannot be determined, the pattern "100" will be displayed.
 *
 *
 *	This pattern will remain lit until the egg has been deposited.
 *
 *	The first LED in the pattern will be on the left of the robot when viewed
 *	from above, with the claw pointing forwards. "1" represents an "On" LED.
 */

#include "LEDs.h"

#include "Log.h"

void stopSignalling(HAL* h) {
	h->ledSet(LED_LEFT, false);
	h->ledSet(LED_MIDD, false);
	h->ledSet(LED_RGHT, false);
}

void signalEggType(EGGTYPE e, HAL* h) {

	DEBUG("[LED] Signalling egg type: " << e);

	switch (e) {
	case EGG_WHITE:
		TRACE("[LED] Lighting LEDs for White egg.");
		h->ledSet(LED_LEFT, false);
		h->ledSet(LED_MIDD, false);
		h->ledSet(LED_RGHT, true);
		break;

	case EGG_BROWN:
		TRACE("[LED] Lighting LEDs for Brown egg.");
		h->ledSet(LED_LEFT, false);
		h->ledSet(LED_MIDD, true);
		h->ledSet(LED_RGHT, false);
		break;

	case EGG_MULTI:
		TRACE("[LED] Lighting LEDs for Multi egg.");
		h->ledSet(LED_LEFT, false);
		h->ledSet(LED_MIDD, true);
		h->ledSet(LED_RGHT, true);
		break;

	case EGG_INDETERMINATE:
		TRACE("[LED] Lighting LEDs for Indeterminate egg.");
		h->ledSet(LED_LEFT, true);
		h->ledSet(LED_MIDD, false);
		h->ledSet(LED_RGHT, false);
		break;

	case NUM_EGGTYPES:
	default:
		WARN("[LED] incorrect use of signalEggType (eggtype not correct).");
	}

}
