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

COLLECTION_POINT Navigation::getNearestOccupiedCP(void) {
	//TODO Implement this
	return CP_0;
}

void Navigation::setUnoccupied(COLLECTION_POINT cp) {
	cpOccupied[cp] = false;
}

