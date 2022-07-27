//HDMI-MUX-driver.ino

#include <WiFiManager.h>

#include "MuxDriver.hpp"

MuxDriver muxDriver;

void setup() {
    Serial.begin(115200);
    Serial.println();

    muxDriver.begin(1);
}

void loop() {
    muxDriver.handleButton();
}
