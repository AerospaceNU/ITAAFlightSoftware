#include <Arduino.h>
#include <bluefruit.h>
#include <SPI.h>

#include "lsm6ds3trc.h"

LSM6DS3TRC::LSM6DS3TRC(const int csPin): m_csPin{csPin} {
}

bool LSM6DS3TRC::init() {
    return false;
}

void LSM6DS3TRC::getData(IMUData& data) {

}


void LSM6DS3TRC::writeRegister(const uint8_t address, const uint8_t writeValue) {

}

uint8_t LSM6DS3TRC::readRegister(const uint8_t address) {

    return 0;
}