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

#include "Common.h"

//CUED libraries
#include <robot_link.h>
#include <robot_instr.h>
#include <stopwatch.h>


enum MOTOR {
	MOTOR_LEFT = 0,
	MOTOR_RIGHT,
	MOTOR_LIFT,
	NUM_MOTORS
};

enum PNEUMATIC {
	PNEU_OPEN = 0,
	PNEU_CLOSE,
	NUM_PNEUS
};

enum LED {
	LED_LEFT = 0,
	LED_MIDD,
	LED_RGHT,
	LED_CLRA,
	LED_CLRB,
	NUM_LEDS
};

enum LDR_SENSOR {
	SENSOR_0 = 0,
	NUM_SENSORS
};

struct LINE_SENSOR_DATA {
	bool fl; //Front left
	bool fc; //Front centre
	bool fr; //Front right
	bool rc; //Rear centre
};

struct SENSOR_DATA {
	int intensity;
};

class HAL {


public:

	//Create a HAL with the given robot wireless card number
	//When running directly on the robot processor, the robot number is ignored.
	HAL(int robotNum);

	//Destructor
	~HAL(void);

	//Return to an initial (known) state where nothing is moving or lit.
	void resetRobot(void);

	//Set motor m to rate r
	//Rate is in the range [-1..1] where 0 is completely stopped, and 1 is the "positive" motion (typically forwards, up)
	//Values larger than 1 or less than -1 should be clamped and a warning should be thrown if significantly out of range
	void motorSet(MOTOR m, float rate);

	//Set the state of the given LED
	void ledSet(LED l, bool on);

	//Toggle the state of the given LED
	void ledToggle(LED l);

	//Read the raw data from the colour sensing LDR
	SENSOR_DATA sensorRead(LDR_SENSOR s);

	//Read the data from the line sensors
	LINE_SENSOR_DATA lineRead(void);

	//Operate the pneumatics
	void pneumaticOperation(PNEUMATIC p, bool state);

	//Simple unit tests to ensure that sub-systems are functioning correctly
	void networkTest(void);
	void ledTest(void);
	void motorTest(void);
	void sensorTest(void);

private:

	//Used to handle reconnection and error logging.
	void handleErrors(void);

};




#endif /* HAL_H_ */
