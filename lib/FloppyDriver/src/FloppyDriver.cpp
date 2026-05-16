#include "FloppyDriver.h"

/*
Here, all the logic for floppy disks driver functions reside.
Better take notes, would ya?
*/
int index = 0;

void initalizeFloppyPins() {
    pinMode(FLOPPY_MOTOR_ON, OUTPUT);
    pinMode(FLOPPY_DRIVE_SEL, OUTPUT);
    pinMode(FLOPPY_DIRECTION, OUTPUT);
    pinMode(FLOPPY_STEP, OUTPUT);
    pinMode(FLOPPY_TRACK00, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOPPY_READ_DATA), readDataAction, FALLING);
    attachInterrupt(digitalPinToInterrupt(FLOPPY_INDEX), IndexAction, FALLING);
}

void IRAM_ATTR readDataAction() {
    
}

void IRAM_ATTR IndexAction() {

}
