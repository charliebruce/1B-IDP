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

	INFO("[HAL] Intialising.");

	if(!rlink.initialise(robot)) {
		ERR("[HAL] Something went wrong when trying to connect to the robot.");
		rlink.print_errs();
	}



}


void HAL::resetRobot(void) {
	INFO("[HAL] Resetting robot.");

	WARN("[HAL] Reset not yet implemented.");
	//TODO implement
}



//Handle link errors
void HAL::handleErrors(void) {

	if(rlink.any_errs()) {
		WARN("[HAL] Handling errors not yet implemented.");

		//TODO implement error catching

	}




}
