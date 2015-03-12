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


static const int juncDelay = 1000;

void reverseJustBeyondJunction(HAL* h) {

	//Back up until on the junction
	//Work around CUED bug
	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);

	h->motorSet(MOTOR_LEFT, -1.0);
	h->motorSet(MOTOR_RIGHT, -1.0);

	//Keep reversing until the junction
	while(true) {
		LINE_SENSOR_DATA lsd = h->lineRead();

		int numBlacks = 0;
		if(lsd.fc == BLACK)
					numBlacks++;

		if(lsd.fl == BLACK)
					numBlacks++;

		if(lsd.fr == BLACK)
					numBlacks++;

		if(numBlacks >= 2) //Any 2 or more blacks mean we are beyond an X
			break;

	}

	//CUED bug compensation
	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);

}

void reverseToJunction(HAL* h) {

	//Back up until on the junction
	//Work around CUED bug	
	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);

	h->motorSet(MOTOR_LEFT, -1.0);
	h->motorSet(MOTOR_RIGHT, -1.0);

	//Keep reversing until the junction
	while(true) {
		LINE_SENSOR_DATA lsd = h->lineRead();

		if((lsd.fc == WHITE) && ((lsd.fl == WHITE) || (lsd.fr == WHITE))) //Any 2 or 3 are white
			break;
	}

	//Reverse slightly more TODO THIS
	//delay(250);

	//CUED bug compensation
	h->motorSet(MOTOR_LEFT, 0.0);
	h->motorSet(MOTOR_RIGHT, 0.0);

}

void centreOnLine(HAL* h) {
	//If we see BBB, bump in one direction until we see a sensor change

	//If we see either edge hit, compensate in the opposing direction

	//If we see BWB, we're done

	//If we see 2+ W, we're stuck and can't align without reversing further. Abort.
}

void uTurn(HAL* h) {

	//Junction turn left
	junctionTurn(true, h);

	//Reverse to the junction
	reverseToJunction(h);

	//Junction turn again
	junctionTurn(true, h);
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
		if( 	((sensors.fl == WHITE) && (sensors.fc == BLACK) && (sensors.fr == BLACK)) ||
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
		h->motorSet(MOTOR_LEFT, -0.3);
		h->motorSet(MOTOR_RIGHT, 1.0);
	}
	else {
		h->motorSet(MOTOR_LEFT, 1.0);
		h->motorSet(MOTOR_RIGHT, -0.3);
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

float clamp(float val, float low, float high) {

	if(val < low)
		return low;
	if(val > high)
		return high;
	return val;

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

	const int velocityReciprocal = 100; //in milliseconds per centimeter TODO improve this estimate

	//Positive error = too far right
	static int errs = 0; //Maintain this variable between runs.

	while(true) { //compensate for 250ms of motor lag, max 20% overshoot

		int time_iteration_begin = watchdog.read();

		//Read the sensor state
		LINE_SENSOR_DATA sensors = h->lineRead();

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
			mtrL = 0.9;
			mtrR = 0.9;

			//errs = 0;

			//Deal with over-compensation...

			//If we had previously erred right
			if(errs > 0) {
				//We are angled too far left now, drag slightly on the right wheel
				//Magic constant should reduce this issue
				mtrR = mtrR * 0.65;
				errs--;
			}

			if(errs < 0) {
				//We are angled too far right now, drag slightly on the left wheel
				//Magic constant should reduce this issue
				mtrL = mtrL * 0.65;
				errs++;
			}


		}

		if (sensors.fl == WHITE) {
			//We need to turn left slightly: slow the left wheel down
			DEBUG("Err r: " << errs);
			const float damp = 100;
			mtrL = 0.5; //0.725 * (1 - (1.0/damp) * clamp(((float) errs / damp), 0.0, damp));
			mtrR = 1.0;
			errs++;
		}

		if (sensors.fr == WHITE) {
			//We need to turn right slightly: slow the right wheel down
			DEBUG("Err l: " << errs);
			const float damp = 100;
			mtrL = 1.0;
			mtrR = 0.5; //0.725 * (1 - (1.0/damp) * clamp( ((float) errs / damp), 0.0, damp));
			errs--;
		}


		//Prevent repeated motor updates - speeeeed.		
		static float mtrLOld = 0.0;
		static float mtrROld = 0.0;
		
		if(mtrL != mtrLOld) {
			mtrLOld = mtrL;
			h->motorSet(MOTOR_LEFT, mtrL);
		}
		if(mtrR != mtrROld) {
			mtrROld = mtrR;
			h->motorSet(MOTOR_RIGHT, mtrR);
		}

		//TODO A small time delay here to reduce jitter?
		//Delay for 10ms
		//while(watchdog.read() < time_iteration_begin + 10) ;
		delay(10);

	}

	watchdog.stop();

}
