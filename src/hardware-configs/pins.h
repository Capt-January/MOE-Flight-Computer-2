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
static constexpr int8_t cs1 = 10; // Barometer CS pin
static constexpr int8_t cs2 = 36;  // Magnetometer 1 CS pin
static constexpr int8_t cs3 = 37;  // Magnetometer 2 CS pin
static constexpr int8_t cs4 = 38;  // LoRa CS pin

static constexpr int8_t reset1 = 22; // GPS reset pin
static constexpr int8_t reset2 = 2; // LoRa reset pin

static constexpr int8_t irq1 = 4; // LoRa IRQ pin
static constexpr int8_t irq2 = 23; // GPS IRQ pin
static constexpr int8_t irqA1 = 17; // Magnetometer A1 IRQ pin
static constexpr int8_t irqA2 = 16; // Magnetometer A2 IRQ pin
static constexpr int8_t irqB1 = 15; // Magnetometer B1 IRQ pin
static constexpr int8_t irqB2 = 14; // Magnetometer B2 IRQ pin