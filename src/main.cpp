#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <time.h>
#include <SD.h>
#include "SparkFun_u-blox_GNSS_Arduino_Library.h"
#include "RH_RF95.h"
#include "Adafruit_ADXL375.h"
#include "SparkFun_BMI270_Arduino_Library.h"
#include "Adafruit_LIS2MDL.h"
#include "MS5611.h"
#include "hardware-configs/pins.h"
#include "hardware-configs/boardConfig.h"

/*
  Implementation list:
  - Connection to all sensors (IMUs, magnetometers, barometer, GPS)
  - Reading data from all sensors
  - Transmit all data over 433MHz LoRa to ground station
  - Check for faulty sensors, and implement fallback logic
  - 
*/

#define IMU1_RATE_HZ    1000
#define IMU2_RATE_HZ    1000
#define MAG_RATE_HZ     100
#define BARO_RATE_HZ    50
#define GPS_RATE_HZ     10
#define LORA_RATE_HZ    10
#define SD_LOG_RATE_HZ  10


#define RF_95_FREQ 433.0

// ============== Sensor Initialization ==============

BMI270 imu1; // First IMU
Adafruit_ADXL343 imu2; // Second IMU

Adafruit_LIS2MDL mag1; // First magnetometer
Adafruit_LIS2MDL mag2; // Second magnetometer

MS5611 baro; // Barometer

SFE_UBLOX_GNSS gps; // GPS module

RH_RF95 rf95(RFM_95_CS, RFM_95_INT); // LoRa radio

uint32_t lastIMU_us   = 0;
uint32_t lastMag_us   = 0;
uint32_t lastBaro_us  = 0;
uint32_t lastGPS_us   = 0;
uint32_t lastLoRa_us  = 0;
uint32_t lastSD_us    = 0;

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

  TelemetryPacket() {
    altitude = 0.0;
    timestamp = 0;
    packetID = 0;
  }
} telemetry;

File globalFile; // Initialize global file object


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

  mag1.begin(); // Initialize magnetometer 1
  mag2.begin(); // Initialize magnetometer 2

  imu1.beginSPI(IMU_CS); // Initialize IMU 1 over SPI using barometer CS pin

  SPI1.begin();
  SPI1.setClockDivider(4);
  // Initialize LoRa module here

  pinMode(RFM_95_RST, OUTPUT);
  digitalWrite(RFM_95_RST, HIGH);

  rf95.init();
  rf95.setFrequency(RF_95_FREQ);
  rf95.setTxPower(15, false); // Set transmission power to 15 dBm (adjust as needed)

  Wire.begin();
  Wire.setClock(400000);
  // Initialize IMUs here

  imu2.begin(); // Initialize IMU 2 over I2C

  while(!gps.begin(Serial1)) {
    Serial.println("GPS not detected, retrying...");
    delay(1000);
  }

  SD.begin(BUILTIN_SDCARD); // Initialize SD card read/write over SDIO

  

  globalFile = SD.open("flight_log.bin", FILE_WRITE); // Open log file for writing


}

// ============== Sensor Reading Functions ==============

void readIMUData() {
  // Read data from both IMUs and populate imu1Data and imu2Data
  imu1.getSensorData();
  imu1Data.accel[0] = imu1.data.accelX;
  imu1Data.accel[1] = imu1.data.accelY;
  imu1Data.accel[2] = imu1.data.accelZ;
  imu1Data.gyro[0] = imu1.data.gyroX;
  imu1Data.gyro[1] = imu1.data.gyroY;
  imu1Data.gyro[2] = imu1.data.gyroZ;

  sensors_event_t event;
  imu2.getEvent(&event);
  imu2Data.accel[0] = event.acceleration.x;
  imu2Data.accel[1] = event.acceleration.y;
  imu2Data.accel[2] = event.acceleration.z;
}

void readMagData() {
  // Read data from both magnetometers and populate mag1Data and mag2Data
  sensors_event_t event;
  mag1.getEvent(&event);
  mag1Data.mag[0] = event.magnetic.x;
  mag1Data.mag[1] = event.magnetic.y;
  mag1Data.mag[2] = event.magnetic.z;
  
  mag2.getEvent(&event);
  mag2Data.mag[0] = event.magnetic.x;
  mag2Data.mag[1] = event.magnetic.y;
  mag2Data.mag[2] = event.magnetic.z;
}

void readBaroData() {
  // Read data from barometer and populate baroData
  baroData.pressure = baro.getPressure();
  baroData.altitude = baro.getAltitude();
}

void readGPSData() {
  // Read data from GPS and populate gpsData
  gpsData.lat = gps.getLatitude();
  gpsData.lon = gps.getLongitude();
  gpsData.altitude = gps.getAltitude();
  gpsData.numSatellites = gps.getSIV();
  gpsData.valid = gps.getFixType() > 0; // Valid if we have
}


void transmitTelemetry() {
  // Populate telemetry packet
  telemetry.timestamp = millis();
  telemetry.roll = 0; // Placeholder, calculate from IMU data
  telemetry.pitch = 0; // Placeholder, calculate from IMU data
  telemetry.yaw = 0; // Placeholder, calculate from IMU data
  telemetry.altitude = baroData.altitude;
  telemetry.lat = gpsData.lat;
  telemetry.lon = gpsData.lon;
  telemetry.packetID++;

  // Transmit telemetry packet over LoRa

  // Implement LoRa transmission logic here


  rf95.send((uint8_t *)&telemetry, sizeof(telemetry));
  rf95.waitPacketSent();
}

// Write data to SD
void writeToSD() {
  /* globalFile.print(millis());
  globalFile.print(",");
  globalFile.print(baroData.altitude);
  globalFile.print(",");
  globalFile.print(gpsData.lat, 6);
  globalFile.print(",");
  globalFile.print(gpsData.lon, 6);
  globalFile.print(",");
  globalFile.flush();
  globalFile.close();
  */

  globalFile.write((uint8_t *)&telemetry, sizeof(telemetry));
  globalFile.flush();

}



// ============== Main Loop ==============

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t now = micros();

  // Find IMU frequency
  if (now - lastIMU_us >= (1000000 / IMU1_RATE_HZ)) {
    readIMUData();
    lastIMU_us = now;
  }

  // Find magnetometer frequency
  if (now - lastMag_us >= (1000000 / MAG_RATE_HZ)) {
    readMagData();
    lastMag_us = now;
  }

  // Find barometer frequency
  if (now - lastBaro_us >= (1000000 / BARO_RATE_HZ)) {
    readBaroData();
    lastBaro_us = now;
  }

  // Find GPS frequency
  if (now - lastGPS_us >= (1000000 / GPS_RATE_HZ)) {
    readGPSData();
    lastGPS_us = now;
  }

 // Write to SD
if (now - lastSD_us >= (1000000 / SD_LOG_RATE_HZ)) {
    writeToSD(); 
    lastSD_us = now;
  }

  // Find LoRa transmission frequency
  if (now - lastLoRa_us >= (1000000 / LORA_RATE_HZ)) {
    transmitTelemetry();
    lastLoRa_us = now;
  }

  // Fault detection and fallback logic

  // Store data to flash

  // Move data to SD card in idle time (because SD card writing is slow and can cause delays)

  // IMU roll, pitch, yaw calc needed
}