/*
 * MissionController.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 */

#include "MissionController.h"


void MissionController::MissionController(void) {

	//Initialise our internal variables to match the state of the game field

	//Game rules define the number and type of eggs
	eggsRemaining[EGG_WHITE] = 2;
	eggsRemaining[EGG_BROWN] = 2;
	eggsRemaining[EGG_MULTI] = 1;
	eggsRemaining[EGG_INDETERMINATE] = 0;

	//At the start, we have not scored any points
	eggsPlaced = 0;
}
