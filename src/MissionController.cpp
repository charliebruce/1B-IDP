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

void MissionController::LineTests(void) {


	followLineToNext(1000, true, false, hal);

}

void MissionController::RunMission(MISSION objective) {

	switch(objective) {
	case ALL_MISSIONS:
		//FALLTHROUGH INTENTIONAL

	case WEEK1_TESTS:

		INFO("[MC-W1] Performing Week 1 Tests.");
		hal->networkTest();
		hal->motorTest();
		hal->sensorTest();

		if(objective != ALL_MISSIONS)
			return;
		//Intentional fall-through for "all missions" option

	case FUNCTIONAL_DEMO_1:

		//•	Travel from the starting location to dropoff location D1 and return to the starting location.

		INFO("[MC-FD1] Travelling to DP1.");
		nav.travelToDP(DP_3, hal);
		INFO("[MC-FD1] Travelling to Start.");
		nav.travelToStart(hal);

		if(objective != ALL_MISSIONS)
			return;
		//Intentional fall-through

	case FUNCTIONAL_DEMO_2:

		//•	Successfully detect the types of three different eggs, and signal the type of the egg using the LEDs mounted to the robot.

		INFO("[MC-FD2] Egg Identification")
		for(int i = 0; i < 3; i++) {

			//Give a countdown
			INFO("Load an egg for identifying ("<<i<<" of 3):");
			for(int countdown = 5; countdown <= 0; countdown--) {
				delay(1000);
				INFO(countdown);
			}

			//Identify the egg using the sensor(s)
			EGGTYPE e = identify(hal);

			//Signal the type using the LEDs
			signalEggType(e, hal);

			INFO("[MC - FD2] Identified the egg (type = "<<e<<")."); //TODO a readable form of this enum?

		}

		if(objective != ALL_MISSIONS)
			return;
		//Intentional fall-through

	case FUNCTIONAL_DEMO_3:

		//•	Demonstrate the procedure of collecting and depositing an egg: operating pneumatic claw to grab the egg, operating the lift
		//mechanism to raise the egg (stopping when the upper limit switch is pressed). The same procedure is followed in reverse
		//to deposit the egg.

		//TODO implement this




		if(objective != ALL_MISSIONS)
			return;
		//Intentional fall-through

	case LINESENSOR_WIRING:

		hal->sensorTest();

		if(objective != ALL_MISSIONS)
			return;
		//Intentional fall-through

	case MAIN_MISSION:
		MainMission();
		return;

	default:
		ERR("Invalid mission objective given.");
	}

}

void MissionController::MainMission(void) {

	INFO("[MC] Starting main mission.");
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
		COLLECTION_POINT nextCP = nav.getNearestEggyCP();
		nav.travelToCP(nextCP, hal);

		INFO("[MC] Picking up the egg.");
		//Pick up the egg
		nav.collectEgg();

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
		nav.travelToDP(dp, hal);

		//Deposit the egg in the egg cup
		nav.dropoffEgg();

		INFO("[MC] Egg has been deposited.");

		//Update the total number of eggs remaining
		eggsRemaining[e]--;
		eggsPlaced++; //This is technically redundant - it should always be equal to (5 - totalEggsRemaining())...

		//Stop displaying the LED pattern now that we've deposited the egg.
		stopSignalling(hal);

	}
	//Bonus points for returning to the start

	//TODO This
	INFO("[MC] Mission complete.");
}


