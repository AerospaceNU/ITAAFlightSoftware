#include "hardware_manager.h"

#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_DPS310.h>
#include <SD.h>

// Function stubs for everything in hardware_manager.h (just so the project will compile)
// These will all need to be created to do something!

Adafruit_LSM6DS3TRC lsm6ds3trc;
Adafruit_DPS310 dps;
Adafruit_Sensor *dps_temp;
Adafruit_Sensor *dps_pressure;

const uint8_t LSM_CS = 6;
const uint8_t DPS_CS = 9;
const uint8_t SD_CS = 10;


void HardwareManager::init() {
    // Initialize the DPS310!
    if (! dps.begin_SPI(DPS_CS)) {
      Serial.println("Failed to find DPS");
    while (1) yield();
    }
    dps_temp = dps.getTemperatureSensor();
    dps_pressure = dps.getPressureSensor();

    


    // Initialize the LSM6DS!
  if (!lsm6ds3trc.begin_SPI(LSM_CS)) {
      Serial.println("Failed to find LSM6DS3TR-C chip");
      while (1) {
        delay(10);
      }
    }



    // Initialize the SD card!
    if (!SD.begin(SD_CS)) {
      Serial.println("initialization failed. Things to check:");
      Serial.println("1. is a card inserted?");
      Serial.println("2. is your wiring correct?");
      Serial.println("3. did you change the chipSelect pin to match your shield or module?");
      Serial.println("Note: press reset button on the board and reopen this Serial Monitor after fixing your issue!");
      while (true);
    }
    
}

void HardwareManager::readSensorData() {

  // Get a new normalized sensor event
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

    sensors_event_t temp_event, pressure_event;
  
  while (!dps.temperatureAvailable() || !dps.pressureAvailable()) {
    return; // wait until there's something to read
  }

  dps.getEvents(&temp_event, &pressure_event);
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa"); 

  Serial.println();

    Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();
}

SensorData& HardwareManager::getSensorData() {
  return m_sensorData;
}

void HardwareManager::logDataPacket() {

}

void HardwareManager::logString(String logString) {

}