/*
 * HAL.cpp
 *
 *  Created on: 13 Feb 2015
 *      Author: Charlie
 */

#include "HAL.h"

#include "Log.h"

robot_link rlink;

void HAL::HAL(int robot) {

	rlink.initialise(robot);

}


void HAL::resetRobot(void) {
	INFO("[HAL] Resetting robot.")
	//TODO implement
}
