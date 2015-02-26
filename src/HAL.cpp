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
//TODO is the assumption that the high 24 bits are ignored correct? uint8_t?
int port0;

HAL::HAL(int robot) {

	INFO("[HAL] Intialising.");

	int result;

#ifdef __arm__
	INFO("[HAL] Connecting to local machine (on ARM).");
	result = rlink.initialise();
#else
	INFO("[HAL] Connecting to remote robot "<<robot<<" (on workstation).");
	result = rlink.initialise(robot);
#endif

	if(!result) {
		ERR("[HAL] Something went wrong when trying to connect to the robot.");
		rlink.print_errs();
	}

	//Set the robot's initial state.
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

	//Motor configuration
	//TODO complete this
	//TODO experiment with ramp
	//According to CUED it ranges from 0 (no ramping) to 254 (slow ramping)
	rlink.command (RAMP_TIME, 0);
	handleErrors();

	//Motors all off
	motorSet(MOTOR_LEFT, 0.0);
	motorSet(MOTOR_RIGHT, 0.0);
	motorSet(MOTOR_LIFT, 0.0);

	//Sensors, etc
	//TODO this
}


//Handle link errors
void HAL::handleErrors(void) {

	if(rlink.any_errs()) {
		WARN("[HAL] Handling errors not yet implemented.");

		//TODO implement error catching and handling
		rlink.print_errs();

		if(rlink.fatal_err()) {
			rlink.reinitialise();
		}

	}

}

LINE_SENSOR_DATA HAL::lineRead(void) {

	LINE_SENSOR_DATA lsd;

	int vals = rlink.request(READ_PORT_1);
	DEBUG("Port 0 read: " << vals);

	//Sensor 1 is on pin 0 (dec 1) - front left
	//Sensor 2 is on pin 1 (dec 2)
	//Sensor 3 is on pin 2 (dec 4)
	//Sensor 4 is on pin 3 (dec 8)

	lsd.fl = (vals & (1<<0));
	lsd.fc = (vals & (1<<1));
	lsd.fr = (vals & (1<<2));
	lsd.rc = (vals & (1<<3));

	bool invert = true;

	//White is true, black is false, but this logic depends on wiring - inverting IS required
	if (invert) {
		lsd.fl = !lsd.fl;
		lsd.fc = !lsd.fc;
		lsd.fr = !lsd.fr;
		lsd.rc = !lsd.rc;
	}

	return lsd;
}

void HAL::sensorTest(void) {

	INFO("[HAL] Sensor test starting.");

	stopwatch sw;
	LINE_SENSOR_DATA lsd;
	sw.start();
	for(int i = 0; i< 10; i++) {

		//Every 500ms
		while(sw.read() < i * 500)
			;

		//Read the value from the sensor
		lsd = lineRead();

		//Output the values in a readable form to the console
		INFO("Front sensors:\t" << ((lsd.fl == WHITE)?"white\t":"black\t") << ((lsd.fc == WHITE)?"white\t":"black\t") << ((lsd.fr == WHITE)?"white\t":"black\t") );

	}

	//Now test the limiting micro-switches.



	//TODO this




	//Now test the LDR


	//TODO this




	//Finally test the mechanical limit switches


	//TODO this



}

void HAL::ledSet(LED led, bool on) {

	//TODO LED CLRA and CLRB

	//TODO ensure that pin numbering matches physical hardware
	DEBUG("[HAL] LED: "<<led<<" set to "<<on);

	int mask = 0;
	bool invert = false; //If a HIGH pin turns the light OFF, this should be TRUE.

	switch(led) {
	case LED_LEFT:
		//Left LED is on pin 0
		mask = (1 << 0);
		invert = false;
		break;
	case LED_MIDD:
		//Middle LED is on pin 1
		mask = (1 << 1);
		invert = false;
		break;
	case LED_RGHT:
		//Middle LED is on pin 2
		mask = (1 << 2);
		invert = false;
		break;
	case LED_CLRA:
		//Colour A LED is on pin 3
		mask = (1 << 3);
		invert = false;
		break;
	case LED_CLRB:
		//Colour B LED is on pin 4
		mask = (1 << 4);
		invert = false;
		break;
	case NUM_LEDS:
	default:
		WARN("[HAL] Incorrect use of ledSet function (wrong LED specified).");
		return;
	}

	if(on != invert) {
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

void HAL::motorSet(MOTOR m, float rate) {

	TRACE("Motor: "<<m<<" rate: "<<rate);


	//If the magnitude of the velocity is more than 1
	if ((rate * rate) > 1)
	{
		WARN("[HAL] Received a command to run a motor at a speed greater than 1.0 - clamping.");
		rate = ((rate > 0.0)?1.0:-1.0);
	}

	command_instruction cmd = MOTOR_1_GO;
	bool reverse_motor = false;
	//Find the motor port with the given meaning
	switch (m) {
	case MOTOR_LEFT:
		cmd = MOTOR_1_GO;
		reverse_motor = true;
		break;
	case MOTOR_RIGHT:
		cmd = MOTOR_2_GO;
		reverse_motor = true;
		rate = rate * 0.5;//Gear ratio compensation on test robot
		break;
	case MOTOR_LIFT:
		cmd = MOTOR_3_GO;
		reverse_motor = true;
		break;

	default:
		WARN("[HAL] Invalid motor commanded.");
		return;

	}

	if (reverse_motor) {
		rate = rate * -1.0;
	}

	TRACE("Rate is now "<<rate);
	//0-128: velocity, with high bit representing reversed motors
	int sig = 0;

	if (rate < 0) {
		//Run in reverse
		sig = 0b10000000;
		rate = rate * -1.0;
	}

	int iRate = (int) (127.0 * rate);

	TRACE("iRate "<<iRate<<" sig "<<sig);
	//Set the low bits with a guard
	sig |= (iRate & 0b01111111);

	TRACE("Command: " << cmd << ". Signal: "<< sig); 

	//Send the command to the hardware
	rlink.command(cmd, sig);

	handleErrors();

}

void HAL::motorTest(void) {

	INFO("[HAL] Motor test starting.");

	//Test all motors by driving forwards
	motorSet(MOTOR_LEFT, 1.0);
	motorSet(MOTOR_RIGHT, 1.0);

	//Then, after 2 seconds
	stopwatch sw;
	sw.start();
	while(sw.read() < 2000) ;
	sw.start();

	INFO("[HAL] Motor test - reversing.");

	//Drive backwards
	motorSet(MOTOR_LEFT, -1.0);
	motorSet(MOTOR_RIGHT, -1.0);

	sw.start();
	while(sw.read() < 2000) ;

	INFO("[HAL] Motor tests finished.");


	motorSet(MOTOR_LEFT, 0.0);
	motorSet(MOTOR_RIGHT, 0.0);
}

void HAL::networkTest(void) {

#ifdef __arm__
	INFO("[HAL - Functional Tests] Running on robot; skipping network tests.");
	return;
#endif

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
