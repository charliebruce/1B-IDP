/*
 * Main.cpp
 *
 *  Created on: 17 Feb 2015
 *      Author: Charlie
 */

#include "Common.h"
#include "HAL.h"
#include "MissionController.h"

#include "Log.h"


int main(void) {

	//Week 1: Initial testing.
	HAL hal(ROBOT_NUM);

	hal.networkTest();

	hal.ledTest();

	hal.motorTest();

	MissionController mc(&hal);

	mc.RunMission();


}


