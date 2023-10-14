#ifndef HARDWARE_MANAGER_H_
#define HARDWARE_MANAGER_H

#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_DPS310.h>
#include <SD.h>

#include "Arduino.h"

// Define the sensor data struct
// you can change this with more sensors if you want!
typedef struct {
    int timestamp;
    // IMU Data
    double accel_x;
    double accel_y;
    double accel_z;
    double angular_velocity_x;
    double angular_velocity_y;
    double angular_velocity_z;
    double imu_temperature;
    // Barometer Data
    double pressure;
    double temperature;
} SensorData_t;

// Function prototypes
class HardwareManager {
public:
    /**
     * @brief Constructor for HardwareManager.
    */
    HardwareManager() = default;

    /**
     * @brief Initialzes hardware manager and the attached sensors.
    */
    void init();

    /**
     * @brief Reads sensor data from all connected sensors and stores
     *        in internal SensorData_t struct.
    */
    void readSensorData();

   /**
    * @brief Returns the sensor data struct for external access.
    * @return SensorData_t representing the current sensor data.
    */
    SensorData_t &getSensorData();

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
    Adafruit_DPS310 dps;
    Adafruit_LSM6DS3TRC lsm6ds3trc;

    SensorData_t m_sensorData;
};

#endif // HARDWARE_MANAGER_H_
