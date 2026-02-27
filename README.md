# MOE Flight Computer

A Teensy 4.1-based flight computer designed for high-power rocketry applications. The system provides real-time sensor fusion, telemetry transmission, and data logging capabilities for flight tracking and recovery.

## Overview

The MOE Flight Computer integrates multiple redundant sensors to ensure reliable data acquisition during flight. It transmits telemetry data via 433MHz LoRa radio to a ground station and communicates with an external system (Panda V2) over RS-485.

## Hardware

### Microcontroller
- **Teensy 4.1** - ARM Cortex-M7 running at 600MHz with ample I/O for sensor integration

### Sensors

| Sensor | Model | Interface | Purpose |
|--------|-------|-----------|---------|
| Primary IMU | BMI270 | SPI | 6-axis accelerometer + gyroscope (1000 Hz) |
| Secondary IMU | ADXL343/375 | I2C | High-g accelerometer for redundancy |
| Magnetometer 1 | LIS2MDL | I2C | 3-axis magnetic field sensing (100 Hz) |
| Magnetometer 2 | LIS2MDL | I2C | Redundant magnetometer |
| Barometer | MS5611 | SPI | Pressure/altitude measurement (50 Hz) |
| GPS | u-blox GNSS | Serial (UART) | Position and velocity tracking (10 Hz) |

### Communication
- **LoRa Radio (RFM95)** - 433 MHz telemetry transmission at 10 Hz
- **RS-485** - Serial communication to Panda V2 system (115200 baud)

## Sensor Update Rates

| Sensor Type | Update Rate |
|-------------|-------------|
| IMU | 1000 Hz |
| Magnetometer | 100 Hz |
| Barometer | 50 Hz |
| GPS | 10 Hz |
| LoRa Telemetry | 10 Hz |

## Pin Configuration

### SPI Chip Select Pins
| Pin | Device |
|-----|--------|
| 9 | IMU (BMI270) |
| 10 | Barometer (MS5611) |
| 36 | Magnetometer 1 |
| 37 | Magnetometer 2 |
| 38 | LoRa (RFM95) |

### Control & Interrupt Pins
| Pin | Function |
|-----|----------|
| 2 | LoRa Reset |
| 4 | LoRa IRQ |
| 22 | GPS Reset |
| 23 | GPS IRQ |

## Telemetry Packet Structure

Each telemetry packet transmitted over LoRa contains:
- Timestamp (ms since boot)
- Orientation (roll, pitch, yaw)
- Barometric altitude
- GPS coordinates (latitude, longitude)
- Packet ID for tracking

## Building & Flashing

This project uses [PlatformIO](https://platformio.org/) for build management.

### Prerequisites
- PlatformIO Core or PlatformIO IDE
- USB connection to Teensy 4.1

### Build Commands

```bash
# Build the project
pio run

# Upload to Teensy 4.1
pio run --target upload

# Monitor serial output
pio device monitor
```

## Dependencies

| Library | Version | Purpose |
|---------|---------|---------|
| SparkFun u-blox GNSS Arduino Library | ^2.2.28 | GPS module communication |
| RadioHead | ^1.120 | LoRa radio driver |
| Adafruit ADXL375 | ^1.1.2 | Secondary IMU driver |
| SparkFun BMI270 Arduino Library | ^1.0.3 | Primary IMU driver |
| Adafruit LIS2MDL | ^2.1.8 | Magnetometer driver |
| MS5611 | ^0.5.1 | Barometer driver |

## Project Structure

```
MOE Flight Computer/
├── src/
│   ├── main.cpp                 # Main application code
│   └── hardware-configs/
│       ├── pins.h               # Pin assignments
│       └── boardConfig.h        # Hardware configuration
├── platformio.ini               # PlatformIO configuration
└── README.md
```

## Planned Features

- Fault detection and sensor fallback logic
- Flash storage for flight data
- SD card data offloading during idle periods
- Attitude estimation from sensor fusion

## License

This project is part of the MOE rocketry program.
