#include "FloppyDriver.h"

/*
Here, all the logic for floppy disks driver functions reside.
Better take notes, would ya?
*/

void initializeFloppyPins() { // We just initialize all pins
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
        // We move the head until we reached the desired track
        digitalWrite(FLOPPY_STEP, LOW);
        delayMicroseconds(10);
        digitalWrite(FLOPPY_STEP, HIGH);
        delay(6); // Needs a small delay
    }
    currentTrack = targetTrack;
}



static const uint32_t timeout = 5000;
int state = 0;
int bytesRead = 0;
int foundSector = -1;
void processByte(uint8_t currentByte, int targetSector, uint8_t* outputBuffer) {
    switch (state) {
        
        case 0:
            if (currentByte == 0xA1) { 
                state = 1;       // Found the mark.
                bytesRead = 0;  // Reset the byte counter for the head
            }
            break;

        case 1:
            // The standard header records: [Track, Side, Sector Number, Size]
            if (bytesRead == 2) { // The third byte of the header is the sector number
                foundSector = currentByte;
            }
            
            bytesRead++;
            
            if (bytesRead == 4) { // The header has already passed (4 bytes)
                if (foundSector == targetSector) {
                    state = 2;       // It's the sector we were looking for. Now saving data
                    bytesRead = 0;  // Reset the counter to fill all 512 bytes
                } else {
                    state = 0;       // It wasn't the sector we wanted. Now we'll look for the next 0xA1
                }
            }
            break;

        case 2:
            // The firsts bytes after the header are usually another data sync mark (Data AM).
            // Once the pure data starts, we overturn it into the OS buffer:
            if (bytesRead >= 4) { // Skip the small bytes of initial separation if there was
                outputBuffer[bytesRead - 4] = currentByte; 
            }
            
            bytesRead++;
            
            if (bytesRead >= 516) { // 512 bytes of data + 4 data header
                state = 3; // Reading completed successfully
            }
            break;
    }
}

bool readSector(int sector, uint8_t* outputBuffer) {
    rotations = 0;
    uint32_t time = millis();
    while (rotations == 0) {
        if ((millis() - time) >= timeout) return; // Timeout
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
            if (interval >= 15 && interval < 25) {
                if (pushBit(1)) {

                }
            }
            if (interval >= 25 && interval < 35) {pushBit(0); pushBit(0); pushBit(1);}
            if (interval >= 35 && interval < 45) {pushBit(0); pushBit(0); pushBit(0); pushBit(1);}
        }
    }
}