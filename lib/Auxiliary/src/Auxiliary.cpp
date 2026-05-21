#include "Auxiliary.h"

static uint8_t accumulatedBits = 0;
static int bitCounter = 0;
uint8_t lastCompletedByte = 0; // New global variable so we can use it later

bool pushBits(int bit) {
    accumulatedBits = (accumulatedBits << 1) | bit;
    bitCounter++;

    if (bitCounter == 8) {
        lastCompletedByte = accumulatedBits; // We save the byte for the loop
        bitCounter = 0;
        accumulatedBits = 0;
        return true; // There's now a fresh and new byte.
    }
    return false; // Aún faltan bits para completar el byte
}
