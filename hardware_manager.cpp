#include "hardware_manager.h"

#include <Wire.h>
#include <SPI.h>

File root;
int currentFlightNum;

/**
  * @brief Gets a random scalar multiplier to simulate noise
  * @param noisePercent Percent of noise, i.e. 0.01 = 1% maximum deviation
  * @return Random noise scalar as a float.
*/
float random_scalar(float noisePercent = 0.0005) {
  return ((float)random(200000 * (1.0-noisePercent), 200000 * (1.0+noisePercent)) / (float)200000); 
}

/**
  * @brief Simulated acceleration values
  * @param t Timestamp in milliseconds
  * @return simulated acceleration in m/s^2.
*/
float test_accelll(uint32_t t) {
  if (t < 5000) {
    return 0;
  } else if (t < 8000) {
    return 40;
  } else {
    return 0;
  }
}

/**
  * @brief Simulated pressure values
  * @param t Timestamp in milliseconds
  * @return simulated pressure in atmospheres.
*/
float test_pressureeee(uint32_t t) {
  float scalar = random_scalar();
  if (t < 5000) {
    return scalar * 1;
  } else if (t < 55000) {
    return scalar * pow((t - 30000.0) / 90000.0, 2) + 0.923;
  } else {
    return scalar * 1;
  }
}

// Can also use SPI!
const int SPI_RX = 4;
const int SPI_TX = 3;
const int SPI_SCK = 2;
const int DPS310_CS = 5;
const int SD_CS = 17;

const int I2C_SDA = 0;
const int I2C_SCL = 1;

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (!entry.isDirectory()) {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

int countExistingLogFiles(File dir) {
  int count = 0;
  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      return count;
    }
    if (!entry.isDirectory()) {
      ++count;
    }
    entry.close();
  }
}


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
    root = SD.open("/");
    printDirectory(root, 0);
    currentFlightNum = countExistingLogFiles(root);
    root.close();
}



void HardwareManager::readSensorData() {
  m_sensorData.timestamp = millis();
  sensors_event_t temp_event, pressure_event;
  dps.getEvents(&temp_event, &pressure_event);
  m_sensorData.temperature = temp_event.temperature;
  m_sensorData.pressure = pressure_event.pressure * 0.000987;
  // m_sensorData.pressure = test_pressure(millis());

  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  m_sensorData.accel_x = accel.acceleration.x;
  //m_sensorData.accel_x = test_accel(millis());
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
  File log_file = SD.open((String("flight_log_") + String(currentFlightNum) + String(".txt")).c_str(), FILE_WRITE);
  log_file.print(m_sensorData.timestamp);
  log_file.print(",");
  log_file.print(m_sensorData.accel_x, 7);
  log_file.print(",");
  log_file.print(m_sensorData.accel_y, 7);
  log_file.print(",");
  log_file.print(m_sensorData.accel_z, 7);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_x, 7);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_y, 7);
  log_file.print(",");
  log_file.print(m_sensorData.angular_velocity_z, 7);
  log_file.print(",");
  log_file.print(m_sensorData.imu_temperature, 7);
  log_file.print(",");
  log_file.print(m_sensorData.pressure, 7);
  log_file.print(",");
  log_file.print(m_sensorData.temperature, 7);
  log_file.println();
  log_file.close();
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