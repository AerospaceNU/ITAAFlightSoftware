#include "hardware_manager.h"

enum states {
  PREFLIGHT,
  ASCENT,
  DESCENT,
  POSTFLIGHT
};

int state;

// How many times a state has run
int state_counter = 0;

// Creates an instance of the HardwareManager
HardwareManager hardware_manager;

// The threshold of acceleration that we want to see
const int launch_accel_threshold = 30; // m/s^2
// The time for which we need to continuously see acceleration above the threshold
const int launch_accel_time = 300; // ms of sustained acceleration
// The first time (milliseconds) that we have started being above our threshold
uint32_t accel_threshold_true_since;

// How fast we loop
const int delay_period_ms = 15;
int last_time_ms;

void setup() {
    state = PREFLIGHT;
    Serial.begin(9600);
    while (!Serial) {}
    delay(1000);
    // Initialize the hardware manager here:
    hardware_manager.init();
    accel_threshold_true_since = millis();
}

void loop() {
    if (millis() - last_time_ms < delay_period_ms) {
      return;
    }
    last_time_ms = millis();
    // Add code that does things with the hardware manager here:
    // What do we need to do each loop?
    hardware_manager.readSensorData();
    hardware_manager.logDataPacket();
    SensorData_t data = hardware_manager.getSensorData();
    switch(state) {
      case PREFLIGHT:
        //Serial.println("Preflight!");
        if (!(abs(data.accel_x) > launch_accel_threshold)) {
          accel_threshold_true_since = millis(); // it has been 0 ms since we started being in the valid acceleration state
        }
        if (millis() - accel_threshold_true_since > launch_accel_time) {
          state = ASCENT;
        }
        if (data.pressure < 900) {
          state = ASCENT;
        }
        break;
      case ASCENT:
        Serial.println("Ascent!");
        break;
      case DESCENT:
        Serial.println("Descent!");       
        break;
      case POSTFLIGHT:
        Serial.println("Postflight!");
        break;
      default:
        break;
    }



}
