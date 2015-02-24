/*
 * MissionController.cpp
 *
 *  Created on: 16 Feb 2015
 *      Author: Charlie
 */

#include "MissionController.h"

#include "Log.h"

HAL* hal;

Navigation nav;

MissionController::MissionController(HAL* h) {

	hal = h;

	TRACE("[MC] Constructor.");

	//Initialise our internal variables to match the state of the game field

	//Game rules define the number and type of eggs
	eggsRemaining[EGG_WHITE] = 2;
	eggsRemaining[EGG_BROWN] = 2;
	eggsRemaining[EGG_MULTI] = 1;
	eggsRemaining[EGG_INDETERMINATE] = 0;

	dropoffForEgg[EGG_WHITE] = DP_1;
	dropoffForEgg[EGG_BROWN] = DP_2;
	dropoffForEgg[EGG_MULTI] = DP_3;
	dropoffForEgg[EGG_INDETERMINATE] = DP_ANYWHERE; //If we really can't determine the type and can't handle, then discard anywhere.

	//At the start, we have not deposited any eggs
	eggsPlaced = 0;

	TRACE("[MC] Created.");

}

MissionController::~MissionController() {

	TRACE("[MC] Destructor.");

}

void MissionController::FunctionalTests(void) {

	hal->networkTest();

	hal->ledTest();

	hal->motorTest();

	hal->sensorTest();

}

void MissionController::RunMission(void) {

	INFO("[MC] Starting mission.");
	stopwatch missionTimer;
	missionTimer.start();

	//Wait until we are placed on the starting position
	//We assume that if we are on the starting position, we will read white for both central sensors

	//If we weren't started on the white lines, but now we are, give 3 seconds for the person to get
	//their hands free.

	const static int TIMELIMIT = 5 * 60 * 1000; //5 minutes in ms

	//While the game is still active
	while (totalEggsRemaining() > 0 && (missionTimer.read() < TIMELIMIT))
	{
		INFO("[MC] Navigating to the next occupied egg collection point.");
		//Navigate to the nearest occupied egg collection point

		//TODO more efficient to generate all possible routes, then choose shortest?
		//ie goToBestCollectionPoint returns a COLLECTION_POINT
		COLLECTION_POINT nextCP = nav.getNearestEggyCP();
		nav.travelToCP(nextCP);

		INFO("[MC] Picking up the egg.");
		//Attempt to pick up the egg
		//TODO this

		//Mark the collection point as unoccupied
		nav.setNoEgg(nextCP);

		//Identify the egg using the sensor(s)
		EGGTYPE e = identify(hal);

		INFO("[MC] Identified as " << e << ".");

		//Sanity check the type of egg
		if(eggsRemaining[e] <= 0) {

			ERR("[MC] We must have mis-identified an egg! Identified as " <<
					e << " but there are none left in play.");

			//TODO what course of action should be taken?

			//If there is only one egg remaining, it is logical to assume that it is this egg
			if(totalEggsRemaining() == 1) {

			}
			//If there are multiple eggs in play, we should re-scan until a sensible answer is received.
			else {

			}
		}

		INFO("[MC] Acting as if this is a " << e << ".");

		//Signal the type using the LEDs
		signalEggType(e, hal);

		//Work out the desired dropoff point
		DROPOFF_POINT dp = dropoffForEgg[e];

		//Navigate to the correct dropoff point
		nav.travelToDP(dp);

		//Deposit the egg in the egg cup

		INFO("[MC] Egg has been deposited.");

		//Update the total number of eggs remaining
		eggsRemaining[e]--;
		eggsPlaced++; //This is technically redundant - it should always be equal to (5 - totalEggsRemaining())...

		//Stop displaying the LED pattern now that we've deposited the egg.
		stopSignalling(hal);

	}

	INFO("[MC] Mission complete.");
}


