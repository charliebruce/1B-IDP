/*
 * HAL.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 */

#include "HAL.h"

#include "Log.h"

robot_link rlink;

HAL::HAL(int robot) {

	INFO("[HAL] Intialising.");

	if(!rlink.initialise(robot)) {
		ERR("[HAL] Something went wrong when trying to connect to the robot.");
		rlink.print_errs();
	}



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
	//TODO implement this
	INFO("[HAL] LED setting not yet implemented.");
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
