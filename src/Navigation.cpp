/*
 * Navigation.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */

#include "Navigation.h"

#define LOGLEVEL LL_DEBUG

#include "Log.h"

ABS_DIRECTION flip (ABS_DIRECTION in) {

	//This should simplify to ((in + 2) % 4) provided NORTH == 0
	switch (in) {
	case NORTH:
		return SOUTH;
	case SOUTH:
		return NORTH;
	case EAST:
		return WEST;
	case WEST:
		return EAST;
	default:
		WARN("Incorrect use of flip!");
		return NORTH;
	}

}

void Navigation::addDeadend(NODEINDEX n, ABS_DIRECTION dir) {

	nodes[n].neighbours[dir] = NODE_DEADEND; //Essentially an unreachable node

}


//The link starts at FROM, runs DIRECTION to TARGET, with the length LENGTH
void Navigation::addLink(NODEINDEX from, ABS_DIRECTION dir, NODEINDEX to, int length) {

	nodes[from].neighbours[dir] = to;
	nodes[to].neighbours[flip(dir)] = from;
	nodes[from].lengths[dir] = length;
	nodes[to].lengths[flip(dir)] = length;

}

void Navigation::setForwardsDirection(ABS_DIRECTION dir) {
	forwards = dir;
}

Navigation::Navigation() {

	DEBUG("[NAV] Creator.");

	//The game state starts with all collection points occupied
	for(int i = 0; i<NUM_CP; i++) {
		cpHasEgg[i] = true;
	}

	//We start pointing EAST
	setForwardsDirection(WEST);

	//At the starting location
	currentNode = NODE_2;

	//No target initially
	targetNode = NODE_START;

	//Set up the world map - doing it this way stops redundancy


	//If it isn't connected to anything it is a deadend
	for(int i = 0; i < NODE_DEADEND; i++) {
		for(int d = 0; d < 4; d++) {
			nodes[i].neighbours[d] = NODE_DEADEND;
		}
	}

	//The upper section
	addLink(NODE_DP2DP3, EAST, NODE_1, 85);
	addLink(NODE_1, EAST, NODE_2, 85);
	//addLink(NODE_2, SOUTH, NODE_3A, 30);
	addLink(/*NODE_3A*/NODE_2, SOUTH, NODE_3, /*114*/144);
	addLink(NODE_3, SOUTH, NODE_4, 24);

	//Collection points, E-W roads
	addLink(NODE_3, WEST, NODE_CP4, 44);
	addLink(NODE_4, WEST, NODE_CP4S, 44);
	addLink(NODE_CP4S, WEST, NODE_CP3S, 20);
	addLink(NODE_CP4, WEST, NODE_CP3, 20);
	addLink(NODE_CP3S, WEST, NODE_CP2S, 20);
	addLink(NODE_CP3, WEST, NODE_CP2, 20);
	addLink(NODE_CP2S, WEST, NODE_CP1S, 20);
	addLink(NODE_CP2, WEST, NODE_CP1, 20);
	addLink(NODE_CP1S, WEST, NODE_CP0S, 20);
	addLink(NODE_CP1, WEST, NODE_CP0, 20);

	//Collection points, N-S roads
	addLink(NODE_CP0, SOUTH, NODE_CP0S, 24);
	addLink(NODE_CP1, SOUTH, NODE_CP1S, 24);
	addLink(NODE_CP2, SOUTH, NODE_CP2S, 24);
	addLink(NODE_CP3, SOUTH, NODE_CP3S, 24);
	addLink(NODE_CP4, SOUTH, NODE_CP4S, 24);

	//The start, and dropoff loop for D1
	addLink(NODE_START, EAST, NODE_7, 19);
	addLink(NODE_7, EAST, NODE_CP0, 26);
	addLink(NODE_START, WEST, NODE_8, 19);
	addLink(NODE_START, SOUTH, NODE_6, 14);
	addLink(NODE_6, SOUTH, NODE_5, 9999);	//Distance uncertain
	addLink(NODE_5, EAST, NODE_CP0S, 44);
	addLink(NODE_START, NORTH, NODE_9, 14);
	addLink(NODE_9, NORTH, NODE_10, 46);
	addLink(NODE_10, EAST, NODE_DP1, 85);
	addLink(NODE_DP1, SOUTH, NODE_CP2, 62);	//Distance uncertain

	//Mark T junctions
	nodes[NODE_CP0].tJunction = true;
	nodes[NODE_CP0].tOrientation = NORTH;
	nodes[NODE_CP1].tJunction = true;
	nodes[NODE_CP1].tOrientation = NORTH;
	nodes[NODE_CP3].tJunction = true;
	nodes[NODE_CP3].tOrientation = NORTH;
	nodes[NODE_CP4].tJunction = true;
	nodes[NODE_CP4].tOrientation = NORTH;

	//TODO finish list of dead ends

}

Navigation::~Navigation() {
	DEBUG("[NAV] Destructor.");
}

