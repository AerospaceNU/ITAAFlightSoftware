#ifndef LSM6DS3TRC_H_
#define LSM6DS3TRC_H_

#include <inttypes.h>

struct IMUData {
    uint16_t accel_x_raw;
    uint16_t accel_y_raw;
    uint16_t accel_z_raw;
    double accel_x_mps;
    double accel_y_mps;
    double accel_z_mps;
    uint16_t gyro_x_raw;
    uint16_t gyro_y_raw;
    uint16_t gyro_z_raw;
    double gyro_x_dps;
    double gyro_y_dps;
    double gyro_z_dps;
};

class LSM6DS3TRC {

    public:
        /**
         * @brief Construct the IMU
         * 
         * @param csPin Chip select pin to use for SPI
         */
        LSM6DS3TRC(const int csPin);

        /**
         * @brief Initialize the IMU
         * 
         * @return boolean status indicating success
         */
        bool init();

        /**
         * @brief Get data from the IMU
         * 
         * @param data Reference to structure to load data into.
         */
        void getData(IMUData& data);


    private:

        /**
         * @brief Write a register to a specific value
         * 
         * @param address Register to write to
         * @param writeValue Value to write to the register
         */
        void writeRegister(const uint8_t address, const uint8_t writeValue);


        /**
         * @brief Read a value from a register
         * 
         * @param address Register to read from
         * 
         * @return Register value
         */
        uint8_t readRegister(const uint8_t address);

      const int m_csPin;


};

#endif // LSM6DS3TRC_H_