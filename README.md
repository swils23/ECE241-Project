## LED Cube

---
### Outline

**Goals:**
- To create an LED matrix - a 4x4x4 cube of LEDs
- 64 independently addressable LEDs
- Interfaces to our custom ECE lab board, driven by an Arduino Nano

**IO:**
- Button Press: Change between various pre-defined patterns/modes
- Dial Rotation: Change the update speed of the pattern
- LCD Output: Display the current pattern and update speed

**Patterns:**

* **Default**\
  As a test, each plane is given a location (row,column) and that plane is moved through all its locations.

* **LED Test**\
  All LEDs are turned on and then off one at a time by iterating through each r,c,p (row, col, plane) position.

* **Squares**\
  Lights up 4x4 squares on each plane, then rotates and lights up 4x4 squares on each column, then rotates and lights up 4x4 squares on each row.

* **Snake**\
  "Snakes" through the matrix

* **Checkerboard**\
Alternating checkerboard pattern on each plane, inverting every cycle.


---

### Software

**Arduino Code**
- [Led_Cube.ino](./LED_Cube/Led_Cube.ino) - Contains the "superloop" that handles all logic involving pattern selection and IO
- [LedCubeData.h](./LED_Cube/LedCubeData.h) - Contains methods to interface with LED cube
- [ButtonDebounce.h](./LED_Cube/ButtonDebounce.h) - Monitors and debounces button presses
- [EncoderMonitor.h](./LED_Cube/EncoderMonitor.h) - Used to monitor encoder position

**Simulation Code (Java)**
- [Matrix.java]() - This program mimics the methods contained in [LedCubeData.ino]() to allow easier pattern development and debugging
- [Pattern.java]() - Contains the patterns to be displayed
- [PatternVisualizer.java]() - Converts pattern into a command-line-friendly format

---


  

