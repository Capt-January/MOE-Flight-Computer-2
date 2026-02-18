#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

/*
    * Board configuration for the MOE Flight Computer
    * 
    * This file contains the configuration settings for the MOE Flight Computer, including
    * pin assignments, communication protocols, and other hardware-specific settings. It serves
    * as a central location for all hardware-related configurations, making it easier to manage
    * and modify the hardware setup of the flight computer.
    
*/

static const SPISettings spiSettings(1000000, MSBFIRST, SPI_MODE0); // SPI settings for sensors and peripherals

static constexpr unsigned int SERIAL_BAUD_RATE = 115200; // Baud rate for serial communication
static constexpr unsigned long GPS_BAUD_RATE = 9600; // Baud rate for GPS module
static constexpr unsigned int SERIAL_TIMEOUT = 1000; // Timeout for serial communication in milliseconds