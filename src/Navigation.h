/*
 * Navigation.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	This class is responsible for monitoring the robot's position, and planning and executing movements and rotations.
 *	This class will communicate with the line-following logic and the high-level controller.
 */

#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include "ObjectRecognition.h"

#include "Common.h"

//Nodes are internal to the navigation class
//TODO complete the node map
enum NODE {
	NODE_START = 0,
	NODE_1,
	NODE_2,
	NODE_D1,
	NUM_NODES
};

//Direction reference for orientation
//We arbitrarily define the raised section to be in the NORTH (ramp running NORTH-SOUTH)
enum ABS_DIRECTION {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
};

class Navigation {

public:

	//Constructor and destructor - initialise the variables to the state at the start of the mission
	Navigation();
	~Navigation();

	//Return the optimal collection point (has egg, is near)
	COLLECTION_POINT getNearestEggyCP(void);

	//We have removed the egg from the given collection point: mark it empty.
	void setNoEgg(COLLECTION_POINT cp);

	//Travel to a given collection or dropoff point
	void travelToDP(DROPOFF_POINT dp);
	void travelToCP(COLLECTION_POINT cp);

	//Pick up and deposit the egg - also operates the lift, claw
	void collectEgg(void);
	void dropoffEgg(void);

private:

	//State of the collection points is stored in here
	bool cpHasEgg[NUM_CP];

	//TODO prototype this and pathfinding code
	void travelToNode(NODE n);

	//Work out which junction we are aiming to navigate to.
	NODE nodeForCP(COLLECTION_POINT cp);
	NODE nodeForDP(DROPOFF_POINT dp);

	//Our current position and approximate orientation
	NODE currentNode;
	ABS_DIRECTION forwards;

	int distanceBetweenNodes(NODE a, NODE b);

};

//Each node has up to 4 neighbours, in each of the 4 cardinal directions
//A neighbour could be a node, a dead end, blank (ie no path) or a T-node
//There is also an estimated distance associated with each direction

//TODO Represent the world in this way, write navigation code which uses this knowledge
class Node {
public:
	Node* neighbours[4];
	int lengths[4];
	bool tJunction;
	ABS_DIRECTION tOrientation;
};


#endif /* NAVIGATION_H_ */
