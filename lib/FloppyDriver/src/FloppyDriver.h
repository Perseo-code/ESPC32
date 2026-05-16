#ifndef FLOPPYDRIVER
#define FLOPPYDRIVER

#include "Config.h"
/*
Wow, man, look at this! A driver for floppy disk!
Yes, I thought of it and this is what made me start doing this project.
Hope you like it and makes you buy more floppy disks. These never die!
*/
void initializeFloppyPins(); // Executes pinMode internally

void seekTrack();
void readSector();
void motorOn();

#endif