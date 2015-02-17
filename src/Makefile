# Automatically generated Makefile
Main :  MissionController.o Main.o LEDs.o HAL.o
	 g++ -L/export/teach/1BRobot -o Main  MissionController.o  Main.o  LEDs.o  HAL.o  -lrobot

MissionController.o: MissionController.cpp MissionController.h Log.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c MissionController.cpp
Main.o: Main.cpp Common.h HAL.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c Main.cpp
LEDs.o: LEDs.cpp LEDs.h Log.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c LEDs.cpp
HAL.o: HAL.cpp HAL.h Log.h
	 g++ -ansi -Wall -g -I/export/teach/1BRobot -c HAL.cpp
