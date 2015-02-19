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
	NODE_0 = 0,
	NODE_1,
	NODE_2,
	NUM_NODES
};

class Navigation {

public:
	Navigation();
	~Navigation();

	COLLECTION_POINT getNearestOccupiedCP(void);
	void setUnoccupied(COLLECTION_POINT cp);

	void travelToDP(DROPOFF_POINT dp);
	void travelToCP(COLLECTION_POINT cp);


private:
	//State of the collection points is also stored in here
	bool cpOccupied[NUM_CP];

	//TODO prototype this and pathfinding code
	void travelToNode(NODE n);

	//Our current position
	//Node currentNode;
	//Node nextNode;
	//int orientation
};



//TODO determine this
class Node {
	Node* neighbours[];

};


#endif /* NAVIGATION_H_ */
