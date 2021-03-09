/******************************************************************************
   PEEPS .ino
   Arduino sketch for Pressure Measuring Device

   Author:  Frank Cummins
   Date:    2021-03-08
******************************************************************************/

// ===================================================================================== //
// Included libraries
// ===================================================================================== //

// ===================================================================================== //
// Constants
// ===================================================================================== //

// Define the sample period in milliseconds
const byte SAMPLE_PERIOD = 10

// ===================================================================================== //
// Members
// ===================================================================================== //

// Sampling values
unsigned long timeOfLastSample = 0L;

// Analog sensor values
const byte nSensors = 2;
const int sensorPins[] = {A0, A1};
const String sensorNames[] = {"0: ", "1: "};
int sensorReadings[] = {0, 0};

// ===================================================================================== //
// Setup Method
// ===================================================================================== //

void setup()
{
  // Start the serial bus at 115200 bps
  Serial.begin(115200);

  // Wait a bit for things to get started
  delay(100);
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

    // Update the timer variable
    timeOfLastSample = millis();
  }
}

// ===================================================================================== //
// Auxiliary Methods
// ===================================================================================== //

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
  for (int sensor = 0; sensor < nSensors - 1; sensor++)
  {
    values.concat(String(sensorReadings[sensor]));
    values.concat(",");
  }
  values.concat(String(sensorReadings[nSensors - 1]));

  // Print the values string to the serial port
  Serial.println(values);
}

// ------------------------------------------------------------------------------------- //

// end of file
