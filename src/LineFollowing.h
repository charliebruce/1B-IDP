/*
 * LineFollowing.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	See:
 *  http://www-h.eng.cam.ac.uk/help/tpl/IDP/linefollowing.html
 *
 *	Responsible for the driving motion of the robot, especially when concerned with moving around lines
 *
 */

#ifndef LINEFOLLOWING_H_
#define LINEFOLLOWING_H_

#include "HAL.h"
#include "Common.h"

//Pass straight over the junction we're at.
void junctionStraight(HAL* hal);

//Turn in the given direction (when at the given junction)
void junctionTurn(bool left, HAL* hal);

//Used to navigate to/around + junctions
void followLineToNext(int estimatedDistance, bool justWentStraight, bool approachingTJunction, HAL* h);


#endif /* LINEFOLLOWING_H_ */
