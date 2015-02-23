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

enum COLLECTION_POINT {
	CP_0 = 0,
	CP_1,
	CP_2,
	CP_3,
	CP_4,
	NUM_CP
};

enum DROPOFF_POINT {
	DP_1 = 0,
	DP_2,
	DP_3,
	DP_ANYWHERE,
	NUM_DP
};

enum NODE {
	NODE_START = 0,
	NODE_1,
	NODE_2,
	NODE_D1,//TODO this better
	NUM_NODES
};

enum ABS_DIRECTION {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST
};



//Each node has up to 4 neighbours, in each of the 4 cardinal directions
//A neighbour could be a node, a dead end, blank (ie no path) or a T-node
//There is also an estimated distance associated with each direction

//If approaching a T-node from one of its sides, follow the edge not the centre of the line
//T-nodes either have an orientation, or are a node with NO_LINE attached in one direction

class Navigation {

public:
	Navigation();
	~Navigation();

	COLLECTION_POINT getNearestEggyCP(void);
	void setNoEgg(COLLECTION_POINT cp);

	void travelToDP(DROPOFF_POINT dp);
	void travelToCP(COLLECTION_POINT cp);


private:

	//State of the collection points is also stored in here
	bool cpHasEgg[NUM_CP];

	//TODO prototype this and pathfinding code
	void travelToNode(NODE n);

	int distanceBetweenNodes(NODE a, NODE b);
	NODE nodeForCP(COLLECTION_POINT cp);
	NODE nodeForDP(DROPOFF_POINT dp);



	//Our current position and approximate orientation
	NODE currentNode;
	ABS_DIRECTION forwards;

};



//TODO determine this
class Node {
	Node* neighbours[];

};


#endif /* NAVIGATION_H_ */
