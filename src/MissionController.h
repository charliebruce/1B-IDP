/*
 * MissionController.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	This represents the high-level control of the robot for the competition. It is aware of the aims
 *	and rules of the competition, but does not interface directly with the hardware.
 *
 *	Game objective:
 *	Score as many points as possible.
 *
 *	Sanity checks that could be implemented:
	 *	Check position/navigation logic regularly
	 *	Dead reckoning should not allow motion outside of arena boundaries
	 *	We should not detect more than a certain number of eggs of a specific colour (see below):
 *	There are 5 eggs:
 *	2 white
 *	2 brown
 *	1 special (multi-coloured)
 *
 */

#ifndef MISSIONCONTROLLER_H_
#define MISSIONCONTROLLER_H_


#include "Common.h"
#include "LEDs.h"
#include "HAL.h"
#include "Navigation.h"
#include "LineFollowing.h"



class MissionController {


public:
	//Constructor and destructor. Set the state of the Mission Controller to represent the initial state of the table.
	MissionController(HAL* h);
	~MissionController(void);

	//Run the competition logic
	void RunMission(MISSION objective);

	//Run functional tests
	void FunctionalTests(void);

	//Run line following tests
	void LineTests(void);

private:

	void MainMission(void);

	//How many of each type of egg remain?
	int eggsRemaining[NUM_EGGTYPES];

	//Count the total number of eggs remaining
	int totalEggsRemaining(void) {int t = 0; for(int i = 0; i<NUM_EGGTYPES;i++) {t+=eggsRemaining[i];} return t;}

	//Where should we drop off eggs of each type?
	DROPOFF_POINT dropoffForEgg[NUM_EGGTYPES];

	//Count the number of eggs we have placed
	int eggsPlaced;
};




#endif /* MISSIONCONTROLLER_H_ */
