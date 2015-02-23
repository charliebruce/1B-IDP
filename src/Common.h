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

//TODO define this
static const int ROBOT_NUM = 11;

enum LINECOLOUR {
	BLACK = false,
	WHITE = true
};

#endif /* COMMON_H_ */
