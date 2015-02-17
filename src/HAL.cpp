/*
 * HAL.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 */

#include "HAL.h"

#include "Log.h"

robot_link rlink;
int port0;

HAL::HAL(int robot) {

	INFO("[HAL] Intialising.");

	if(!rlink.initialise(robot)) {
		ERR("[HAL] Something went wrong when trying to connect to the robot.");
		rlink.print_errs();
	}

	//We start with the GPIOs all off
	port0 = 0;

}


HAL::~HAL(void) {

	TRACE("[HAL] Destructor.");

}


void HAL::resetRobot(void) {
	INFO("[HAL] Resetting robot.");

	WARN("[HAL] Reset not yet implemented.");
	//TODO implement
}


//Handle link errors
void HAL::handleErrors(void) {

	if(rlink.any_errs()) {
		WARN("[HAL] Handling errors not yet implemented.");

		//TODO implement error catching

	}

}


void HAL::ledSet(LED led, bool on) {

	//TODO ensure that pin numbering matches physical hardware
	DEBUG("[HAL] LED: "<<led<<" set to "<<on);

	int mask = 0;
	switch(led) {
	case LED_LEFT:
		//Left LED is on pin 0
		mask = (1 << 0);
		break;
	case LED_MIDD:
		//Middle LED is on pin 1
		mask = (1 << 1);
		break;
	case LED_RGHT:
		//Middle LED is on pin 2
		mask = (1 << 2);
		break;
	}

	//TODO ensure that this is correct (is the LED wired such that a written 1 is ON or OFF?).
	if(on) {
		port0 |= mask;
	} else {
		port0 &= ~mask;
	}

	rlink.command(WRITE_PORT_0, port0);

	handleErrors();

}


void HAL::ledTest(void) {

	INFO("[HAL] LED Test: Setting all LEDs on.");
	ledSet(LED_LEFT, true);
	ledSet(LED_MIDD, true);
	ledSet(LED_RGHT, true);

	//Wait for approximately 3 seconds in a platform-independent way.
	stopwatch sw;
	sw.start();
	while(sw.read() < 3000)
		;
	sw.stop();

	INFO("[HAL] LED Test: Setting all LEDs off again.");
	ledSet(LED_LEFT, false);
	ledSet(LED_MIDD, false);
	ledSet(LED_RGHT, false);

}


void HAL::networkTest(void) {

	//Perform latency tests and reliability. Uses CUED's "stopwatch" code.

	stopwatch sw;
	int successes = 0;
	int linkerr = 0;
	int miscerr = 0;

	sw.start();

	for(int i = 0; i<100; i++)
	{
		int val = rlink.request(TEST_INSTRUCTION);

		if(val == REQUEST_ERROR) {
			linkerr++;
		}
		else if (val == TEST_INSTRUCTION_RESULT) {
			successes++;
		}
		else {
			miscerr++;
		}

	}

	int time = sw.stop();
	INFO("[HAL] Network reliability and network test results follow (100 repeats):")
	INFO("[HAL] Time = " << time << ", successes = "<<successes<<", linkerr = "<<linkerr<<", miscerr = "<<miscerr);



}
