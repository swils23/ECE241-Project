### Goals:
* To create an LED matrix - a 4x4x4 cube of LEDs
* 64 independently addressable LEDs
* Interfaces to our custom ECE lab board, driven by an Arduino Nano


* Button Press: Change between various pre-defined patterns/modes
* Dial Rotation: Change the update speed of the pattern
* LCD Output: Display the current pattern and update speed

### Software:
**Arduino Code**\
[Led_Cube.ino](./LED_Cube/Led_Cube.ino) - Main\
[LedCubeData.h](./LED_Cube/LedCubeData.h) - Contains methods\
[ButtonDebounce.h](./LED_Cube/ButtonDebounce.h) - Monitors and debounces button presses\
[EncoderMonitor.h](./LED_Cube/EncoderMonitor.h) - Used to monitor encoder position

**Java:**
Testing suite

### Patterns:
* **Mode0: Default mode provided with assignment**\
As a test, each plane is given a location (row,column) and that plane is moved through all its locations.

* **Mode1: LED Test**\
All LEDs are turned on and then off one at a time by iterating through each r,c,p (row, col, plane) position.

* **Mode2: Squares**\
Lights up 4x4 squares on each plane, then rotates and lights up 4x4 squares on each column, then rotates and lights up 4x4 squares on each row.

* **Mode3: Snake**\
"Snakes" through the matrix 

* **Mode4: Checkerboard (Chkrbrd)**\
Alternating checkerboard pattern on each plane, inverting every cycle.

  

