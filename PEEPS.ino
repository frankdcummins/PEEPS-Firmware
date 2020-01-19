/******************************************************************************
   PEEPS .ino
   Arduino sketch for Pressure Measuring Device

   Author:  Frank Cummins
   Date:    2020-01-19
   ******************************************************************************/

// ===================================================================================== //
// Included libraries
// ===================================================================================== //

// Wire library for I2C communications
#include <Wire.h>

// SFE_MicroOLED library for OLED display
#include <SFE_MicroOLED.h>

// ===================================================================================== //
// Definitions
// ===================================================================================== //

// The library assumes a reset pin is necessary.
// The Qwiic OLED has RST hard-wired, so pick an arbitrarty IO pin that is not being used
#define PIN_RESET 9

//The DC_JUMPER is the I2C Address Select jumper.
// Set to 1 if the jumper is open (Default), or set to 0 if it's closed.
#define DC_JUMPER 1

// Define the sample period in milliseconds
#define SAMPLE_PERIOD 100

// ===================================================================================== //
// Members
// ===================================================================================== //

// OLED Display
MicroOLED oled(PIN_RESET, DC_JUMPER);

// Sampling values
unsigned long timeOfLastSample = 0L;

// Analog sensor values
const byte nSensors = 4;
const int sensorPins[] = {A0, A1, A2, A3};
const String sensorNames[] = {"0: ", "1: ", "2: ", "3: "};
int sensorReadings[] = {0, 0, 0, 0};

// ===================================================================================== //
// Setup Method
// ===================================================================================== //

void setup()
{
  // Start the serial bus at 115200 bps
  Serial.begin(115200);

  // Start te I2C bus
  Wire.begin();

  // Wait a bit for things to get started
  delay(100);

  // Initialize the OLED
  oled.begin();
  oled.clear(ALL); // Clear the display's internal memory
  printTitle("PEEPS Pressure Breakout", 0);
}

// ===================================================================================== //
// Main Loop
// ===================================================================================== //

void loop()
{
  // If SAMPLE_PERIOD has elapsed
  if (millis() - timeOfLastSample >= SAMPLE_PERIOD)
  {
    // Sample the sensor values
    sampleSensors();

    // Print the readings to the serial port
    printReadingsToSerial();

    // Print the readings for sensors on the OLED display
    printReadingsToOLED();
  }
}

// ===================================================================================== //
// Auxiliary Methods
// ===================================================================================== //

// Method to print a splash screen of text
void printTitle(String title, int font)
{
  oled.clear(PAGE);
  oled.setFontType(font);
  oled.setCursor(0, 4);
  oled.print("PEEPS");
  oled.setCursor(0, 20);
  oled.print("Pressure");
  oled.setCursor(0, 36);
  oled.print("Sensors");
  oled.display();
  delay(1500);
  oled.clear(PAGE);
}

// ------------------------------------------------------------------------------------- //

// Method to sample the sensors
void sampleSensors()
{
  // Loop over the sensors and load the readings into the sensorReadings array
  for (int sensor = 0; sensor < nSensors; sensor++)
  {
    sensorReadings[sensor] = analogRead(sensorPins[sensor]);
  }
}

// ------------------------------------------------------------------------------------- //

// Method to print the sensor readings to the Serial port
void printReadingsToSerial()
{
  // Instantiate the string of values
  String values = "";

  // Loop over the sensors and concatenate the data onto the values string
  for (int sensor = 0; sensor < nSensors-1; sensor++)
  {
    values.concat(String(sensorReadings[sensor]));
    values.concat(",");
  }
  values.concat(String(sensorReadings[nSensors - 1]));

  // Print the values string to the serial port
  Serial.println(values);
}

// ------------------------------------------------------------------------------------- //

// Method to print the sensor readings to the OLED display
void printReadingsToOLED()
{
  // Clear the display
  oled.clear(PAGE);

  // Loop over the sensors and add data to the display buffer
  for (int sensor = 0; sensor < nSensors; sensor++)
  {
    oled.setCursor(0, 13 * sensor);
    oled.print(String(sensorNames[sensor]));
    oled.print(String(sensorReadings[sensor]));
  }

  // Show what is in the display buffer
  oled.display();
}

// ------------------------------------------------------------------------------------- //

// end of file
