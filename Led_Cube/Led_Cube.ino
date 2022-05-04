/* Pins:
 * 5V = 5V
 * P0-P3 = A0-A3
 * DIn = D11
 * Clk = D13
 * Gnd = Gnd
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

void NextDisplay()
{
  // Send next pattern to Shift Registers.
  PORTC |= 0x0f; // set bottom bit low, turning off display.
  SPI.transfer16(~LedCube_NextPlane());
  // Turn on the current plane.
  PORTC &= ~(1 << LedCube_CurrentPlane);

} // End of NextDisplay

// As a test, each plane is given a location (row,column) and that plane is moved through all its locations.
int ZeroRow = 0, ZeroColumn = 0, OneRow = 1, OneColumn = 1, TwoRow = 2, TwoColumn = 2, ThreeRow = 3, ThreeColumn = 3;
void MoveZero(){
  ZeroColumn++;
  if (ZeroColumn >= 4){
    ZeroColumn = 0;
    ZeroRow++;
    if (ZeroRow >= 4)
      ZeroRow = 0;
  }
} // End of MoveZero

void MoveOne(){
  OneColumn++;
  if (OneColumn >= 4){
    OneColumn = 0;
    OneRow++;
    if (OneRow >= 4)
      OneRow = 0;
  }
}

void MoveTwo(){
  TwoColumn++;
  if (TwoColumn >= 4){
    TwoColumn = 0;
    TwoRow++;
    if (TwoRow >= 4)
      TwoRow = 0;
  }
}

void MoveThree(){
  ThreeColumn++;
  if (ThreeColumn >= 4){
    ThreeColumn = 0;
    ThreeRow++;
    if (ThreeRow >= 4)
      ThreeRow = 0;
  }
}

void Mode0(){
  LedCube_ClearData();
  MoveZero();
  LedCube_SetLed(ZeroRow, ZeroColumn, 0);
  MoveOne();
  LedCube_SetLed(OneRow, OneColumn, 1);
  MoveTwo();
  LedCube_SetLed(TwoRow, TwoColumn, 2);
  MoveThree();
  LedCube_SetLed(ThreeRow, ThreeColumn, 3);
}

// Reset all variables used by patterns.
void reset(){
  r = 0;
  c = 0;
  p = 0;
  count = 0;
}

// lights up each led individually
void Mode1(){
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

// lights up 4x4 squares on each plane, then rotates and lights up 4x4 squares on each column, then rotates and lights up 4x4 squares on each row
void Mode2(){
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

// "snakes" through the matrix eventually lighting up the entire matrix, then unlights the entire matrix in the same fashion
void Mode3(){

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

// setup code, run once:
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
  refresh = 2000; // default refresh rate
}

void loop(){ // super loop
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
    }
    LedTimer += refresh;
  }
  // Encoder input for custom refresh rate
  refresh = 2000 + (encoderPosition/4 * 100);
  if (refresh <= 0) refresh = 2000;
  // LCD output (mode/pattern and refresh rate)
  if (millis() - LcdTimer >= 100){
    lcd.clear();
    lcd.print("Mode: " + String(mode));
    lcd.setCursor(0, 1);
    lcd.print("Refresh: " + String(refresh) + "ms");
    LcdTimer += 100;
  }
  // Debounced button input handling to change mode/pattern
  if (ButtonNextState(digitalRead(4)) == 2){
    LedCube_ClearData();
    reset();
    mode++;
    if (mode > 3)
      mode = 0;
    // Serial.println("Mode: " + String(mode)); // debug
    // Serial.println("Refresh: " + String(refresh)); // debug
  }
}
