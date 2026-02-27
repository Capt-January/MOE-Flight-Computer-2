#pragma once
#include <Arduino.h>

/*
    * Pin definitions for the MOE Flight Computer
    * 
    * This file defines the pin assignments for the various components of the flight computer,
    * including sensors, communication modules, and other peripherals. The pin numbers are
    * defined as constants to ensure consistency throughout the codebase and to make it easier
    * to modify pin assignments in the future if needed.

*/

// Pin definitions
static constexpr int8_t IMU_CS = 9; // IMU CS pin (using barometer CS pin for SPI communication)
static constexpr int8_t BARO_CS = 10; // Barometer CS pin
static constexpr int8_t MAG1_CS = 36;  // Magnetometer 1 CS pin
static constexpr int8_t MAG2_CS = 37;  // Magnetometer 2 CS pin
static constexpr int8_t RFM_95_CS = 38;  // LoRa CS pin

static constexpr int8_t GPS_RST = 22; // GPS reset pin
static constexpr int8_t RFM_95_RST = 2; // LoRa reset pin

static constexpr int8_t RFM_95_INT = 4; // LoRa IRQ pin
static constexpr int8_t GPS_INT = 23; // GPS IRQ pin
static constexpr int8_t MAG_A1_INT = 17; // Magnetometer A1 IRQ pin
static constexpr int8_t MAG_A2_INT = 16; // Magnetometer A2 IRQ pin
static constexpr int8_t MAG_B1_INT = 15; // Magnetometer B1 IRQ pin
static constexpr int8_t MAG_B2_INT = 14; // Magnetometer B2 IRQ pin