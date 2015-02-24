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

//The number on the wireless dongle attached to the robot.
static const int ROBOT_NUM = 11;

//Type safeness is key
enum LINECOLOUR {
	BLACK = false,
	WHITE = true
};

void delay(int ms);

#endif /* COMMON_H_ */
