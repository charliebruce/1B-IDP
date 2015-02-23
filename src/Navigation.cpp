/*
 * Navigation.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */

#include "Navigation.h"

#include "Log.h"

Navigation::Navigation() {

	DEBUG("[NAV] Creator.");
	//The game state starts with all collection points occupied
	for(int i = 0; i<NUM_CP; i++) {
		cpHasEgg[i] = true;
	}

	//We start pointing EAST
	forwards = EAST;

	//At the starting location
	currentNode = NODE_START;

}

Navigation::~Navigation() {
	DEBUG("[NAV] Destructor.");
}

void Navigation::travelToCP(COLLECTION_POINT cp) {
	//TODO implement this
	travelToNode(NODE_START);
}

void Navigation::travelToDP(DROPOFF_POINT dp) {
	//TODO implement this
	travelToNode(NODE_START);
}

//Internal use only
void Navigation::travelToNode(NODE n) {

	//TODO implement this
}

//Find the nearest location from which we can collect an egg, given our current location
COLLECTION_POINT Navigation::getNearestEggyCP(void) {

	//TODO simplify:
	//if at D1, we prioritise CP2, 3, 4, 1, 0 to get the points ASAP
	//If at D2 or D3, prioritise in descending order
	//If at start, we prioritise in ascending order
	//We should not be asked this if located anywhere else! Maybe if we discard an egg or encounter another error.
	COLLECTION_POINT priority[NUM_CP];


	if(currentNode == NODE_START) {
		priority[0] = CP_0;
		priority[1] = CP_1;
		priority[2] = CP_2;
		priority[3] = CP_3;
		priority[4] = CP_4;
	}

	else if(currentNode == NODE_D1) {
		priority[0] = CP_2;
		priority[1] = CP_3;
		priority[2] = CP_4;
		priority[3] = CP_1;
		priority[4] = CP_0;
	}

	else {
		priority[0] = CP_4;
		priority[1] = CP_3;
		priority[2] = CP_2;
		priority[3] = CP_1;
		priority[4] = CP_0;
	}

	int i = 0;
	while(i < NUM_CP && !cpHasEgg[priority[i]]) {
		i++;
	}

	if(i == NUM_CP)	{
		WARN("No valid collection point found!");
		return CP_0;
	}

	DEBUG("[NAV] Found nearest CP: " << priority[i]);
	return priority[i];

}

//An estimate of the line-following distance between two nodes on the field in cm
int Navigation::distanceBetweenNodes(NODE a, NODE b) {
	return 100;
}

NODE Navigation::nodeForCP(COLLECTION_POINT cp) {
	return NODE_START;
}


void Navigation::setNoEgg(COLLECTION_POINT cp) {
	cpHasEgg[cp] = false;
}

