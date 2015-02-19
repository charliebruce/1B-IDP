/*
 * Navigation.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */

#include "Navigation.h"



Navigation::Navigation() {

	//The game state starts with all collection points occupied
	for(int i = 0; i<NUM_CP; i++) {
		cpOccupied[i] = true;
	}


}

Navigation::~Navigation() {

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
	//TODO Implement this
	return CP_0;
}

void Navigation::setUnoccupied(COLLECTION_POINT cp) {
	cpOccupied[cp] = false;
}

