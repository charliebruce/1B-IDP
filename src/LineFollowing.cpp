/*
 * LineFollowing.cpp
 *
 *  Created on: 19 Feb 2015
 *      Author: Charlie
 */



//Follow the line we are currently on until we reach WWWW (indicating that our front sensors are on a node, pointing straight)
//Estimated Distance is to ensure that we don't massively overshoot - stops us driving off to infinity.
void followToNext(int estimatedDistance, HAL* h) {

}




void followEdgeToNext(bool left, int estimatedDistance, HAL* h) {

	//Use a primitive estimate from the average velocity of the robot
	stopwatch watchdog;
	watchdog.start();

	const int velocityReciprocal = 100; //in millieconds per centimeter
	while(watchdog.read() < (250 + 1.2 * (estimatedDistance * velocityReciprocal))) { //compensate for 250ms of motor lag, max 20% overshoot

		//TODO stop on the node rather than just following an edge.
		//TODO implement left-edge-following

		//Ignore the centre-wheel sensor for now

		//Continue while pattern is XWB
		//Turn left when pattern is XBB
		//Turn right when pattern is XWW




		//Stop when X = W? This will assume sensors are wide enough
		//but will allow us to stop on nodes and Ts
		//That is the only difference between Edge and Line following

		//TODO PID control to prevent/smooth out jaggy motion

	}

	watchdog.stop();

}
