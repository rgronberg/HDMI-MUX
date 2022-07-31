/*
 * HDMI-MUX: MuxDriver.cpp
 */

#include <EEPROM.h>

#include "MuxDriver.hpp"

void MuxDriver::begin() {
    int initialPort;

    // Set the pin modes for the pins used in the class
    pinMode(buttonPin, INPUT);
    pinMode(muxA_enable, OUTPUT);
    pinMode(muxA_select, OUTPUT);
    pinMode(muxB_enable, OUTPUT);
    pinMode(muxB_select, OUTPUT);
    pinMode(port1LED, OUTPUT);
    pinMode(port2LED, OUTPUT);
    pinMode(port3LED, OUTPUT);
    pinMode(port4LED, OUTPUT);

    EEPROM.begin(16);

    // Set initial state, from EEPROM if valid value
    EEPROM.get(0, initialPort);
    if (0 < initialPort && initialPort <= numPorts) {
        enablePort(initialPort);
    }
    else {
        enablePort(1);
    }
}

void MuxDriver::handleButton() {
    // read the state of the switch into a local variable:
    int reading = digitalRead(buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != lastButtonState) {
        // reset the debouncing timer
        lastDebounceTime = millis();
    }

    // Long button press should cause settings reset
    if ((millis() - lastDebounceTime) > longPress &&
            buttonState == LOW &&
            !_shouldReset) {
        _shouldReset = true;
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != buttonState) {
            buttonState = reading;

            // Only toggle the LED if the new button state is HIGH
            // This performs the action when the button is unpressed
            if (buttonState == HIGH) {
                // Enable the next port
                enableNextPort();
            }
        }
    }

    // save the reading for the next time through the loop
    lastButtonState = reading;
}

void MuxDriver::handleSettings() {
    if (millis() - lastStoreTime > saveTime) {
        lastStoreTime = millis();
        saveSettings();
    }
}

void MuxDriver::saveSettings() {
    EEPROM.put(0, enabledPort());
    EEPROM.commit();
}

void MuxDriver::clearEnabledPort() {
    digitalWrite(muxA_enable, LOW);
    digitalWrite(muxA_select, LOW);
    digitalWrite(muxB_enable, LOW);
    digitalWrite(muxB_select, LOW);
    digitalWrite(port1LED, LOW);
    digitalWrite(port2LED, LOW);
    digitalWrite(port3LED, LOW);
    digitalWrite(port4LED, LOW);
}

void MuxDriver::enablePort1() {
    clearEnabledPort();
    digitalWrite(muxA_enable, HIGH);
    digitalWrite(muxA_select, LOW);
    digitalWrite(port1LED, HIGH);
    currentPortIndex = 0;
}

void MuxDriver::enablePort2() {
    clearEnabledPort();
    digitalWrite(muxA_enable, HIGH);
    digitalWrite(muxA_select, HIGH);
    digitalWrite(port2LED, HIGH);
    currentPortIndex = 1;
}

void MuxDriver::enablePort3() {
    clearEnabledPort();
    digitalWrite(muxB_enable, HIGH);
    digitalWrite(muxB_select, LOW);
    digitalWrite(port3LED, HIGH);
    currentPortIndex = 2;
}

void MuxDriver::enablePort4() {
    clearEnabledPort();
    digitalWrite(muxB_enable, HIGH);
    digitalWrite(muxB_select, HIGH);
    digitalWrite(port4LED, HIGH);
    currentPortIndex = 3;
}

void MuxDriver::enablePort(int port /* one-based */) {
    if (0 < port && port <= numPorts) {
        currentPortIndex = port - 1;
        (this->*(enableFunctions[currentPortIndex]))();
    }
}

void MuxDriver::enableNextPort() {
    int newPortOneBased = ((currentPortIndex + 1) % numPorts) + 1;
    enablePort(newPortOneBased);
}

int MuxDriver::enabledPort() {
    // Return value is one-based
    return currentPortIndex + 1;
}

bool MuxDriver::shouldReset() {
    return _shouldReset;
}

void MuxDriver::resetSettings() {
    currentPortIndex = 0;
    saveSettings();
}
