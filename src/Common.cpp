/*
 * Common.cpp
 *
 *  Created on: 23 Feb 2015
 *      Author: Charlie
 */


#include "Common.h"

#include <stopwatch.h>

#include "Log.h"

//Blocking delay function
void delay(int ms) {

	if(ms < 0)
	{
		WARN("Delaying for a negative time is stupid: " << ms);
		return;
	}
	if(ms > 5000) {
		WARN("Wasting time by delaying this long:" << ms);
	}

	TRACE("Delaying for " << ms);

	stopwatch sw;
	sw.start();

	while(sw.read() < ms)
		;

	sw.stop();

}

//This should make enums of the eggtype print nicely - hopefully it works!
std::ostream& operator<<  (std::ostream& os, EGGTYPE var) {

	switch(var) {
	case EGG_INDETERMINATE:
		return os << "Indeterminate Egg";
	case EGG_WHITE:
		return os << "White Egg";
	case EGG_BROWN:
		return os << "Brown Egg";
	case EGG_MULTI:
		return os << "Creme Egg";
	default:
		break;
	}

	return os << "Unknown Egg Type!";
}

