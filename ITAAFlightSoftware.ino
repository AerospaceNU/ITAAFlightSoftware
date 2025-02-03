#include <movingAvg.h> 

#include "hardware_manager.h"

// Creates an instance of the HardwareManager

HardwareManager hardware_manager;

movingAvg pressure_avg(20);

const double ACCEL_G_EARTH = 9.8; // m/s^2

const double LAUNCH_ACCEL_THRESHOLD = 3 * ACCEL_G_EARTH;
const uint32_t LAUNCH_ACCEL_TIME = 300; // ms

int min_pressure = 10000; // minimum pressure = maximum height

// The time since which we have continuously been above the accel threshold
uint32_t launch_threshold_true_since = 0;

enum states {
  Preflight = 0,
  Ascent,
  Descent,
  Postflight
};

int state;

void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    pressure_avg.begin();
    SPI.begin();
    Wire.begin();
    // Initialize the hardware manager here:
    // TODO: Initialization of hardware manager
    hardware_manager.init();
    state = Preflight;
    Serial.println("Setup finished!");
}

void loop() {
    Serial.println("Looping!");
    // Add code that does things with the hardware manager here:
    // What do we need to do each loop?
    
    // TODO: Read sensor data
    hardware_manager.readSensorData();
    

    const SensorData& sensorData = hardware_manager.getSensorData();

    pressure_avg.reading((int)sensorData.pressure);
    Serial.println(sensorData.pressure);

    // TODO: Log sensor data to SD card

    // TODO: Print out the data here

    switch (state) {
      case Preflight:
        if (abs(sensorData.accel_x) < LAUNCH_ACCEL_THRESHOLD) {
          launch_threshold_true_since = millis();
        }
        if (millis() - launch_threshold_true_since > LAUNCH_ACCEL_TIME) {
          state = Ascent;
        }
        Serial.println("In preflight!");
        break;
      case Ascent:
        min_pressure = min(pressure_avg.getAvg(), min_pressure);
        if (pressure_avg.getAvg() > 30 + min_pressure) {
          state = Descent;
        }

        Serial.println("In ascent!");
        break;
      case Descent:
        if (false) {
          state = Postflight;
        }
        Serial.println("In descent!");
        break;
      case Postflight:
        Serial.println("In postflight!");
        break;
      default:
        // Something is wrong
        break;
    }


    delay(10);
}
