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
	bool doLFTests = false;

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

		if(argc > 1) {
			if(!strcmp(argv[1], "functional-tests")) {
				doFunctionalTests = true;
				doMainMission = false;
				doLFTests = false;
			}
			else if(!strcmp(argv[1], "run-mission")) {
				doFunctionalTests = false;
				doMainMission = true;
				doLFTests = false;
			}
			else if(!strcmp(argv[1], "run-all")) {
				doFunctionalTests = true;
				doMainMission = true;
				doLFTests = true;
			}
			else if(!strcmp(argv[1], "run-lf")) {
				doFunctionalTests = false;
				doMainMission = false;
				doLFTests = true;
			}

			else {
				WARN("Invalid parameters supplied; proceeding with main mission.");
			}
		}
		else {
			INFO("No parameters supplied; proceeding with main mission.");
		}

	}

	HAL hal(ROBOT_NUM);

	MissionController mc(&hal);

	if(doFunctionalTests)
		mc.FunctionalTests();

	if(doLFTests)
		mc.LineTests();

	if(doMainMission)
		mc.RunMission();


	return 0;
}


