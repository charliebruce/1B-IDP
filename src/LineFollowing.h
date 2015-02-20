/*
 * LineFollowing.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	See:
 *  http://www-h.eng.cam.ac.uk/help/tpl/IDP/linefollowing.html
 *
 *	TODO determine sensor layout/communicate with mechanical team regarding placement of the sensors.
 *
 */

#ifndef LINEFOLLOWING_H_
#define LINEFOLLOWING_H_

#include "HAL.h"


//Used to navigate to/around + junctions
void followLineToNext(int estimatedDistance, HAL* h);

//Used to find T junctions
void followEdgeToNext(bool left, int estimatedDistance, HAL* h);



#endif /* LINEFOLLOWING_H_ */
