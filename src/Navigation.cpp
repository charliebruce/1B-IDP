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
	DEBUG("[NAV] Travelling to CP"<<cp);
	travelToNode(nodeForCP(cp));
}

void Navigation::travelToDP(DROPOFF_POINT dp) {
	DEBUG("[NAV] Travelling to DP"<<(dp + 1)); //DP_1 == 0
	travelToNode(nodeForDP(dp));
}

void Navigation::collectEgg(void) {
	WARN("[NAV] Not yet implemented egg pickup.");
	//TODO time lift time to work out weight / "shake" up and down to time?
	//TODO this
}
void Navigation::dropoffEgg(void) {
	WARN("[NAV] Not yet implemented egg dropoff.");
	//TODO this
}

//Internal use only
void Navigation::travelToNode(NODE n) {

	DEBUG("[NAV] Travelling to node " << n);

	//TODO implement this

	//Plan out the route as the optimal sequence of line followings, junction crossings and turnings











}

//Find the nearest location from which we can collect an egg, given our current location
COLLECTION_POINT Navigation::getNearestEggyCP(void) {

	//if at D1, we prioritise CP2, 3, 4, 1, 0 to get the points ASAP
	//If at D2 or D3, prioritise in descending order
	//If at start, we prioritise in ascending order
	//We should not be asked this if located anywhere else! Maybe if we discard an egg or encounter another error.
	//In that case a non-optimal pickup location is the least of our worries.
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

	else { //TODO if we're not at D2 or D3 this may not be optimal!
		priority[0] = CP_4;
		priority[1] = CP_3;
		priority[2] = CP_2;
		priority[3] = CP_1;
		priority[4] = CP_0;
	}

	int i = 0;
	while(i < NUM_CP && !cpHasEgg[priority[i]]) {
		//If the egg has been collected, move to the next-highest ranking pickup location
		i++;
	}

	if(i == NUM_CP)	{
		WARN("[NAV] No valid collection point found!");
		return CP_0;
	}

	DEBUG("[NAV] Found nearest CP: " << priority[i]);
	return priority[i];

}

//An estimate of the line-following distance between two nodes on the field in cm
int Navigation::distanceBetweenNodes(NODE a, NODE b) {
	return 100; //TODO implement
}

NODE Navigation::nodeForDP(DROPOFF_POINT dp) {
	switch(dp) {
	case DP_1:
		return NODE_START; //TODO this
	case DP_2:
		return NODE_START; //TODO this
	case DP_3:
		return NODE_START; //TODO this
	default:
		ERR("[NAV] Tried to get node for an unknown or invalid dropoff point!");
		return NODE_START;
	}
}

NODE Navigation::nodeForCP(COLLECTION_POINT cp) {
	switch(cp) {
	case CP_0:
		return NODE_START; //TODO this
	case CP_1:
		return NODE_START; //TODO this
	case CP_2:
		return NODE_START; //TODO this
	case CP_3:
		return NODE_START; //TODO this
	case CP_4:
		return NODE_START; //TODO this
	default:
		ERR("[NAV] Tried to get node for an unknown or invalid collection point!");
		return NODE_START;
	}
}

void Navigation::setNoEgg(COLLECTION_POINT cp) {
	cpHasEgg[cp] = false;
}

