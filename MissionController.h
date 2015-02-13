/*
 * MissionController.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	This represents the high-level control of the robot for the competition. It is aware of the aims and rules of the competition, but does not interface
 *	directly with the hardware.
 *
 *	Game objective:
 *	Score as many points as possible.
 *
 *	Sanity checks that could be implemented:
 *	Check position/navigation logic regularly
 *	Dead reckoning should not allow motion outside of arena boundaries
 *
 *	There are 5 eggs:
 *	2 white
 *	2 brown
 *	1 special (multi-coloured)
 *
 */

#ifndef MISSIONCONTROLLER_H_
#define MISSIONCONTROLLER_H_

class MissionController {

public:
	MissionController();
	~MissionController();



private:

};




#endif /* MISSIONCONTROLLER_H_ */
