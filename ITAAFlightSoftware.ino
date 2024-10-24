#include "hardware_manager.h"

// Creates an instance of the HardwareManager

HardwareManager hardware_manager;

void setup() {
    Serial.begin(9600);
    while (!Serial) {}
    SPI.begin();
    Wire.begin();
    // Initialize the hardware manager here:


    Serial.println("Setup finished!");
}

void loop() {
    Serial.println("Looping!");
    // Add code that does things with the hardware manager here:
    // What do we need to do each loop?

    delay(1000);
}
