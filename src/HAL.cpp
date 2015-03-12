/*
 * HAL.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 */

#include "HAL.h"

#define LOGLEVEL LL_DEBUG

#include "Log.h"

robot_link rlink;

//State of the pins connected to port0
//TODO is the assumption that the high 24 bits are ignored correct? uint8_t?
int port0, port1;

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
	port1 = 0;
	rlink.command(WRITE_PORT_0, port0);
	rlink.command(WRITE_PORT_1, port1);
	handleErrors();

	//Motor configuration
	//TODO complete this
	//TODO experiment with ramp
	//According to CUED it ranges from 0 (no ramping) to 254 (slow ramping)
	rlink.command (RAMP_TIME, 16); //TODO determine dieal value - was 10
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

bool HAL::switchRead(SWITCH s) {

	int vals0 = rlink.request(READ_PORT_0);
	//Currently no switches on port 1
	//int vals1 = rlink.request(READ_PORT_1);


	switch(s) {

	case SWITCH_EGG:
		return (vals0 & (1 << 0)); //TODO pin numbers

	case SWITCH_LIMIT_DOWN:
		return (vals0 & (1 << 1));

	case SWITCH_LIMIT_UP:
		return (vals0 & (1 << 2));

	//Error catching
	case NUM_SWITCHES:
	default:
		WARN("[HAL] Incorrect use of switchRead!" << s);
		return false;
	}
}

LINE_SENSOR_DATA HAL::lineRead(void) {

	LINE_SENSOR_DATA lsd;

	int vals = rlink.request(READ_PORT_1);
	TRACE("Port 1 read: " << vals);

	//Sensor 1 is on pin 0 (dec 1) - front left
	//Sensor 2 is on pin 1 (dec 2)
	//Sensor 3 is on pin 2 (dec 4)
	//Sensor 4 is on pin 3 (dec 8)

	lsd.fl = (vals & (1<<0));
	lsd.fc = (vals & (1<<1));
	lsd.fr = (vals & (1<<2));
	lsd.rc = (vals & (1<<3));

	bool invert = false;

	//White is true, black is false, but this logic depends on wiring - inverting IS NOT required
	if (invert) {
		lsd.fl = !lsd.fl;
		lsd.fc = !lsd.fc;
		lsd.fr = !lsd.fr;
		lsd.rc = !lsd.rc;
	}

	return lsd;
}

void HAL::lsTest(void) {

	LINE_SENSOR_DATA lsd;

	//Read the value from the sensor
	lsd = lineRead();

	//Output the values in a readable form to the console
	INFO("Front sensors (L, C, R):\t" << ((lsd.fl == WHITE)?"white\t":"black\t")
			<< ((lsd.fc == WHITE)?"white\t":"black\t")
			<< ((lsd.fr == WHITE)?"white\t":"black\t")
			<< " Rear: \t"
			<< ((lsd.rc == WHITE)?"white\n":"black\n") );

}

void HAL::carriageMove(CARRIAGE_POS desired) {

	if(desired == NUM_POS)

	//Operate lift up
	motorSet(MOTOR_LIFT, -1.0);

	//Until the upper limit is hit
	while(!switchRead(SWITCH_LIMIT_UP))
		;

	//Lower
	motorSet(MOTOR_LIFT, 0.0);
	motorSet(MOTOR_LIFT, 1.0);

	//Until the lower limit is hit
	while(!switchRead(SWITCH_LIMIT_DOWN))
		;

	//Then stop the mechanism.
	motorSet(MOTOR_LIFT, 0.0);
}

void HAL::switchTest(void) {
	INFO("Switches - Egg: " 	<< (switchRead(SWITCH_EGG)?"y":"n") <<
					"\t LU:" 	<< (switchRead(SWITCH_LIMIT_UP)?"y":"n") <<
					"\t LD:" 	<< (switchRead(SWITCH_LIMIT_DOWN)?"y":"n"));
}

void HAL::ldrTest(void) {
	SENSOR_DATA sa = sensorRead(SENSOR_EGG_LDR);
	SENSOR_DATA sb = sensorRead(SENSOR_EGG_LF);

	DEBUG("LDR: " << sa.intensity << ", \tLF: " << sb.intensity);
	delay(500);
}

void HAL::sensorTest(void) {

	INFO("[HAL] Sensor test starting.");

	for(int i = 0; i < 20; i++) {

		//Every 500ms
		delay(500);

		//Line sensors
		lsTest();

		//Limit switches
		switchTest();

		//LDR
		ldrTest();

	}

}

