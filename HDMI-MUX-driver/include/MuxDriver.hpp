/*
 * HDMI-MUX: MuxDriver.hpp
 */

#ifndef __MUXDRIVER_HPP__
#define __MUXDRIVER_HPP__

#include <Arduino.h>

// Forward definition of MuxDriver class for use in EnableFuncPtr
class MuxDriver;
typedef void (MuxDriver::*EnableFuncPtr)();

class MuxDriver
{
    private:
        /***** Member constants *****/
        const unsigned long debounceDelay = 50; // ms
        const int numPorts = 4;                 // Number of ports in the mux
        const EnableFuncPtr enableFunctions[4] = { &MuxDriver::enablePort1,
                                                   &MuxDriver::enablePort2,
                                                   &MuxDriver::enablePort3,
                                                   &MuxDriver::enablePort4 };

        /*** Inputs ***/
        const int buttonPin = 16;

        /*** Outputs ***/
        // The following enable/select pins are low until the code takes them high.
        const int muxA_enable = 5;
        const int muxA_select = 12;
        const int muxB_enable = 13;
        const int muxB_select = 14;

        // GPIO 0, 2, and 15 are used for boot functions and are not used to drive the MUX logic.
        const int port1LED = 0;
        const int port2LED = 2;
        const int port3LED = 4;
        const int port4LED = 15;

        /***** Member variables *****/
        unsigned long lastDebounceTime = 0; // the last time the input pin was toggled
        int buttonState = HIGH;             // the current reading from the input pin
        int lastButtonState = HIGH;         // the previous reading from the input pin
        int currentPortIndex = 0;           // the currently sellected mux port (zero based)

        /***** Internal methods *****/
        void clearEnabledPort();

    public:
        /***** Setup and loop functions *****/
        void begin(int initialPort);
        void handleButton();

        /***** State management functions *****/
        void enablePort1();
        void enablePort2();
        void enablePort3();
        void enablePort4();
        void enablePort(int port);  // One-based port index
        void enableNextPort();
        int enabledPort();          // One-based port index
};

#endif  // __MUXDRIVER_HPP__
