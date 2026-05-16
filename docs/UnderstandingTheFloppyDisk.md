# Understanding The Floppy Disk
*Floppy Disks* are one of the most well-known retro digital storage devices. These write data using **magnetic pulses**, like *HDDs* or *magnetic tapes* (for *casettes* or *VHS*). 

So... What's the magic behind them? 

> Most of the magic usually comes with the **floppy drive**, which reads the disk using 34 *PINs* or 26, depending on the type of driver. If it is from an old laptop, it will most probably be a *26-Pin* one, while on old PCs use a *34-Pin* one.

# How could my computer read them?
Your computer had what we, computer scientists call a driver. A driver is not a floppy drive, so you understand that we're not talking about the same thing. A driver is a program written so you can use for example your keyboard. It's the program that makes all your external hardware work.

# How does the driver work?
Well, a driver receives the electronic signals from the floppy disk and puts logic into it so that you can read the content, remove it or even copy it.
These work on one of the lowest level in your computer. They were usually programmed in C and some made entirely in assembly.

# Every _PIN_ and its functionality (Floppy Drive)
I haven't looked into each PIN, only in the important ones. One important thing that these drives have is that the output and input are reversed in some of them, so you have to use a pull-up input if you want to make your code more readable. But, for the output, you'll have to stick to the normal one. In other words, you'll have to use LOW when you want to give the signal to the drive.

### Warning, this is my configuration for the ESP32. It's just an example
```
#define FLOPPY_MOTOR_ON    12 // <-- MOTOR PIN
#define FLOPPY_DRIVE_SEL   13 // <-- DRIVE_SELECTOR PIN
#define FLOPPY_DIRECTION   14 // <-- DIRECTION PIN
#define FLOPPY_STEP        27 // <-- Step PIN
#define FLOPPY_READ_DATA   32 // <-- READ_DATA PIN
#define FLOPPY_INDEX       25 // <-- INDEX PIN
#define FLOPPY_TRACK00     26 // <-- TRACK00
```
Each one has its own purpose. But as I said before, these are the PINS that I connected from the drive to the ESP. It's not the full pinout.
### Floppy Drive 34-Pin Data Interface


| Pin | Signal Name | Direction | Description |
| :--- | :--- | :--- | :--- |
| **1-33 (Odds)** | GND | - | Ground lines for noise reduction |
| **2** | /REDWC | To Drive | Write density select / Density mode |
| **4** | NC | - | Reserved / Not connected |
| **6** | /DS3 | To Drive | Drive select 3 (Rare on PC) |
| **8** | /INDEX | To Host | Index pulse (One pulse per rotation) |
| **10** | /MOTEA | To Drive | Motor enable A |
| **12** | /DRVSB | To Drive | Drive select B |
| **14** | /DRVSA | To Drive | Drive select A |
| **16** | /MOTEB | To Drive | Motor enable B |
| **18** | /DIR | To Drive | Direction control (High=Out, Low=In) |
| **20** | /STEP | To Drive | Step pulse (Moves head one track) |
| **22** | /WDATA | To Drive | Write data bits |
| **24** | /WGATE | To Drive | Write gate (Enable write) |
| **26** | /TRK00 | To Host | Track 00 sensor (Head at start) |
| **28** | /WPROT | To Host | Write protect sensor |
| **30** | /RDATA | To Host | Read data bits |
| **32** | /SIDE1 | To Drive | Select head (High=Side 0, Low=Side 1) |
| **34** | /DSKCHG | To Host | Diskette change detect / Ready signal |

---

### Floppy Power Connector (Berg / Mini-Molex)

* **Pin 1:** Yellow -> **+12V** (Powers the drive motors)
* **Pin 2:** Black -> **GND** (Ground)
* **Pin 3:** Black -> **GND** (Ground)
* **Pin 4:** Red -> **+5V** (Powers the internal logic)
