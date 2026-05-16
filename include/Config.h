#ifndef CONFIG
#define CONFIG
#include <Arduino.h>
/*
Global variables for configuration.
*/

// ___/ Floppy Pins \___________

#define FLOPPY_MOTOR_ON    12
#define FLOPPY_DRIVE_SEL   13
#define FLOPPY_DIRECTION   14
#define FLOPPY_STEP        27  
#define FLOPPY_READ_DATA   32  
#define FLOPPY_INDEX       25 
#define FLOPPY_TRACK00     26

// ___/ SPI Bus (Display + MicroSD) \_____
#define SPI_MOSI           23
#define SPI_MISO           19
#define SPI_SCK            18
#define TFT_CS             15
#define SD_CS              5

// ___/ Keyboard (PS/2) \_____
#define KB_DATA            4
#define KB_CLK             2

#endif