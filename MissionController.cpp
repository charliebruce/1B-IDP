/*
 * MissionController.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 */

#include "MissionController.h"

#include "Log.h"

void MissionController::MissionController(void) {

	TRACE("[MC] Constructor.");

	//Initialise our internal variables to match the state of the game field

	//Game rules define the number and type of eggs
	eggsRemaining[EGG_WHITE] = 2;
	eggsRemaining[EGG_BROWN] = 2;
	eggsRemaining[EGG_MULTI] = 1;
	eggsRemaining[EGG_INDETERMINATE] = 0;

	//At the start, we have not deposited any eggs
	eggsPlaced = 0;

	TRACE("[MC] Created.");

}