void Navigation::goHome(HAL* hal) {
	DEBUG("[NAV] Returning home...");
	calculateRouteToNode(NODE_START);
	travelRoute(hal);
	DEBUG("[NAV] Orienting in the E-W direction...");
	if((forwards == EAST) || (forwards == WEST)) {
		INFO("[NAV] Made it home - no need to turn.");
		return;
	}
	junctionTurn(false, hal); //False: Clockwise (right) turn
	if(forwards == NORTH)
		forwards = EAST; //Clockwise turn from NORTH
	else
		forwards = WEST; //Clockwise turn from SOUTH	
	
	INFO("[NAV] Home after turning!");
}

void Navigation::travelToCP(COLLECTION_POINT cp, HAL* h) {
	DEBUG("[NAV] Travelling to "<<cp);
	calculateRouteToNode(nodeForCP(cp));
	travelRoute(h);
}

void Navigation::travelToDP(DROPOFF_POINT dp, HAL* h) {
	DEBUG("[NAV] Travelling to "<<dp);
	calculateRouteToNode(nodeForDP(dp));
	travelRoute(h);
}

void Navigation::travelToStart(HAL* h) {
	DEBUG("[NAV] Returning home.");
	calculateRouteToNode(NODE_START);
	travelRoute(h);
}

void Navigation::collectEgg(COLLECTION_POINT cp, HAL* h) {
	WARN("[NAV] Not yet implemented egg pickup.");
	//TODO time lift time to work out weight / "shake" up and down to time?
	//TODO this


	//Orient ourselves at the CP ie face south

	if(forwards == EAST) {
		//Rotate Clockwise (ie right)
		junctionTurn(false, h);
	}
	if(forwards == WEST) {
		//Rotate Anticlockwise (ie left)
		junctionTurn(true, h);
	}
	if(forwards == NORTH) {
		//WTF?
		ERR("We should never be collecting but facing NORTH!");
		return;
	}
	//We will now be facing south, to the CP.
	forwards = SOUTH;

	//We're actually very lucky: if we are just behind CPxS, our claw lines up.

	//Approach and hit CPxS
	followLineToNext(24, false, false, h);

	//We're now at CPxS - reverse slowly until we just see 2 black.
	//In case we overshot the junction
	reverseToJunction(h);
	reverseJustBeyondJunction(h);

	//Line up straight (ie centre ourselves on the line)
	//Probably not necessary if wide jaws: centreOnLine(h);

	//Operate claw
	h->pneumaticOperation(PNEU_CLAW, CLAW_CLOSED);

	//Wait for pneumatic action.
	delay(3000);

	//Verify a good grab
	if(!h->switchRead(SWITCH_EGG)) {
		ERR("Failed to grab egg! Cannot continue.");
		return;
	}


	//Operate lift
	h->motorSet(MOTOR_LIFT, 1.0);

	while(!h->switchRead(SWITCH_LIMIT_UP))
		;

	h->motorSet(MOTOR_LIFT, 0.0);


	//Pivot

	//Find node
}

void Navigation::dropoffEgg(DROPOFF_POINT dp, HAL* h) {
	WARN("[NAV] Not yet implemented egg dropoff.");
	//TODO this

	//Orient

	//Approach

	//Operate lift

	//Operate claw
	h->pneumaticOperation(PNEU_CLAW, CLAW_OPEN);

	//Pivot

	//Find node
}

static const int HIGHWEIGHT = 100000;

bool Navigation::notFinishedWeighting(void) {
	for(int i = 0; i < NODE_DEADEND; i++) {
		if (nodes[i].weight == HIGHWEIGHT)
			return true;
	}
	return false;
}

//Return the distance from current location to the node
int Navigation::calculateRouteToNode(NODEINDEX givenTarget) {

	DEBUG("[NAV] Calculating route to node " << givenTarget);

	targetNode = givenTarget;

	//Assign weight of 100000 to all nodes including the DEADEND node
	for(int i = 0; i< NUM_NODES; i++) {
		nodes[i].weight = HIGHWEIGHT;
	}

	//Start at the target node, assign a weight of 0
	nodes[givenTarget].weight = 0;

	//Now loop until no node is at -1 (unreachable nodes will cause a loop until the failsafe catches it)
	int runs = 0;
	while(notFinishedWeighting()) {

		runs++;
		TRACE("[NAV] Weighting: Run " << runs);

		//For each of the nodes except dead ends
		for(int i = 0; i < NODE_DEADEND; i++) {

			//For each of the directions
			for(int dir = 0; dir < 4; dir++) {

				//Consider the neighbour in each direction
				NODEINDEX neighbour = nodes[i].neighbours[dir];

				TRACE("[NAV] Considering "<<i<<"'s neighbour in the "<<dir <<" direction. Has weight "<<nodes[neighbour].weight);

				//If the neighbour in that direction is reachable (weight realistix)
				if(nodes[neighbour].weight < HIGHWEIGHT) {

					TRACE("[NAV] That neighbour is weighted!");
					//The total weight through that neighbour is (neighbour weight + interconnecting line length)
					int neighbourRouteWeight = nodes[neighbour].weight + nodes[i].lengths[dir];

					//The weight of the current node should be min ( ( current weight ) , ( weight of neighbouring node + distance between them ) )
					if(neighbourRouteWeight < nodes[i].weight)
						nodes[i].weight = neighbourRouteWeight;
				}

			}
			//This algorithm isn't very fast but for a small map (around 30 nodes) we don't care, we have the power - we're not an 8-bit microcontroller.
		}

		//This should typically reach a conclusion in under 20 loops - guards against un-linked nodes (we won't spin forever)
		if (runs > 25) {
			WARN("[NAV] Possibly unreachable node in navigation mesh!");

			WARN("[NAV] Not yet weighted: ");
			for(int i = 0; i< NODE_DEADEND; i++) {
				if(nodes[i].weight == HIGHWEIGHT)
					WARN("Node " << i);
			}

			return nodes[currentNode].weight;
		}
	}


	//NOTE: This algorithm does not take in to account the fact that changing direction has a cost associated with it.

	DEBUG("[NAV] Found weights, distance is " << nodes[currentNode].weight);
	return nodes[currentNode].weight;

}

