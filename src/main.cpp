#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "hardware-configs/pins.h"
#include "hardware-configs/boardConfig.h"


// ============== Data Structs ==============

struct IMUData {
  float accel[3]; // x, y, z acceleration values
  float gyro[3];  // x, y, z gyroscope values
} imu1Data, imu2Data;

struct MagData {
  float mag[3]; // x, y, z magnetometer values
} mag1Data, mag2Data;

struct BaroData {
  float pressure; // Pressure in hPa
  float altitude; // Altitude in meters
} baroData;

struct GPSData {
  float lat, lon; // Latitude and Longitude
  float altitude; // Altitude in meters
  float speed;    // Speed in m/s
  uint8_t numSatellites; // Number of satellites in view
  bool valid; // Whether the GPS data is valid
} gpsData;

struct TelemetryPacket {
  uint32_t timestamp; // Timestamp in milliseconds since boot
  float roll, pitch, yaw; // Orientation angles in degrees
  float altitude; // Altitude in meters
  float lat, lon; // Latitude and Longitude
  uint16_t packetID; // Incrementing packet ID for tracking
} telemetry;

// ============== Setup ==============

void setup() {
  // Initialize RS-485 serial communication to Panda V2
  Serial8.begin(SERIAL_BAUD_RATE);
  Serial8.setTimeout(SERIAL_TIMEOUT);

  // Initialize GPS serial communication
  Serial1.begin(GPS_BAUD_RATE);
  Serial1.setTimeout(SERIAL_TIMEOUT); 

  SPI.begin();
  SPI.setClockDivider(4);
  // Initialize magnetometers and barometers here

  SPI1.begin();
  SPI1.setClockDivider(4);
  // Initialize LoRa module here

  Wire.begin();
  Wire.setClock(400000);
  // Initialize IMUs here

}

// ============== Main Loop ==============

void loop() {
  // put your main code here, to run repeatedly:
}

// ============== Sensor Reading Functions ==============
