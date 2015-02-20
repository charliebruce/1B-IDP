# Makefile for IDP, group L206
# Supports building for robot or workstation
# To build for workstation:  "make robot"
# To build for robot: "make robot_arm"
# To build both: "make all"

# TODO:
# -> Optimisation level

CC=g++
CC_ARM=arm-linux-gnueabi-g++
CCFLAGS=-ansi -Wall -g -I/export/teach/1BRobot
CCFLAGS_ARM=-ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot 
LDFLAGS=-L/export/teach/1BRobot
LDFLAGS_ARM=-L/usr/arm-unknown-linux-gnueabi/lib
LDLIBS=-lrobot
LDLIBS_ARM=-lrobot
EXECUTABLE=robot
EXECUTABLE_ARM=robot.arm

SOURCES := $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_ARM = $(SOURCES:.cpp=.oarm)
all: robot robot_arm

robot: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o robot $(LDLIBS)

robot_arm: $(OBJECTS_ARM)
	$(CC_ARM) $(LDFLAGS_ARM) -o robot.arm $(OBJECTS_ARM) $(LDLIBS_ARM)

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

%.oarm : %.cpp
	$(CC_ARM) $(CCFLAGS_ARM) -c $< -o $@
 
clean: 
	-rm -f *.o *.oarm robot.arm robot