//Go down the "weight gradient" to the target node
void Navigation::travelRoute(HAL* h) {

	TRACE("[NAV] I walk this lonely road...");

	//While we haven't arrived
	int juncs = 0;	
	while(currentNode != targetNode) {

		juncs++;

		DEBUG("[NAV] Not there yet: at "<<currentNode);

		NODEINDEX next;
		ABS_DIRECTION nextdir;

		int lowest = 100000;
		//Work out which neighbour to head towards (lowest numbered neighbour)
		for(int dir = 0; dir < 4; dir++) {
			NODEINDEX n = nodes[currentNode].neighbours[dir];
			if (nodes[n].weight < lowest)
			{
				next = n;
				nextdir = (ABS_DIRECTION) dir;
				lowest = nodes[n].weight;
				TRACE("[NAV] "<<n <<" has lower weight of "<<nodes[n].weight<<"; a better candidate.");
			}
		}

		DEBUG("[NAV] Next node will be " << next);

		//U-Turns implemented in a special case (we are turning on the spot)
		bool uTurned = false;	
		if(nextdir == flip(forwards)) {
			DEBUG("[NAV] U Turn.");
			uTurn(h);
			DEBUG("[NAV] U Turned.");
			forwards = flip(forwards);
			uTurned = true;
		}


		//If we need to change orientation, do so
		if(forwards != nextdir) {

			//Work out if we want to perform a left or a right turn. This assumes that the orientations are defined clockwise as seen from above looking down
			bool left = true;
			if (((forwards + 1) % 4) == nextdir){
				left = false;
			} else {
				left = true;
			}

			DEBUG("[NAV] Current orientation is "<<forwards<<" but we need " << nextdir << " - we need to rotate "<<(left?"left":"right"));

			//Perform the rotation and update the orientation
			junctionTurn(left, h);
			forwards = nextdir;

			//Are we approaching a T junction from the side?
			bool approachingTSide = false;
			if(nodes[next].tJunction && (forwards != flip(nodes[next].tOrientation)))
				approachingTSide = true;

			if(approachingTSide)
				TRACE("[NAV] We are approaching a T junction from the side!");

			//Follow the line
			followLineToNext(nodes[currentNode].lengths[nextdir], false, approachingTSide, h);

		} else {
			if(!uTurned)
				junctionStraight(h);

			//Are we approaching a T junction from the side?
			bool approachingTSide = false;
			if(nodes[next].tJunction && (forwards != flip(nodes[next].tOrientation)))
				approachingTSide = true;

			if(approachingTSide)
				TRACE("[NAV] We are approaching a T junction from the side!");

			//Follow the line to the next node
			followLineToNext(nodes[currentNode].lengths[nextdir], true, approachingTSide, h);

		}

		//Update the current node
		currentNode = next;

	}

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

	else if(currentNode == NODE_DP1) { //If at DP1 we're almost certain to need to take it up the ramp (assuming we can't reach from below!!!!)
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

NODEINDEX Navigation::nodeForDP(DROPOFF_POINT dp) {
	switch(dp) {
	case DP_1:
		return NODE_DP1;
	case DP_2:
		return NODE_DP2DP3;
	case DP_3:
		return NODE_DP2DP3;
	default:
		ERR("[NAV] Tried to get node for an unknown or invalid dropoff point!");
		return NODE_START;
	}
}

NODEINDEX Navigation::nodeForCP(COLLECTION_POINT cp) {
	switch(cp) {
	case CP_0:
		return NODE_CP0;
	case CP_1:
		return NODE_CP1;
	case CP_2:
		return NODE_CP2;
	case CP_3:
		return NODE_CP3;
	case CP_4:
		return NODE_CP4;
	default:
		ERR("[NAV] Tried to get node for an unknown or invalid collection point!");
		return NODE_START;
	}
}

void Navigation::setNoEgg(COLLECTION_POINT cp) {
	cpHasEgg[cp] = false;
}

