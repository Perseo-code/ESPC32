#ifndef FLOPPYDRIVER
#define FLOPPYDRIVER

#include "Config.h"
/*
Wow, man, look at this! A driver for floppy disk!
Yes, I thought of it and this is what made me start doing this project.
Hope you like it and makes you buy more floppy disks. These never die!
*/
#define BLIMIT 1024
struct CBuffer {
    volatile int writeIndex = 0;
    volatile int readIndex = 0;
    volatile uint16_t buffer[BLIMIT];
};

static CBuffer cb;

volatile uint32_t lastTime;
volatile uint32_t rotationTime;
volatile uint32_t lastRotationTime;
volatile int rotations = 0;
volatile uint32_t currentTrack;
void initializeFloppyPins(); // Executes pinMode internally

void seekTrack(int targetTrack); // Changes tracks
void readSector(int sector, uint8_t* outputBuffer); // Reads sectors (Each one is 512 bytes)
void motorOn(); // Starts the drive motor
void calibrate(); // Changes to track 0

#endif