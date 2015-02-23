/*
 * LineFollowing.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */

#include "LineFollowing.h"

#include "Log.h"


//Follow the line we are currently on until we reach WWWW (indicating that our front sensors are on a node, pointing straight)
//Estimated Distance is to ensure that we don't massively overshoot - stops us driving off to infinity.
void followLineToNext(int estimatedDistance, HAL* h) {

	//Use a primitive estimate from the average velocity of the robot
	stopwatch watchdog;
	watchdog.start();


	//A is the LEFT motor when RIGHT-following, and vice versa
	float mtrA = 0.4;
	float mtrB = 0.4;


	const int velocityReciprocal = 100; //in millieconds per centimeter
	while(watchdog.read() < (250 + 1.2 * (estimatedDistance * velocityReciprocal))) { //compensate for 250ms of motor lag, max 20% overshoot

		int time_iteration_begin = watchdog.read();


		//Read the sensor state
		LINE_SENSOR_DATA sensors = h->lineRead();
		//TODO determine if repeat readings help?


		//If we lose the white line on our rear sensor there is a BIG PROBLEM!
		if(sensors.rc != WHITE) {
			//TODO implement recovery strategy, downgrade this to a warning
			ERR("[LF] Temporary loss of line! Unable to correct, returning.");
			return;
		}

		if ((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == WHITE)){
			INFO("[LF] We might well have reached an X-junction! Party party party.");
			return;
		}
		else if ((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == BLACK))
		{
			INFO("[LF] We might well have reached a T-junction! (or are approaching an X at a slight angle). Party.");
			return;
		}


		//TODO We assume that the spacing of the sensors is just greater than the line width?
		//TODO: Very primitive implementation here: improve with PID/smoothing

		if ((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) {
			//We are on track: full speed ahead!
			mtrA = 0.4;
			mtrB = 0.4;
		}

		if (sensors.fl == WHITE) {
			//We need to turn left slightly: slow the left wheel down
			mtrA = 0.1;
		}

		if (sensors.fr == WHITE) {
			//We need to turn right slightly: slow the right wheel down
			mtrB = 0.1;
		}

		//If we're in a situation where we're seeing BBB, we're either in a transitional period
		//(ie the robot is starting to drift, but the spacing of the sensor means we haven't caught it')
		//That is OK as long as we check frequently enough that the line is "trapped" at all times
		//A total loss would be bad. Testing will determine if this is a problem.
		if(sensors.fr == sensors.fc == sensors.fr == BLACK) {
			DEBUG("[LF] BBB seen. Decrease sensor spacing if seen frequently?");
		}


		h->motorSet(MOTOR_LEFT, mtrA);
		h->motorSet(MOTOR_RIGHT, mtrB);


	}

	watchdog.stop();

}