void HAL::ledSet(LED led, bool on) {

	//TODO ensure that pin numbering matches physical hardware
	DEBUG("[HAL] LED: "<<led<<" set to "<<on);

	int mask = 0;
	int* port = &port0;

	bool invert = false; //If a HIGH pin turns the light OFF, this should be TRUE.

	switch(led) {
	case LED_LEFT:
		//Left LED is on pin B4, port 1
		mask = (1 << 4);
		port = &port1;
		break;
	case LED_MIDD:
		//Middle LED is on B5, port 1
		mask = (1 << 5);
		port = &port1;
		break;
	case LED_RGHT:
		//Middle LED is on B6, port 1
		mask = (1 << 6);
		port = &port1;
		break;
	case LED_CLRA:
		//Colour A LED is on pin 6, port 0
		mask = (1 << 6);
		port = &port0;
		break;
	case LED_CLRB:
		//Colour B LED is on pin 7, port 0
		mask = (1 << 7);
		port = &port0;
		break;
	case NUM_LEDS:
	default:
		WARN("[HAL] Incorrect use of ledSet function (wrong LED specified).");
		return;
	}

	if(on != invert) {
		*port |= mask;
	} else {
		*port &= ~mask;
	}

	TRACE("Port0 now "<<port0);
	TRACE("Port1 now "<<port1);

	rlink.command(WRITE_PORT_0, port0);
	rlink.command(WRITE_PORT_1, port1);

	handleErrors();

}


void HAL::ledTest(void) {

	for(int i = 0; i<2; i++) {
		INFO("[HAL] LED Test: Setting all LEDs on.");
		ledSet(LED_LEFT, true);
		delay(1000);
		ledSet(LED_MIDD, true);
		delay(1000);
		ledSet(LED_RGHT, true);
		delay(1000);
		ledSet(LED_CLRA, true);
		delay(1000);
		ledSet(LED_CLRB, true);

		handleErrors();

		delay(1000);

		INFO("[HAL] LED Test: Setting all LEDs off again.");
		ledSet(LED_LEFT, false);
		ledSet(LED_MIDD, false);
		ledSet(LED_RGHT, false);
		ledSet(LED_CLRA, false);
		ledSet(LED_CLRB, false);

		handleErrors();

		delay(1000);
	}

}


void HAL::pneumaticOperation(PNEUMATIC p, bool state) {

	int mask = 0;

	switch(p) {
	case PNEU_NC:
		//Port 0 B4
		mask = (1 << 4);
		break;
	case PNEU_CLAW:
		//Port 0 B5
		mask = (1 << 5);
		break;

	case NUM_PNEUS:
	default:
		WARN("Incorrect use of pneumatic operation: "<< p <<", "<<state);
	}

	if(state) {
		port0 |= mask;
	} else {
		port0 &= ~mask;
	}

	rlink.command(WRITE_PORT_0, port0);

}

SENSOR_DATA HAL::sensorRead(LDR_SENSOR s) {

	SENSOR_DATA sd;

	switch(s) {

	case SENSOR_EGG_LDR:
		//Read from the egg-pointing LDR
		sd.intensity = rlink.request(ADC0);
		break;

	case SENSOR_EGG_LF:
		//Read from the line-following sensor pointing at the egg
		sd.intensity = rlink.request(ADC1);
		break;

	default:
		WARN("Incorrect use of sensorRead: " << s);
		break;

	}

	//If the sensor is undefined, we return an undefined value.
	return sd;

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
		reverse_motor = false;
		//NO LONGER REQUIRED - Borrowed Eric's Motor! Thanks :)
		//rate = rate * 0.575;//Gear ratio compensation on test robot
		break;
	case MOTOR_LIFT:
		cmd = MOTOR_3_GO;
		reverse_motor = true;
		break;

	default:
		WARN("[HAL] Invalid motor commanded.");
		return;

	}

	static float rawrates[NUM_MOTORS] = {0,0,0};
	//if(rawrates[m] = -rate) //CUED BUG? Compensate by zeroing?

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
	delay(2000);

	INFO("[HAL] Motor test - reversing.");

	//Drive backwards
	//CUED BUG?
	//motorSet(MOTOR_LEFT, 0.0);
	//motorSet(MOTOR_RIGHT, 0.0);
	delay(50);

	//motorSet(MOTOR_LEFT, -1.0);
	//motorSet(MOTOR_RIGHT, -1.0);

	//After 2 more seconds
	delay(2000);

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
