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


