# PEEPS-Firmware
Repository containing the C++ code required for the Pressure Sensor Shield for Arduino

## Theory of Operation
* This code will take readings from each sensor every 10 ms, or at a sampling frequency of 100Hz.
* The readings are sampled straight from a 10-bit analog to digital converter and are therefore in a range of 0-1023.
* This range corresponds to an analog voltage range of 0-3.3VDC, such that 512 would indicate a reading of 1.65 volts.

## Interface
* This code displays the qualitative intensity of the pressure readings via the attached NEOPIXEL LEDs.
* Additionally, this code prints the quantitative pressure readings to the serial port, configured at 115200 bps.

## Prerequisites
|**Item**|**Description**|
|:-------|:--------------|
|[Arduino Software](www.arduino.cc/en/Main/Software)|Integrated Development Environment for programming Arduino-based microcontrollers|
|[OLED Library](SparkFun_Micro_OLED_Arduino_Library-master.zip)|Library to enable the OLED display on the Pressure Sensor Shield|

---
*Last updated 2021-03-08*