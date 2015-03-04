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

#define LOGLEVEL LL_TRACE

#include "Log.h"


static const int juncDelay = 3000;

void uTurn(HAL* h) {

	//Spin 90 degrees on the spot twice
	//Stop when the centre sensor hits the white
	float uSpeed = 0.5;

	h->motorSet(MOTOR_LEFT, -1.0*uSpeed);
	h->motorSet(MOTOR_RIGHT, 1.0*uSpeed);

	TRACE("U turn: 1");

	delay(200);
	//Wait until we've seen RH sensor go white
	while(h->lineRead().fr != WHITE)
		;

	delay(200);

	TRACE("U turn: 2");
	//We're now in the BBB region

	//Rotate until we hit the centre of the next one
	while(h->lineRead().fc != WHITE)
		;

	delay(200);

	TRACE("U turn: 3");
	//Wait until we've seen RH sensor go white again
	while(h->lineRead().fr != WHITE)
		;

	delay(200);
	TRACE("U turn: 4");
	//Wait until we've lined up with the line again
	while(h->lineRead().fc != WHITE);


	TRACE("U turn: Fin");

	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);


}

void junctionStraight(HAL* h) {
	TRACE("[LF] Starting junctionStraight");
	stopwatch watchdog;
	watchdog.start();

	//move forwards, then re-align oneself on the line.
	h->motorSet(MOTOR_LEFT, 1.0);
	h->motorSet(MOTOR_RIGHT, 1.0);

	//Cut out after half a second or junction just traversed.
	while(watchdog.read() < 500) {

		//Read the sensors
		LINE_SENSOR_DATA sensors = h->lineRead();
		//If two OR MORE are black, we must have passed the junction and be back on the line
		if( ((sensors.fl == WHITE) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) ||
			((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) || 
			((sensors.fl == BLACK) && (sensors.fc == BLACK) && (sensors.fr == WHITE)) ||
			((sensors.fl == BLACK) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) )
		{
			break;
		}
	}


	//Line ourselves back up straight with the line (BWB).
	//sensors = hal->lineRead()

	//When going straight, our rear sensor should remain over the line
	//TODO check this condition is met?

	//Finally, stop.
	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);
	
	//For debugging, stop for some time at every straight junction.
	delay(juncDelay);

	watchdog.stop();
	TRACE("[LF] Ending junctionStraight");
}

//TODO implement
void junctionTurn(bool left, HAL* h) {
	TRACE("[LF] Starting junctionTurn: " << left);
	//Turning right is the same as turning left, but with the sensors and motors flipped.

	stopwatch watchdog;
	watchdog.start();

	//Slowly (so slowly that we can't "jump" the lines)
	//turn just far enough to lose the line completely (BBB)
	//by pivoting about the correct wheel
	if (left) {
		h->motorSet(MOTOR_LEFT, -0.4);
		h->motorSet(MOTOR_RIGHT, 1.0);
	}
	else {
		h->motorSet(MOTOR_LEFT, 1.0);
		h->motorSet(MOTOR_RIGHT, -0.4);
	}
	while(true) {
		LINE_SENSOR_DATA sensors = h->lineRead();
		if((sensors.fl == BLACK) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) {
			break;
		}
	}
	//Then keep going until the front centre sensor hits white again.
	while(true)
	{
		LINE_SENSOR_DATA sensors = h->lineRead();
		if(sensors.fc == WHITE)
		{
			break;
		}
	}
	//Done. STOP!
	
	//Preempt that and slow down by looking at the left or right sensor?

	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);

	//Delay for some delay for debugging
	delay(juncDelay);

	//TODO Estimate the time taken, error condition if it all goes wrong...!
	watchdog.stop();
	TRACE("[LF] Ending junctionTurn");
}

//Follow the line we are currently on until we reach a node, pointing straight
//Estimated Distance is to ensure that we don't massively overshoot - stops us driving off to infinity.
void followLineToNext(int lineDistance, bool justWentStraight, bool approachingTJunctionFromSide, HAL* h) {

	//Use a primitive estimate from the average velocity of the robot
	stopwatch watchdog;
	watchdog.start();

	float mtrL = 1.0;
	float mtrR = 1.0;
	
	//Stop it from sitting forever if we stop at a junction aligned well, velocity 0.0
	h->motorSet(MOTOR_LEFT, mtrL);
	h->motorSet(MOTOR_RIGHT, mtrR);
	
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

	int errs = 0;

	while(true) { //compensate for 250ms of motor lag, max 20% overshoot
		
		int time_iteration_begin = watchdog.read();

		//Read the sensor state
		LINE_SENSOR_DATA sensors = h->lineRead();

		//DEBUG("We see "<<sensors.fl <<", "<<sensors.fc <<", "<<sensors.fr);


		//If we lose the white line on our rear sensor there is a BIG PROBLEM!
		if(sensors.rc != WHITE) {
			//TODO implement recovery strategy, downgrade this to a warning
			//ERR("[LF] Loss of line on rear sensor! Unable to correct. Continuing, but maybe into the abyss!");
			//return;
		}

		//If junctions are regularly detected falsely, sensor spacing needs to be increased.

		if ((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == WHITE)){
			INFO("[LF] Found an X junction.");
			if (approachingTJunctionFromSide) {
				ERR("[LF] Was expecting T junction but found X!");
				//TODO appropriate recovery strategy
			}
			return;
		}
		
		//we have reached a T from the side
		if (	((sensors.fl == WHITE) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) 	||
					((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == WHITE))	)
		{
			INFO("[LF] Found a T junction.");
			if (!approachingTJunctionFromSide) {
				ERR("[LF] Was expecting X junction but found T!");
				//TODO recovery strategy
			}
			return;
		}


		//If we're in a situation where we're seeing BBB, we're either in a transitional period
		//(ie the robot is starting to drift, but the spacing of the sensor means we haven't caught it')
		//That is OK as long as we check frequently enough that the line is "trapped" at all times
		//A total loss would be bad. Testing will determine if this is a problem.
		if((sensors.fl == BLACK) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) {
			//DEBUG("[LF] BBB seen. Decrease sensor spacing if seen frequently? Momentary.");
			continue;
		}



		//TODO We assume that the spacing of the sensors is just greater than the line width?
		//TODO: Very primitive implementation here: improve with PID/smoothing

		if ((sensors.fl == BLACK) && (sensors.fc == WHITE) && (sensors.fr == BLACK)) {
			//We are on track: full speed ahead!
			mtrL = 1.0;
			mtrR = 0.95;
			errs = 0;

		}

		if (sensors.fl == WHITE) {
			//We need to turn left slightly: slow the left wheel down
			mtrL = 0.5; // ((float) errs / 30.0);
			mtrR = 1.0;
			errs++;
		}

		if (sensors.fr == WHITE) {
			//We need to turn right slightly: slow the right wheel down
			mtrL = 1.0;
			mtrR = 0.5; // ((float) errs / 30.0);
			errs++;
		}

		h->motorSet(MOTOR_LEFT, mtrL);
		h->motorSet(MOTOR_RIGHT, mtrR);

		//TODO A small time delay here to reduce jitter?
		//Delay for 10ms
		//while(watchdog.read() < time_iteration_begin + 10) ;
		delay(20);

	}

	watchdog.stop();

}
