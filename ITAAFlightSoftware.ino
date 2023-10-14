#include "hardware_manager.h"

// Creates an instance of the HardwareManager

HardwareManager hardware_manager;

const int delay_period_ms = 10;
int last_time_ms;

void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    delay(3000);
    // Initialize the hardware manager here:
    hardware_manager.init();
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
    hardware_manager.logString("Hello!");

}
