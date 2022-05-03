/* Pins:
 * 5V = 5V
 * P0-P3 = A0-A3
 * DIn = D11
 * Clk = D13
 * Gnd = Gnd
 */

#include <SPI.h>
#include <MsTimer2.h>

unsigned long Timer = 0;

#include "LedCubeData.h"
#include "ButtonDebounce.h"

void NextDisplay()
{
  // Send next pattern to Shift Registers.
  PORTC |= 0x0f; // set bottom bit low, turning off display.
  SPI.transfer16(~LedCube_NextPlane());
  // Turn on the current plane.
  PORTC &= ~(1 << LedCube_CurrentPlane);

} // End of NextDisplay

// As a test, each plane is given a location (row,column)
// and that plane is moved through all its locations.
int ZeroRow = 0, ZeroColumn = 0;
void MoveZero()
{
  ZeroColumn++;
  if (ZeroColumn >= 4)
  {
    ZeroColumn = 0;
    ZeroRow++;
    if (ZeroRow >= 4)
      ZeroRow = 0;
  }
} // End of MoveZero

int OneRow = 1, OneColumn = 1;
void MoveOne()
{
  OneColumn++;
  if (OneColumn >= 4)
  {
    OneColumn = 0;
    OneRow++;
    if (OneRow >= 4)
      OneRow = 0;
  }
} // End of MoveOne

int TwoRow = 2, TwoColumn = 2;
void MoveTwo()
{
  TwoColumn++;
  if (TwoColumn >= 4)
  {
    TwoColumn = 0;
    TwoRow++;
    if (TwoRow >= 4)
      TwoRow = 0;
  }
} // End of MoveTwo

int ThreeRow = 3, ThreeColumn = 3;
void MoveThree()
{
  ThreeColumn++;
  if (ThreeColumn >= 4)
  {
    ThreeColumn = 0;
    ThreeRow++;
    if (ThreeRow >= 4)
      ThreeRow = 0;
  }
} // End of MoveThree

void Mode0()
{
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

int r, c, p, count;
void reset()
{
  r = 0;
  c = 0;
  p = 0;
  count = 0;
}

void Mode1()
{
  LedCube_ClearData();
  // set data
  LedCube_SetLed(r, c, p);

  // generate data
  r++;
  if (r > 3)
  {
    r = 0;
    c++;
  }
  if (c > 3)
  {
    c = 0;
    p++;
  }
  if (p > 3)
  {
    p = 0;
  }
}

void Mode2()
{
  LedCube_ClearData();

  if (count < 4)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(i, j, count);
    }
  }
  else if (count < 8)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(i, count - 4, j);
    }
  }
  else if (count < 12)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
        LedCube_SetLed(count - 8, i, j);
    }
  }
  count++;
  if (count > 11)
  {
    reset();
  }
}

void Mode3()
{

  LedCube_SetLed(r, c, p);

  // ticker
  if (((p % 2 == 0) && (c % 2 == 0)) || (!(p % 2 == 0) && !(c % 2 == 0)))
    r++;
  else
    r--;

  // "snake" logic
  if (p % 2 == 0)
  { // even planes
    if (r > 3)
    {
      r = 3;
      c++;
    }
    if (r < 0)
    {
      r = 0;
      c++;
    }
    if (c > 3)
    {
      c = 3;
      p++;
    }
  }
  else
  { // odd planes
    if (r > 3)
    {
      r = 3;
      c--;
    }
    if (r < 0)
    {
      r = 0;
      c--;
    }
    if (c < 0)
    {
      c = 0;
      p++;
    }
  }

  // TODO reset or snake back through turning leds off?
  if (p > 3)
  {
    LedCube_ClearData();
    reset();
  }
}

// setup code, run once:
void setup()
{
  MsTimer2::set(4, NextDisplay); // 4ms period
  MsTimer2::start();

  Serial.begin(9600);

  // A3-A0 to outputs.
  DDRC |= 0x0f;
  // Set up display data.
  LedCube_ClearData();
  // Start up the SPI
  SPI.begin();
  // Set the parameters for the transfers.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));

  ButtonInitialize();

  // Timer for moving the ON led's
  Timer = millis();

} // End of setup

int Mode = 0;

// main code, run repeatedly:
void loop()
{
  // 2000 millisecond timer to update display
  if (millis() - Timer >= 250)
  {
    switch (Mode)
    {
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
    Timer += 250; // Update timer
  }               // End of timer if.

  if (ButtonNextState(digitalRead(4)) == 2)
  {
    LedCube_ClearData();
    reset();
    Mode++;
    if (Mode > 3)
      Mode = 0;
    Serial.print("Mode: ");
    Serial.println(Mode);
  }
} // End of loop.
