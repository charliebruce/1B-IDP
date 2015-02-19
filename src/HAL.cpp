/*
 * HAL.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 */

#include "HAL.h"

#include "Log.h"

robot_link rlink;

//State of the pins connected to port0
int port0;

HAL::HAL(int robot) {

	INFO("[HAL] Intialising.");

	if(!rlink.initialise(robot)) {
		ERR("[HAL] Something went wrong when trying to connect to the robot.");
		rlink.print_errs();
	}

	resetRobot();


}


HAL::~HAL(void) {

	TRACE("[HAL] Destructor.");

}


void HAL::resetRobot(void) {

	INFO("[HAL] Resetting robot.");

	//We start with the GPIOs all off
	port0 = 0;
	rlink.command(WRITE_PORT_0, port0);
	handleErrors();

	//Motors all off
	//TODO complete this
	//handleErrors();

	//Motor ramp settings
	//TODO complete this
	//handleErrors();
}


//Handle link errors
void HAL::handleErrors(void) {

	if(rlink.any_errs()) {
		WARN("[HAL] Handling errors not yet implemented.");

		//TODO implement error catching and handling
		rlink.print_errs();
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

	//If the hardware is wired such that HIGH is OFF then reverse this.
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

	handleErrors();

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

	handleErrors();

}

void HAL::motorTest(void) {
	//TODO experiment with ramp
    rlink.command (RAMP_TIME, 120);

    //Test all motors by driving forward indefinitely.
	rlink.command(MOTOR_1_GO, 127);
	rlink.command(MOTOR_2_GO, 127);
	rlink.command(MOTOR_3_GO, 127);
	rlink.command(MOTOR_4_GO, 127);

	stopwatch sw;
	sw.start();
	while(sw.read() < 2000) ;
	sw.start();

	rlink.command(MOTOR_1_GO, 255);
	rlink.command(MOTOR_2_GO, 255);
	rlink.command(MOTOR_3_GO, 255);
	rlink.command(MOTOR_4_GO, 255);


	sw.start();
	while(sw.read() < 2000) ;
}

void HAL::networkTest(void) {

	//Perform latency tests and reliability. Uses CUED's "stopwatch" code.

	stopwatch sw;
	int successes = 0;
	int linkerr = 0;
	int miscerr = 0;

	sw.start();
	static const int N = 100;
	for(int i = 0; i<N; i++)
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
	INFO("[HAL] Network reliability and network test results follow ("<<N<<" repeats):");
	INFO("[HAL] Time = " << time << ", successes = "<<successes<<", linkerr = "<<linkerr<<", miscerr = "<<miscerr);



}
