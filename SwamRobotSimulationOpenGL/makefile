###############################################################
# Program:
#     Project Swarm Robot Simulation
#     Brother Grimmett, CS461
# Author:
#     Jace Walton
# Summary:
#     
#     
#
#     
#     Estimated: ????? hrs
#     Actual:     hrs
#     
###############################################################

###############################################################
# Programs:
#   
#    Ganglion:  This is the ganglion executable  
###############################################################

Ganglion : uiDraw.o uiInteract.o point.o neuron.o ganglion.o velocity.o
	g++ -o Ganglion uiDraw.o uiInteract.o point.o neuron.o ganglion.o velocity.o -lglut -lGLU

#	tar -cf Ganglion.tar *.h *.cpp makefile

###############################################################
# Individual files
#    uiDraw.o      Draw polygons on the screen and do all OpenGL graphics
#    uiInteract.o  Handles input events
#    point.o       The position on the screen
#    neuron.o      The class for an individual swarm agent
#    velocity.o    The velocity vectors
#    ganglion.o    The ganglion is the hive mind of the swarm
###############################################################
uiInteract.o : uiInteract.cpp uiInteract.h point.h
	g++ -c  uiInteract.cpp

uiDraw.o : uiDraw.cpp uiDraw.h point.h
	g++ -c  uiDraw.cpp

point.o : point.cpp point.h
	g++ -c  point.cpp

neuron.o : neuron.cpp neuron.h velocity.h point.h uiInteract.h uiDraw.h
	g++ -c  neuron.cpp

velocity.o : velocity.cpp velocity.h
	g++ -c  velocity.cpp

ganglion.o : ganglion.cpp ganglion.h velocity.h point.h uiInteract.h
	g++ -c  ganglion.cpp
###############################################################
# General rules
###############################################################
clean :
	rm Ganglion *.o *.tar *.compile

all : Ganglion

