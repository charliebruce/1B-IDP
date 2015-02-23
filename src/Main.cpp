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
	bool doFunctionalTests = false;
	bool doMainMission = true;

	if(argc) {
		INFO("Run with "<< argc << " arguments:");
		for(int i = 0; i < argc; i++) {
			INFO(i<<" is \"" << argv[i] <<"\".");
		}

		//Program name is usually argv[0], so argv[1] is the first parameter.
		if(argc > 2) {
			WARN("Too many parameters; only the first will be considered.");
		}

		//TODO add calibration mode, save calibration parameters to file

		if(!strcmp(argv[1], "functional-tests")) {
			doFunctionalTests = true;
			doMainMission = false;
		}
		else if(!strcmp(argv[1], "run-mission")) {
			doFunctionalTests = false;
			doMainMission = true;
		}
		else if(!strcmp(argv[1], "run-all")) {
			doFunctionalTests = true;
			doMainMission = true;
		}
		else {
			WARN("No valid parameters supplied; proceeding with main mission.");
		}

	}

	HAL hal(ROBOT_NUM);

	MissionController mc(&hal);

	if(doFunctionalTests)
		mc.FunctionalTests();

	if(doMainMission)
		mc.RunMission();


	return 0;
}


