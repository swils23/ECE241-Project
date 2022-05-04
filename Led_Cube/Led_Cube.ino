/*
PINS
  5V = 5V
  P0-P3 = A0-A3
  DIn = D11
  Clk = D13
  Gnd = Gnd
*/

#include <SPI.h>
#include <MsTimer2.h>
#include <LiquidCrystal.h>
#include "LedCubeData.h"
#include "ButtonDebounce.h"
#include "EncoderMonitor.h"

LiquidCrystal lcd(A5, A4, 5, 6, 7, 8);
unsigned long LedTimer = 0, LcdTimer = 0;
int r, c, p, count, mode, refresh;
char modeName[9];

// Sets the name of the mode/pattern to passed string.
void setModeName(char *x){
  for (int i = 0; i < 9; i++) modeName[i] = x[i];
}

// Reset all variables used by patterns.
void reset(){
  r = 0;
  c = 0;
  p = 0;
  count = 0;
  setModeName("");
}

void NextDisplay()
{
  // Send next pattern to Shift Registers.
  PORTC |= 0x0f; // set bottom bit low, turning off display.
  SPI.transfer16(~LedCube_NextPlane());
  // Turn on the current plane.
  PORTC &= ~(1 << LedCube_CurrentPlane);

} // End of NextDisplay



/* MODES
Mode0(): Default mode provided with assignment
  As a test, each plane is given a location (row,column) and that plane is moved through all its locations.

Mode1(): LED Test
  All LEDs are turned on and then off one at a time by iterating through each r,c,p (row, col, plane) position.

Mode2(): Squares
  Lights up 4x4 squares on each plane, then rotates and lights up 4x4 squares on each column, then rotates and lights up 4x4 squares on each row.

Mode3(): Snake
  "Snakes" through the matrix eventually lighting up the entire matrix, then unlights the entire matrix in the same fashion.

Mode4(): Checkerboard (Chkrbrd)
  Alternating checkerboard pattern on each plane, inverting every cycle.

******** TODO/Ideas: ********
Mode5(): Random
  Randomly lights up LEDs on each plane.

Mode6(): Bars
  Horiz or vertical bars?
******** TODO/Ideas ********
*/
int ZeroRow = 0, ZeroColumn = 0, OneRow = 1, OneColumn = 1, TwoRow = 2, TwoColumn = 2, ThreeRow = 3, ThreeColumn = 3;
void Mode0(){
  setModeName("Default");
  LedCube_ClearData();
  ZeroColumn++;
  if (ZeroColumn >= 4){
    ZeroColumn = 0;
    ZeroRow++;
    if (ZeroRow >= 4) ZeroRow = 0;
  }
  LedCube_SetLed(ZeroRow, ZeroColumn, 0);
  OneColumn++;
  if (OneColumn >= 4){
    OneColumn = 0;
    OneRow++;
    if (OneRow >= 4) OneRow = 0;
  }
  LedCube_SetLed(OneRow, OneColumn, 1);
  TwoColumn++;
  if (TwoColumn >= 4){
    TwoColumn = 0;
    TwoRow++;
    if (TwoRow >= 4) TwoRow = 0;
  }
  LedCube_SetLed(TwoRow, TwoColumn, 2);
  ThreeColumn++;
  if (ThreeColumn >= 4){
    ThreeColumn = 0;
    ThreeRow++;
    if (ThreeRow >= 4) ThreeRow = 0;
  }
  LedCube_SetLed(ThreeRow, ThreeColumn, 3);
}

void Mode1(){
  setModeName("LED Test");
  LedCube_ClearData();
  // set data
  LedCube_SetLed(r, c, p);

  // generate data
  r++;
  if (r > 3){
    r = 0;
    c++;
  }
  if (c > 3){
    c = 0;
    p++;
  }
  if (p > 3){
    p = 0;
  }
}

void Mode2(){
  setModeName("Squares");
  LedCube_ClearData();

  if (count < 4){
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(i, j, count);
    }
  }
  else if (count < 8){
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(i, count - 4, j);
    }
  }
  else if (count < 12){
    for (int i = 0; i < 4; i++){
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(count - 8, i, j);
    }
  }
  count++;
  if (count > 11){
    reset();
  }
}

void Mode3(){
  setModeName("Snake");

  LedCube_SetLed(r, c, p);
  // ticker
  if (((p % 2 == 0) && (c % 2 == 0)) || (!(p % 2 == 0) && !(c % 2 == 0))) r++;
  else r--;
  // "snake" logic
  if (p % 2 == 0){ // even planes
    if (r > 3){
      r = 3;
      c++;
    }
    if (r < 0){
      r = 0;
      c++;
    }
    if (c > 3){
      c = 3;
      p++;
    }
  }
  else{ // odd planes
    if (r > 3){
      r = 3;
      c--;
    }
    if (r < 0){
      r = 0;
      c--;
    }
    if (c < 0){
      c = 0;
      p++;
    }
  }
  // TODO reset or snake back through turning leds off?
  if (p > 3){
    LedCube_ClearData();
    reset();
  }
}

void Mode4(){
  setModeName("Chkrbrd");
  LedCube_ClearData();
  for(int i = 0; i < 4; i++) // iterate planes
    for(int j = 0; j < 4; j++) // iterate rows
      for(int k = 0; k < 4; k++){ // iterate columns
        if((i + j + k) % 2 == 0 && (count % 2 == 0)) LedCube_SetLed(k, j, i); // even cycle and even led position
        if((i + j + k) % 2 == 1 && (count % 2 == 1)) LedCube_SetLed(k, j, i); // odd cycle and odd led position
      }
  count++;
}

// Setup
void setup(){
  MsTimer2::set(4, NextDisplay); // 4ms period
  MsTimer2::start();
  // A3-A0 to outputs.
  DDRC |= 0x0f;
  // Set up display data.
  LedCube_ClearData();
  // SPI setup & configuration.
  SPI.begin();
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  // Initialize input monitors
  ButtonInitialize();
  EncoderInitialize();
  // Timers for LED and LCD displays
  LedTimer = millis();
  LcdTimer = millis();
  // Serial.begin(9600); // Serial output for debugging
  // Config LCD with 16x2 display
  lcd.begin(16, 2);
  // Initialize LED cube with default values
  reset();
  mode = 0; // default pattern
  refresh = 1000; // default refresh rate
}

// super loop
void loop(){
  // LED Pattern Display
  if (millis() - LedTimer >= refresh){ // LED refresh rate
    switch (mode){
    case 0:
      Mode0();
      break;
    case 1:
      Mode1();
      break;
    case 2:
      Mode2();
      break;
    case 3:
      Mode3();
      break;
    case 4:
      Mode4();
      break;
    }
    LedTimer += refresh;
  }
  // Encoder input for custom refresh rate
  refresh = 1000 + (encoderPosition/4 * 100);
  if (refresh <= 0) refresh = 1000;
  // LCD output (mode/pattern and refresh rate)
  if (millis() - LcdTimer >= 100){
    lcd.clear();
    lcd.print("Mode" + String(mode)+ ": " + String(modeName));
    lcd.setCursor(0, 1);
    lcd.print("Refresh: " + String(refresh) + "ms");
    LcdTimer += 100;
  }
  // Debounced button input handling to change mode/pattern
  if (ButtonNextState(digitalRead(4)) == 2){
    LedCube_ClearData();
    reset();
    mode++;
    if (mode > 4) mode = 0;
    // Serial.println("Mode: " + String(mode)); // debug
    // Serial.println("Refresh: " + String(refresh)); // debug
  }
}
