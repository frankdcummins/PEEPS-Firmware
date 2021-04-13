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
const byte SAMPLE_PERIOD = 10;
const byte MOVING_AVERAGE_WINDOW = 16;

// Pins
const byte PIN_AIR_PUMP = 10;

// Air Pump
const byte AIR_PUMP_DUTY_CYCLE = 230;
const int TIME_PUMP_REST = 5000;
const int TIME_PUMP_ON = 10000;

// ===================================================================================== //
// Members
// ===================================================================================== //

// Sampling variables
unsigned long timeOfLastSample = 0UL;
float movingAverage[MOVING_AVERAGE_WINDOW];

// Analog sensor variables
const byte nSensors = 2;
const int sensorPins[] = {A0, A1};
const String sensorNames[] = {"0: ", "1: "};
int sensorReadings[] = {0, 0};

// Air pump variables
unsigned long timeSinceLastPumpStart = 0;
unsigned long timeSinceLastPumpStop = 0;
bool pumpState = false;

// Serial control variables
String serialMessage = "";

// ===================================================================================== //
// Setup Method
// ===================================================================================== //

void setup()
{
  // Start the serial bus at 115200 bps
  SerialUSB.begin(115200);

  pinMode(PIN_AIR_PUMP, OUTPUT);
  analogReadResolution(12);
}

// ===================================================================================== //
// Main Loop
// ===================================================================================== //

void loop()
{
  // Run Pressure Sensor Feature
  RunPressureSensorFeature();

  // Run Air Pump Feature
  RunAirPumpFeature();

  // Method to handle serial events
  HandleSerialEvents();
}

// ===================================================================================== //
// Auxiliary Methods
// ===================================================================================== //

// Method to run the pressure sensor feature
void RunPressureSensorFeature()
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
  else
  {
    return;
  }
}

// ------------------------------------------------------------------------------------- //

// Method to run the air pump feature
void RunAirPumpFeature()
{
  // If the pump is to be turned on
  if (pumpState == true)
  {
    // If more than TIME_PUMP_REST has elapsed since the last recording and
    // the time since the pump started is less than TIME_PUMP_ON
    if (millis() - timeSinceLastPumpStop > TIME_PUMP_REST &&
        millis() - timeSinceLastPumpStart < TIME_PUMP_ON)
    {
      // Turn on the pump
      analogWrite(PIN_AIR_PUMP, AIR_PUMP_DUTY_CYCLE);
    }
    else
    {
      // Lower the pump state flag
      pumpState = false;

      // Turn off the pump
      analogWrite(PIN_AIR_PUMP, 0);

      // Update the timer variable
      timeSinceLastPumpStop = millis();
    }
  }
  else
  {
    return;
  }
}

// ------------------------------------------------------------------------------------- //

// Method to parse serial commands
void parseCommand(String message)
{
  // If the serial terminal received an R for record
  if (serialMessage.equals("R"))
  {
    // Raise the pump state flag
    pumpState = true;

    // Update the timer variable
    timeSinceLastPumpStart = millis();
  }
}

// ------------------------------------------------------------------------------------- //

// Method to calculate moving average
int calculateMovingAverage(int value)
{
  float sumMeasurements = 0;

  for (uint8_t i = MOVING_AVERAGE_WINDOW - 1; i > 0; --i)
  {
    movingAverage[i] = movingAverage[i - 1];
    sumMeasurements += movingAverage[i];
  }
  movingAverage[0] = float(value);
  sumMeasurements += movingAverage[0];

  return (int)(sumMeasurements / MOVING_AVERAGE_WINDOW);
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

  // sensorReadings[0] = calculateMovingAverage(analogRead(sensorPins[1]));
  // sensorReadings[1] = analogRead(sensorPins[1]);
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
  SerialUSB.println(values);
}

// ------------------------------------------------------------------------------------- //

// Method to handle serial events
void HandleSerialEvents()
{
  // If there is any SerialUSB data available, read it
  if (SerialUSB.available())
  {
    // Read a byte from the SerialUSB communication buffer
    char serialInput = SerialUSB.read();

    // If we have reached the end of the message, begin processing it
    if (serialInput == '\n')
    {
      // Parse the command
      parseCommand(serialMessage);

      // Clear the SerialUSB message
      serialMessage = "";
    }

    // Otherwise, continue building the incoming SerialUSB message
    else
    {
      serialMessage += serialInput;
    }
  }
}

// ------------------------------------------------------------------------------------- //
// end of file
