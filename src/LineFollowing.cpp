/*
 * LineFollowing.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */



//Follow the line we are currently on until we reach WWWW (indicating that our front sensors are on a node, pointing straight)
//Estimated Distance is to ensure that we don't massively overshoot - stops us driving off to infinity.
void followToNextNode(int estimatedDistance) {

}




void followEdgeToNext(bool left, int estimatedDistance) {

	//TODO stop on the node rather than just following an edge.
	//TODO implement left-edge-following

	//Ignore the centre-wheel sensor

	//Continue while pattern is XWB
	//Turn left when pattern is XBB
	//Turn right when pattern is XWW

	//Stop when X = W? This will assume sensors are wide enough
	//but will allow us to stop on nodes and Ts

	//That is the only difference between Edge and Line following

	//TODO PID control to prevent/smooth out jaggy motion

}
