#ifndef HARDWARE_MANAGER_H_
#define HARDWARE_MANAGER_H

#include <Arduino.h>
#include <bluefruit.h>
#include <Wire.h>
#include <SPI.h>
#include "lsm6ds3trc.h"


// Define the sensor data struct
// you can change this with more sensors if you want!
struct SensorData {
    // IMU Data
    double accel_x;
    double accel_y;
    double accel_z;
    double angular_velocity_x;
    double angular_velocity_y;
    double angular_velocity_z;
    // Barometer Data
    double pressure;
    double temperature;
};

// Function prototypes
class HardwareManager {
public:
    /**
     * @brief Constructor for HardwareManager.
    */
    HardwareManager() {}

    /**
     * @brief Initialzes hardware manager and the attached sensors.
    */
    void init();

    /**
     * @brief Reads sensor data from all connected sensors and stores
     *        in internal SensorData struct.
    */
    void readSensorData();

   /**
    * @brief Returns the sensor data struct for external access.
    * @return SensorData representing the current sensor data.
    */
    SensorData& getSensorData();

    /**
     * @brief Logs the currently stored data packet to the SD card.
    */
    void logDataPacket();

    /**
     * @brief Logs the given string to the SD card.
     * @param logString the string to log.
    */
    void logString(String logString);

private:
    SensorData m_sensorData;
};

#endif // HARDWARE_MANAGER_H_
