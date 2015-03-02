/*
 * LineFollowing.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 *
 *	Contains implementation of line following logic.
 *	Makes implicit assumptions about the configuration of the sensors and the layout of the board.
 *	These are generally reasonable:
 *
 *	Sensor spacing assumed to be slightly wider than the line
 *	Lines are long enough that pivoting about one wheel will never end up with us losing the line
 *
 */

#include "LineFollowing.h"

#include "Log.h"


void junctionStraight(HAL* hal) {

	//Just shoot a little forwards, then re-align oneself on the line.
	stopwatch sw;
	hal->motorSet(MOTOR_LEFT, 0.1);
	hal->motorSet(MOTOR_RIGHT, 0.1);
	sw.start();

	//Cut out after half a second or junction just traversed.
	while(sw.read() < 500) {

		//Read the sensors
		//If two are black, we must have passed the junction and be back on the line


	}


	//Line ourselves back up straight with the line (BWB).
	//sensors = hal->lineRead()

	//When going straight, our rear sensor should remain over the line
	//TODO check this condition is met?

	//Finally, stop.
	hal->motorSet(MOTOR_LEFT, 0.0);
	hal->motorSet(MOTOR_RIGHT, 0.0);

}

//TODO implement
void junctionTurn(bool left, HAL* hal) {

	//Turning right is the same as turning left, but with the sensors and motors flipped.

	//Slowly (so slowly that we can't "jump" the lines)
	//turn just far enough to lose the line completely (BBB)
	//by pivoting about the correct wheel

	//Then keep going until the front centre sensor hits white again.

	//Done. STOP!

	//Preempt that and slow down by looking at the left or right sensor?

	//TODO Estimate the time taken, error condition if it all goes wrong...!

}

//Follow the line we are currently on until we reach a node, pointing straight
//Estimated Distance is to ensure that we don't massively overshoot - stops us driving off to infinity.
void followLineToNext(int lineDistance, bool justWentStraight, bool approachingTJunctionFromSide, HAL* h) {

	//Use a primitive estimate from the average velocity of the robot
	stopwatch watchdog;
	watchdog.start();

	float mtrL = 0.4;
	float mtrR = 0.4;

	//Assume we start lined up correctly.
	//Also assume that the sensors are exactly 8cm in front of the wheel centreline

	//If we've turned, our sensors are 8cm in front of the line
	int estimatedDistanceRemaining = lineDistance - 8;

	//If we've crossed a junction, we've got the entire length to travel.
	if(justWentStraight)
		estimatedDistanceRemaining += 8;

	//Sanity check the remaining distance - no junctions should be within 8cm of each other.
	if(estimatedDistanceRemaining <= 0) {
		ERR("[LF] We are being asked to follow a line, but the distance remaining is " << estimatedDistanceRemaining);
		return;
	}

	const int velocityReciprocal = 100; //in milliseconds per centimeter
	while(watchdog.read() < (250 + 1.2 * (estimatedDistanceRemaining * velocityReciprocal))) { //compensate for 250ms of motor lag, max 20% overshoot

		int time_iteration_begin = watchdog.read();

		//Read the sensor state
		LINE_SENSOR_DATA sensors = h->lineRead();
		//TODO determine if repeat readings help?


		//If we lose the white line on our rear sensor there is a BIG PROBLEM!
		if(sensors.rc != WHITE) {
			//TODO implement recovery strategy, downgrade this to a warning
			ERR("[LF] Loss of line on rear sensor! Unable to correct. Continuing, but maybe into the abyss!");
			//return;
		}

		//If junctions are regularly detected falsely, sensor spacing needs to be increased.

		if ((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == WHITE)){
			INFO("[LF] We might well have reached an X-junction! Party party party.");
			if (approachingTJunctionFromSide) {
				ERR("[LF] Was expecting T junction but found what appears to be X!");
				//TODO appropriate recovery strategy
			}
			return;
		}
		//we have reached a T from the side
		else if (	((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) 	||
					((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == WHITE))	)
		{
			INFO("[LF] We might well have reached a T-junction! (or are approaching an X at a slight angle). Party.");
			if (!approachingTJunctionFromSide) {
				ERR("[LF] Was expecting X junction but found what appears to be T!");
				//TODO recovery strategy
			}
			return;
		}


		//If we're in a situation where we're seeing BBB, we're either in a transitional period
		//(ie the robot is starting to drift, but the spacing of the sensor means we haven't caught it')
		//That is OK as long as we check frequently enough that the line is "trapped" at all times
		//A total loss would be bad. Testing will determine if this is a problem.
		if((sensors.fr == BLACK) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) {
			DEBUG("[LF] BBB seen. Decrease sensor spacing if seen frequently? Momentary.");
			continue;
		}



		//TODO We assume that the spacing of the sensors is just greater than the line width?
		//TODO: Very primitive implementation here: improve with PID/smoothing

		if ((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) {
			//We are on track: full speed ahead!
			mtrL = 0.4;
			mtrR = 0.4;
		}

		if (sensors.fl == WHITE) {
			//We need to turn left slightly: slow the left wheel down
			mtrL = 0.0;
			mtrR = 0.6;
		}

		if (sensors.fr == WHITE) {
			//We need to turn right slightly: slow the right wheel down
			mtrL = 0.6;
			mtrR = 0.0;

		}

		h->motorSet(MOTOR_LEFT, mtrL);
		h->motorSet(MOTOR_RIGHT, mtrR);

		//TODO A small time delay here to reduce jitter?
		//Delay for 10ms
		//while(watchdog.read() < time_iteration_begin + 10) ;

	}

	watchdog.stop();

}
