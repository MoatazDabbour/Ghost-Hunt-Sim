# COMP2401 Final Project

Written by Moataz Dabbour

## About:

- This program simulates a ghost hunt by placing a number of hunters in a virtual building with a ghost,
	and have them attempt to identify the ghost.

- This program attemps a simple visualization of the 'map' anytime a hunter exits for any reason.
- The user may want to change the time of "USLEEP_TIME" for a faster, or slower sim.
- The user may want to increase the "MAX_HUNTERS" variable to have more hunters on the job.
- The user may want to un-comment anything after "// VERBOSE" for a more detailed account of what has happened.

please refer to the attached pdf '~/specs.pdf' for more detailed information about the project.

## Contains:

- ReadMe.txt
- main.c
- building.c
- ops.c
- init.c
- defs.h
- specs.pdf
- Makefile

## Compiling & Executing:

- Launch terminal in the unpacked directory, or 
- Navigate to the unpacked directory 

TYPE:
make all
make clean

TYPE: 
./sim


## NOTE:
- This program can only be compiled and run on a linux/unix system, it utilizes libraries that are unavailable for Windows machines.
- While the hunters have ID'd the ghost in test runs, it is surprisingly rare.


Thank you!
