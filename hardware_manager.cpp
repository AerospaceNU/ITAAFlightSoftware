#include "hardware_manager.h"

#include <Wire.h>
#include <SPI.h>

// Function stubs for everything in hardware_manager.h (just so the project will compile)
// These will all need to be created to do something!


// Can also use SPI!
const int SPI_RX = 4;
const int SPI_TX = 3;
const int SPI_SCK = 2;
const int DPS310_CS = 5;
const int SD_CS = 17;

const int I2C_SDA = 0;
const int I2C_SCL = 1;

void HardwareManager::init() {
    Wire.setSDA(I2C_SDA);
    Wire.setSCL(I2C_SCL);
    
    SPI.setRX(SPI_RX);
    SPI.setTX(SPI_TX);
    SPI.setSCK(SPI_SCK);

    // Initialize the DPS310!
    if (!dps.begin_SPI(DPS310_CS)) {  // If you want to use SPI
      Serial.println("Failed to find DPS");
    } else {
      Serial.println("DPS OK!");
      dps.configurePressure(DPS310_64HZ, DPS310_64SAMPLES);
      dps.configureTemperature(DPS310_64HZ, DPS310_64SAMPLES);
    }

    // Initialize the LSM6DS!
    if (!lsm6ds3trc.begin_I2C()) {
        Serial.println("Failed to find LSM6DS3TR-C chip");
    } else {
      Serial.println("LSM6DS3TR initialized!");
      lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
      lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2
    }

    // Initialize the SD card!
    if (!SD.begin(SD_CS)) {
      Serial.println("SD card initialization failed!");
    } else {
      Serial.println("SD card initialized!");
    }
}

void HardwareManager::readSensorData() {
  m_sensorData.timestamp = millis();
  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);
  m_sensorData.temperature = temp_event.temperature;
  m_sensorData.pressure = pressure_event.pressure;

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  m_sensorData.accel_x = accel.acceleration.x;
  m_sensorData.accel_y = accel.acceleration.y;
  m_sensorData.accel_z = accel.acceleration.z;

  m_sensorData.angular_velocity_x = gyro.gyro.x;
  m_sensorData.angular_velocity_y = gyro.gyro.y;
  m_sensorData.angular_velocity_z = gyro.gyro.z;
  m_sensorData.imu_temperature = temp.temperature;
}

SensorData_t& HardwareManager::getSensorData() {
  return m_sensorData;
}

void HardwareManager::logDataPacket() {
  File log_file = SD.open("data_log.txt", FILE_WRITE);
  log_file.print(m_sensorData.timestamp);
  log_file.print(",");
  log_file.print(m_sensorData.accel_x);
  log_file.print(",");
  log_file.print(m_sensorData.accel_y);
  log_file.print(",");
  log_file.print(m_sensorData.accel_z);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_x);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_y);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_z);
  log_file.print(",");
  log_file.print(m_sensorData.imu_temperature);
  log_file.print(",");
  log_file.print(m_sensorData.pressure);
  log_file.print(",");
  log_file.print(m_sensorData.temperature);
  log_file.println();
  log_file.close();


  Serial.print(m_sensorData.timestamp);
  Serial.print(",");
  Serial.print(m_sensorData.accel_x);
  Serial.print(",");
  Serial.print(m_sensorData.accel_y);
  Serial.print(",");
  Serial.print(m_sensorData.accel_z);
  Serial.print(",");
  Serial.print(m_sensorData.angular_velocity_x);
  Serial.print(",");
  Serial.print(m_sensorData.angular_velocity_y);
  Serial.print(",");
  Serial.print(m_sensorData.angular_velocity_z);
  Serial.print(",");
  Serial.print(m_sensorData.imu_temperature);
  Serial.print(",");
  Serial.print(m_sensorData.pressure);
  Serial.print(",");
  Serial.print(m_sensorData.temperature);
  Serial.println();
}

void HardwareManager::logString(String logString) {
  File log_file = SD.open("message_log.txt", FILE_WRITE);
  log_file.print(m_sensorData.timestamp);
  log_file.print(": ");
  log_file.print(logString);
  log_file.println();
  log_file.close();  
  Serial.print(m_sensorData.timestamp);
  Serial.print(": ");
  Serial.print(logString);
  Serial.println();
}