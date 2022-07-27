/*
 * HDMI-MUX: MuxDriver.cpp
 */

#include "MuxDriver.hpp"

void MuxDriver::begin(int initialPort /* one-based */) {
    // Set the pin modes for the pins used in the class
    pinMode(this->buttonPin, INPUT);
    pinMode(this->muxA_enable, OUTPUT);
    pinMode(this->muxA_select, OUTPUT);
    pinMode(this->muxB_enable, OUTPUT);
    pinMode(this->muxB_select, OUTPUT);
    pinMode(this->port1LED, OUTPUT);
    pinMode(this->port2LED, OUTPUT);
    pinMode(this->port3LED, OUTPUT);
    pinMode(this->port4LED, OUTPUT);

    // Set initial state
    this->enablePort(initialPort);
}

void MuxDriver::handleButton() {
    // read the state of the switch into a local variable:
    int reading = digitalRead(this->buttonPin);

    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited long enough
    // since the last press to ignore any noise:

    // If the switch changed, due to noise or pressing:
    if (reading != this->lastButtonState) {
        // reset the debouncing timer
        this->lastDebounceTime = millis();
    }

    if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
        // whatever the reading is at, it's been there for longer than the debounce
        // delay, so take it as the actual current state:

        // if the button state has changed:
        if (reading != this->buttonState) {
            this->buttonState = reading;

            // Only toggle the LED if the new button state is HIGH
            // This performs the action when the button is unpressed
            if (this->buttonState == HIGH) {
                // Enable the next port
                this->enableNextPort();
            }
        }
    }

    // save the reading for the next time through the loop
    this->lastButtonState = reading;
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
    this->clearEnabledPort();
    digitalWrite(muxA_enable, HIGH);
    digitalWrite(muxA_select, LOW);
    digitalWrite(port1LED, HIGH);
}

void MuxDriver::enablePort2() {
    this->clearEnabledPort();
    digitalWrite(muxA_enable, HIGH);
    digitalWrite(muxA_select, HIGH);
    digitalWrite(port2LED, HIGH);
}

void MuxDriver::enablePort3() {
    this->clearEnabledPort();
    digitalWrite(muxB_enable, HIGH);
    digitalWrite(muxB_select, LOW);
    digitalWrite(port3LED, HIGH);
}

void MuxDriver::enablePort4() {
    this->clearEnabledPort();
    digitalWrite(muxB_enable, HIGH);
    digitalWrite(muxB_select, HIGH);
    digitalWrite(port4LED, HIGH);
}

void MuxDriver::enablePort(int port /* one-based */) {
    if (0 < port && port <= this->numPorts) {
        this->currentPortIndex = port - 1;
        (this->*(this->enableFunctions[this->currentPortIndex]))();
    }
}

void MuxDriver::enableNextPort() {
    int newPortOneBased = ((this->currentPortIndex + 1) % this->numPorts) + 1;
    this->enablePort(newPortOneBased);
}

int MuxDriver::enabledPort() {
    // Return value is one-based
    return this->currentPortIndex + 1;
}
