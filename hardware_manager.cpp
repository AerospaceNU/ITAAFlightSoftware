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

    


    // Initialize the LSM6DS!




    // Initialize the SD card!

    
}

void HardwareManager::readSensorData() {

}

SensorData_t& HardwareManager::getSensorData() {
  return m_sensorData;
}

void HardwareManager::logDataPacket() {

}

void HardwareManager::logString(String logString) {

}