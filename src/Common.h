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
	NUM_MISSIONS
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
inline std::ostream& operator<< (std::ostream& os, EGGTYPE& var) {

	switch(var) {
	case EGG_INDETERMINATE:
		return os << "Indeterminate Egg";
	case EGG_WHITE:
		return os << "White Egg";
	case EGG_BROWN:
		return os << "Brown Egg";
	case EGG_MULTI:
		return os << "Creme Egg";
	default:
		break;
	}

	return os << "Unknown Egg Type!";
}

inline std::ostream& operator<< (std::ostream& os, LINECOLOUR& var) {

	switch(var) {
	case BLACK:
		return os << "black";
	case WHITE:
		return os << "white";
	}

}

inline std::ostream& operator<< (std::ostream& os, ABS_DIRECTION& var) {

	switch(var) {
	case NORTH:
		return os << "North";
	case EAST:
		return os << "East";
	case SOUTH:
		return os << "South";
	case WEST:
		return os << "West";
	default:
		return os << "Invalid Direction";
	}

}

inline std::ostream& operator<< (std::ostream& os, DROPOFF_POINT& var) {

	switch(var) {
	case DP_1:
		return os << "Dropoff Point 1";
	case DP_2:
		return os << "Dropoff Point 2";
	case DP_3:
		return os << "Dropoff Point 3";
	case DP_ANYWHERE:
		return os << "Anywhere!";

	case NUM_DP:
	default:
		return os << "Invalid Dropoff Point";
	}
}

inline std::ostream& operator<< (std::ostream& os, COLLECTION_POINT& var) {

	switch(var) {
	case CP_0:
		return os << "Collection Point 0";
	case CP_1:
		return os << "Collection Point 1";
	case CP_2:
		return os << "Collection Point 2";
	case CP_3:
		return os << "Collection Point 3";
	case CP_4:
		return os << "Collection Point 4";

	case NUM_CP:
	default:
		return os << "Invalid Collection Point";
	}
}


#endif /* COMMON_H_ */
