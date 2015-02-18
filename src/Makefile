# Automatically generated Makefile
# TODO: Improve with the following:
# -> Recipe method (not naming files)
# -> Make for both targets
# -> Optimisation level


CC=g++
CC_ARM=arm-linux-gnueabi-g++
CCFLAGS=-ansi -Wall -g -I/export/teach/1BRobot
CCFLAGS_ARM=-ansi -Wall -g -I/usr/arm-unknown-linux-gnueabi/include -I/export/teach/1BRobot 
LDFLAGS=-L/export/teach/1BRobot
LDFLAGS_ARM=-L/usr/arm-unknown-linux-gnueabi/lib
LDLIBS=-lrobot
LDLIBS_ARM=
EXECUTABLE=robot
EXECUTABLE_ARM=robot.arm

SOURCES := $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS_ARM = $(SOURCES:.cpp=.oarm)
all: robot robot_arm

robot: $OBJ
	$(CC) $(LDFLAGS) $(OBJECTS) -o robot $(LDLIBS)

robot_arm: $OBJ_ARM
	$(CC_ARM) $(LDFLAGS_ARM) $(OBJECTS_ARM) -o robot.arm $(LDLIBS_ARM)

%.o : %.cpp
	$(CC) $(CCFLAGS) $< -o $@

%.oarm : %.cpp
	$(CC_ARM) $(CCFLAGS_ARM) $< -o $@
 
clean: 
	-rm -f *.o