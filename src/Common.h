/*
 * Common.h
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 *
 *	This file contains the definitions of things that are shared in lots of places.
 *	For example, egg type needs to be known at both a low level (to recognise it from raw data)
 *	and at a high level (so that the mission controller can work out the drop zone).
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>

//Direction reference for orientation
//We arbitrarily define the raised section to be in the NORTH (ramp running NOR$
enum ABS_DIRECTION {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
};

//The indeterminate case should hopefully never be used but exists for completeness - in this situation
//the logical behaviour would be to either deposit in the nearest bin, or put back/discard the egg.
enum EGGTYPE {
	EGG_INDETERMINATE = 0,
	EGG_WHITE,
	EGG_BROWN,
	EGG_MULTI,
	NUM_EGGTYPES
};

//The five collection points
enum COLLECTION_POINT {
	CP_0 = 0,
	CP_1,
	CP_2,
	CP_3,
	CP_4,
	NUM_CP
};

//Dropping off the egg anywhere might be used in a recovery strategy, if an un-recoverable error state is reached.
enum DROPOFF_POINT {
	DP_1 = 0,
	DP_2,
	DP_3,
	DP_ANYWHERE,
	NUM_DP
};

//The various missions we can run
enum MISSION {
	MAIN_MISSION,
	WEEK1_TESTS,
	FUNCTIONAL_DEMO_1,
	FUNCTIONAL_DEMO_2,
	FUNCTIONAL_DEMO_3,
	LINESENSOR_WIRING,
	EFUNC_DEMO,
	NUM_MISISONS
};

//The number on the wireless dongle attached to the robot.
static const int ROBOT_NUM = 11;

//Type safeness is key
enum LINECOLOUR {
	BLACK = false,
	WHITE = true
};

void delay(int ms);

//This should make enums of the eggtype print nicely - hopefully it works!
std::ostream& operator<<  (std::ostream& os, EGGTYPE var) {

	switch(var) {
	case EGG_INDETERMINATE:
		return os << "Indeterminate Egg";
	case EGG_WHITE:
		return os << "White Egg";
	case EGG_BROWN:
		return os << "Brown Egg";
	case EGG_MULTI:
		return os << "Creme Egg";
	}

	return os << "Unknown Egg Type!";
}




#endif /* COMMON_H_ */
