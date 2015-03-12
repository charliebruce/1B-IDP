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

	case WEEK1_TESTS:

		INFO("[MC-W1] Performing Week 1 Tests.");

		//hal->networkTest();
		//hal->ledTest();
		//while(1)
			//hal->motorTest();
			//hal->sensorTest();

		//Cycle the middle platform up and down


		for(int i = 0; i< 3; i++) {

			hal->carriageMove(POS_UP);
			delay(1000);
			hal->carriageMove(POS_DOWN);
			delay(1000);
		}

		return;

	case FUNCTIONAL_DEMO_1:

		//•	Travel from the starting location to dropoff location D1 and return to the starting location.

		INFO("[MC-FD1] Travelling to DP2.");
		nav.travelToDP(DP_2, hal);
		INFO("[MC-FD1] Travelling to Start.");
		nav.goHome(hal);

		return;

	case FUNCTIONAL_DEMO_2:

		//•	Successfully detect the types of three different eggs, and signal the type of the egg using the LEDs mounted to the robot.

		INFO("[MC-FD2] Egg Identification")
		for(int i = 0; i < 3; i++) {

			//Give a countdown
			INFO("Load an egg for identifying ("<<i<<" of 3):");
			for(int countdown = 5; countdown <= 0; countdown--) {
				delay(1000);
				INFO(countdown<<"...");
			}

			//Identify the egg using the sensor(s)
			EGGTYPE e = identify(hal);

			//Signal the type using the LEDs
			signalEggType(e, hal);

			INFO("[MC - FD2] Identified the egg (type = "<<e<<")."); //TODO a readable form of this enum?

		}

		return;

/*	case FUNCTIONAL_DEMO_3:

		//•	Demonstrate the procedure of collecting and depositing an egg: operating pneumatic claw to grab the egg, operating the lift
		//mechanism to raise the egg (stopping when the upper limit switch is pressed). The same procedure is followed in reverse
		//to deposit the egg.

		WARN("Functional Demo 3 messes with orientation: Cannot use in conjunction with other demos");

		nav.setForwardsDirection(SOUTH);
		INFO("[MC-FD3] Picking egg up.");
		nav.collectEgg(CP_0, hal); //Does not matter what collection point, really, as long as our orientation matches approach direction

		delay(3000);

		nav.setForwardsDirection(NORTH);
		INFO("[MC-FD3] Dropping egg off.");
		nav.dropoffEgg(DP_1, hal);

		return; //*/

	case FUNCTIONAL_DEMO_3:
		//Electrical functionality demo: Operate the actuators.
		//PASSED
		INFO("Electrical functionality demo. Pneumatic actuator channels will switch on and off in turn.");

		for(int i = 0; i< 5; i++) {
			while(true)
			{
				hal->pneumaticOperation(PNEU_CLAW, CLAW_CLOSED);
				delay(3000);

				if(!hal->switchRead(SWITCH_EGG))
					WARN("No egg detected");

				INFO("Identified: " << identify(hal));
	
				hal->pneumaticOperation(PNEU_CLAW, CLAW_OPEN);
				delay(5000);
			}	
		}

		return; //*/

	case LINESENSOR_WIRING:

		while(true)
			hal->lsTest(); //never returns

		return; //Never reached.

	case MAIN_MISSION:
		MainMission();
		return;

	case NUM_MISSIONS:
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
		//If we have one egg remaining, we can infer its colour
		//We can also estimate the amount of time it will take to do this.
		//Therefore we can work out if it is worth attempting to deposit the egg,
		//or to get the points for returning to the start.

		//Safety margin of 5 seconds
		static const int SAFETY_MARGIN = 5000;

		//If we have already collected 4 of the 5 eggs
		if(totalEggsRemaining() == 1) {
			int estimatedTimeToComplete = 60000;//Initially estimate 1 minute per egg - TODO improve this estimate

			//If it will take longer to complete the final egg than the time we have remaining,
			//we should sacrifice the final egg and return to the start
			if((SAFETY_MARGIN + estimatedTimeToComplete) > (TIMELIMIT - missionTimer.read()))
				break; //out of the main while loop - will cause us to return home.

		}


		INFO("[MC] Navigating to the next occupied egg collection point.");

		//Navigate to the nearest occupied egg collection point
		COLLECTION_POINT nextCP = nav.getNearestEggyCP();
		nav.travelToCP(nextCP, hal);

		INFO("[MC] Picking up the egg.");
		//Pick up the egg
		nav.collectEgg(nextCP, hal);

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
		nav.dropoffEgg(dp, hal);

		INFO("[MC] Egg has been deposited.");

		//Update the total number of eggs remaining
		eggsRemaining[e]--;
		eggsPlaced++; //This is technically redundant - it should always be equal to (5 - totalEggsRemaining())...

		//Stop displaying the LED pattern now that we've deposited the egg.
		stopSignalling(hal);

	}

	//Return to the start
	nav.goHome(hal);

	INFO("[MC] Mission complete.");
}


