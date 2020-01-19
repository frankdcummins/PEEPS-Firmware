# PEEPS-Firmware
Repository containing the C++ code required for the pressure measuring Arduino board.

## Theory of Operation
* This code will take readings from each sensor every 100 ms, or at a sampling frequency of 10Hz
* The readings are sampled straight from a 10-bit analog to digital converter and are therefore in a range of 0-1023
* This range corresponds to an analog voltage range of 0-3.3VDC, such that 512 would indicate a reading of 1.65 volts.

## Interface
* This code prints the readings to the integrated OLED display for easy viewing without a PC terminal.
* Additionally, this code prints the comma-separated readings to the serial port, configured at 115200 bps.

## Prerequisites
|**Item**|**Description**|
|:-------|:--------------|
|[Arduino Software](www.arduino.cc/en/Main/Software)|Integrated Development Environment for programming Arduino-based microcontrollers|
|[OLED Library](SparkFun_Micro_OLED_Arduino_Library-master.zip)|Library to enable the OLED display on the Pressure Sensor Shield|

---
*Last updated 2020-01-19
