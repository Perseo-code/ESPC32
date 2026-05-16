#include "Config.h"
#include "DisplayMgr.h"
#include "FileSystem.h"
#include "FloppyDriver.h"

void setup() {
    Serial.begin(19200);
    initializeFloppyPins();
}

void loop() {
    
}