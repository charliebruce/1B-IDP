/*
 * HAL.h
 *
 *  Created on: 12 Feb 2015
 *      Author: Charlie
 *
 *	Hardware Abstraction Layer for 1B IDP robot.
 *
 *	This code is responsible for interfacing with the hardware.
 */

#ifndef HAL_H_
#define HAL_H_


enum MOTOR {
	MOTOR_LEFT = 0,
	MOTOR_RIGHT,
	MOTOR_GRIP,
	MOTOR_LIFT,
};

enum LED {
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
};

enum SENSOR {
	SENSOR_0 = 0,
};

struct SENSOR_DATA {
	int r;
	int g;
	int b;
};

class HAL {


public:

	HAL(int robotNum);
	~HAL(void);

	//Return to an initial (known) state where nothing is moving or lit.
	void resetRobot(void);

	//Set motor M to rate r
	//Rate is in the range [-1..1] where 0 is completely stopped
	//Values larger than 1 or less than -1 should be clamped and a warning should be thrown if significantly out of range
	void motorSet(MOTOR m, float rate);

	//Set the state of the given LED
	void ledSet(LED l, bool on);

	//Toggle the state of the given LED
	void ledToggle(LED l);

	//Read the raw data from the colour sensor (or is it just a simple LDR?)
	SENSOR_DATA sensorRead(SENSOR s);


private:

};




#endif /* HAL_H_ */
