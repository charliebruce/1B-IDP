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
		cpOccupied[i] = true;
	}

	//We start pointing EAST
	forwards = EAST;

	currentNode = NODE_0;

}

Navigation::~Navigation() {
	DEBUG("[NAV] Destructor.");
}

void Navigation::travelToCP(COLLECTION_POINT cp) {
	//TODO this
	travelToNode(NODE_0);
}

void Navigation::travelToDP(DROPOFF_POINT dp) {
	//TODO this
	travelToNode(NODE_0);
}

//Internal use only
void Navigation::travelToNode(NODE n) {

}

//Find the nearest location from which we can collect an egg, given our current location
COLLECTION_POINT Navigation::getNearestOccupiedCP(void) {

	COLLECTION_POINT best = CP_0;

	//No collection point can ever be more than 10m away
	int bestDist = 10000;

	for(COLLECTION_POINT i = CP_0; i < NUM_CP; i++) {

		//If the current collection point has no egg, move on to the next.
		if(!cpOccupied[i]) {
			TRACE("CP"<<i<<" is not occupied with egg; continuing.");
			continue;
		}

		//If the distance to the node associated with CP i is lower
		if(int bd = distanceBetweenNodes(currentNode, nodeForCP(i)) < bestDist)
		{
			//We've found a new minimum distance
			bestDist = bd;

			//That becomes the best collection point
			best = i;

			TRACE("[NAV] CP" <<i<<" is "<<bd<<"cm away which is new best.");
		} else {
			TRACE("[NAV] CP "<<i<<" is "<<bd<<"cm away which is not closer.");
		}
	}


	if(bestDist == 10000) {
		//No collection point is occupied with egg, or the free one is inaccessible
		WARN("[NAV] No occupied collection point, or inaccessible.");
	}

	DEBUG("[NAV] Found nearest CP: "<<best);
	return best;

}

//An estimate of the line-following distance between two nodes on the field in cm
int Navigation::distanceBetweenNodes(NODE a, NODE b) {
	return 100;
}

NODE nodeForCP(COLLECTION_POINT cp) {
	return NODE_0;
}


void Navigation::setUnoccupied(COLLECTION_POINT cp) {
	cpOccupied[cp] = false;
}

