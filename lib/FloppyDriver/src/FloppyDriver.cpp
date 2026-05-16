#include "FloppyDriver.h"

/*
Here, all the logic for floppy disks driver functions reside.
Better take notes, would ya?
*/

void initializeFloppyPins() {
    pinMode(FLOPPY_MOTOR_ON, OUTPUT);
    pinMode(FLOPPY_DRIVE_SEL, OUTPUT);
    pinMode(FLOPPY_DIRECTION, OUTPUT);
    pinMode(FLOPPY_STEP, OUTPUT);
    pinMode(FLOPPY_TRACK00, INPUT_PULLUP);
    pinMode(FLOPPY_READ_DATA, INPUT_PULLUP);
    pinMode(FLOPPY_INDEX, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOPPY_READ_DATA), readDataAction, FALLING);
    attachInterrupt(digitalPinToInterrupt(FLOPPY_INDEX), IndexAction, FALLING);
}

void IRAM_ATTR readDataAction() {
    uint32_t CurrentTime = micros(); // We get the current runtime
    uint32_t interval = CurrentTime - lastTime; // We subtract the current runtime with the last time that an interruption happened
    cb.buffer[cb.writeIndex] = interval; // We put the interval in the array
    if (cb.writeIndex == BLIMIT - 1) { // If the write index surpasses the limit, we go back to 0
        cb.writeIndex = 0;
    } else { // If not, we just add one to the write index
        cb.writeIndex++;
    }
    lastTime = CurrentTime; // And finally, change the last time with the current time
}

void IRAM_ATTR IndexAction() {
    uint32_t CurrentTime = micros();
    rotationTime = CurrentTime - lastRotationTime;
    lastRotationTime = CurrentTime;
    rotations++;
}


void motorOn() {
    digitalWrite(FLOPPY_MOTOR_ON, LOW); // Low in this floppy drive means High
    digitalWrite(FLOPPY_DRIVE_SEL, LOW);
    rotations = 0;
    delay(500);
}

void calibrate() {
    motorOn();
    digitalWrite(FLOPPY_DIRECTION, HIGH); // Floppy disk head on sector 0
    while (digitalRead(FLOPPY_TRACK00) == HIGH) {
        digitalWrite(FLOPPY_STEP, LOW);
        delayMicroseconds(10);
        digitalWrite(FLOPPY_STEP, HIGH);
        delay(10);
    }

    currentTrack = 0;
}

void seekTrack(int targetTrack) {
    if (targetTrack < 0 || targetTrack > 79) return;

    if (targetTrack > currentTrack) digitalWrite(FLOPPY_DIRECTION, LOW); // Go in (To the track)
    else digitalWrite(FLOPPY_DIRECTION, HIGH); // Or go out.
    int steps = abs(targetTrack - (int)currentTrack);

    for (int i = 0; i < steps; i++) {
        digitalWrite(FLOPPY_STEP, LOW);
        delayMicroseconds(10);
        digitalWrite(FLOPPY_STEP, HIGH);
        delay(6);
    }
    currentTrack = targetTrack;
}


static const uint32_t timeout = 5000;

void readSector(int sector, uint8_t* outputBuffer) {
    rotations = 0;
    uint32_t time = millis();
    while (rotations == 0) {
        if (time >= timeout) return; // Timeout
    }

    cb.readIndex = cb.writeIndex;
    // Find data in a maximum of 2 rotations
    while (rotations < 3) {
        
        // New pulses in the buffer waiting to be processed?
        if (cb.readIndex != cb.writeIndex) {
            
            // Read interval time
            uint16_t interval = cb.buffer[cb.readIndex];
            
            // Advance readIndex using a fast bit mask
            cb.readIndex = (cb.readIndex + 1) & (BLIMIT - 1);
            
            // MFM decoding
            // Here we process the interval to convert it into bits
            
        }
}
}