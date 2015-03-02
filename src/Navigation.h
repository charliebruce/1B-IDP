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
enum NODEINDEX {

	NODE_START = 0,
	NODE_DP1,
	NODE_DP2DP3,

	NODE_CP0,
	NODE_CP1,
	NODE_CP2,
	NODE_CP3,
	NODE_CP4,

	NODE_CP0S,
	NODE_CP1S,
	NODE_CP2S,
	NODE_CP3S,
	NODE_CP4S,

	NODE_1,
	NODE_2,
	NODE_3,
	NODE_4,
	NODE_5,
	NODE_6,
	NODE_7,
	NODE_8,
	NODE_9,
	NODE_10,
	NODE_11,

	NUM_NODES
};

//Struct for storing data

//Each node has up to 4 neighbours, in each of the 4 cardinal directions
//A neighbour could be a node, a dead end, blank (ie no path) or a T-node
//There is also an estimated distance associated with each direction
struct Node {
	//Which node index is our neighbour in each of the given directions
	NODEINDEX neighbours[4];
	//Length, in cm, of the line in the given direction
	int lengths[4];
	//Is the node a T-junction?
	bool tJunction;
	//The side of the node that is missing the line
	ABS_DIRECTION tOrientation;
	//Calculated distance from the target node
	int weight;
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
	void travelToDP(DROPOFF_POINT dp, HAL* h);
	void travelToCP(COLLECTION_POINT cp, HAL* h);
	void travelToStart(HAL* h);

	//Pick up and deposit the egg - also operates the lift, claw
	void collectEgg(void);
	void dropoffEgg(void);

	//Calculate the distance (weight) map and return the distance from our current location
	int calculateRouteToNode(NODEINDEX n);

	//Actually make the journey
	void travelRoute(HAL* h);

private:

	//State of the collection points is stored in here
	bool cpHasEgg[NUM_CP];

	//TODO prototype this and pathfinding code


	//Work out which junction we are aiming to navigate to.
	NODEINDEX nodeForCP(COLLECTION_POINT cp);
	NODEINDEX nodeForDP(DROPOFF_POINT dp);

	//Our current position and approximate orientation
	NODEINDEX currentNode;
	ABS_DIRECTION forwards;

	//The node that we have calculated our weights based on
	NODEINDEX targetNode;

	void addLink(NODEINDEX from, ABS_DIRECTION dir, NODEINDEX to, int length);
	bool notFinishedWeighting(void);
	Node nodes[NUM_NODES];

};


#endif /* NAVIGATION_H_ */
