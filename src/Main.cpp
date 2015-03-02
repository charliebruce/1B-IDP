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

#include <cstring>

int main(int argc,  char** argv) {

	//If no arguments are specified, run the main mission only.
	MISSION objective = MAIN_MISSION;

	if(argc) {
		TRACE("Run with "<< argc << " arguments:");
		for(int i = 0; i < argc; i++) {
			TRACE(i<<" is \"" << argv[i] <<"\".");
		}

		//Program name is usually argv[0], so argv[1] is the first parameter.
		if(argc > 2) {
			WARN("Too many parameters; only the first will be considered.");
		}

		//TODO add calibration mode, save calibration parameters to file

		if(argc > 1) {
			if(!strcmp(argv[1], "functional-tests")) {

			}
			else if(!strcmp(argv[1], "run-mission")) {
				objective = MAIN_MISSION;
			}
			else if(!strcmp(argv[1], "run-all")) {
				objective = ALL_MISSIONS;
			}
			else if(!strcmp(argv[1], "run-week1")) {
				objective = WEEK1_TESTS;
			}
			else if(!strcmp(argv[1], "run-functional-demo1")) {
				objective = FUNCTIONAL_DEMO_1;
			}
			else if(!strcmp(argv[1], "run-functional-demo2")) {
				objective = FUNCTIONAL_DEMO_2;
			}
			else if(!strcmp(argv[1], "run-functional-demo3")) {
				objective = FUNCTIONAL_DEMO_3;
			}

			else {
				WARN("Invalid parameters supplied; proceeding with main mission.");
			}
		}
		else {
			DEBUG("No parameters supplied; proceeding with main mission.");
		}

	}

	HAL hal(ROBOT_NUM);

	MissionController mc(&hal);

	mc.RunMission(objective);


	return 0;
}


