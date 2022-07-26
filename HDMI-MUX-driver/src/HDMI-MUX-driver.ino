//HDMI-MUX-driver.ino

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

#include "MuxDriver.hpp"

MuxDriver muxDriver;

ESP8266WebServer server(80);

void setup() {
    Serial.begin(115200);

    // char hostname[32] = "";
    // snprintf(hostname, sizeof(hostname), "hdmimux-%d", ESP.getChipId());

    WiFi.mode(WIFI_STA); // Default to station to attempt to connect to saved credentials
    WiFi.hostname("hdmimux");
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point and then goes into a blocking loop awaiting
    // configuration and will return success result
    bool res;
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // TODO: Handle failure better
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }

    server.on("/", []() {
        server.send(200, "text/plain", "portal setup success");
    });

    server.on("/reset", []() {
        server.send(200, "text/plain", "WiFi creds reset and rebooting...");
        WiFi.persistent(true);
        WiFi.disconnect(true);
        WiFi.persistent(false);
        delay(1000);
        ESP.restart();      
    });

    server.begin();
}

void loop() {
  server.handleClient();
}


// #include <EEPROM.h>

// #include "ESP8266WiFi.h"
// #include "ESP8266WebServer.h"
 
// ESP8266WebServer server(80);

// // constants won't change. They're used here to set pin numbers:

// // GPIO 0, 2, and 15 are used for boot functions and are not used to drive the MUX logic.
// // They are allways low until the code takes them high.

// const int muxA_enable = 5;
// const int muxA_select = 12;
// const int muxB_enable = 13;
// const int muxB_select = 14;

// const int port1LED = 0;
// const int port2LED = 2;
// const int port3LED = 4;
// const int port4LED = 15;

// const int buttonPin = 16;    // the number of the pushbutton pin
// const int ledPins[] = {0,2,4,5,12,13,14,15};
// const int numLeds = 8;

// // Variables will change:
// int ledState = HIGH;         // the current state of the output pin
// int currentLedIndex = 0;
// int buttonState = HIGH;       // the current reading from the input pin
// int lastButtonState = HIGH;   // the previous reading from the input pin

// // the following variables are unsigned longs because the time, measured in
// // milliseconds, will quickly become a bigger number than can be stored in an int.
// unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
// unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// void writeLED() {
//   Serial.print("Writing GPIO ");
//   Serial.print(ledPins[currentLedIndex]);
//   Serial.println(" to HIGH");
//   for (int pinIndex = 0; pinIndex < numLeds; pinIndex++) {
//     digitalWrite(ledPins[pinIndex], LOW);
//   }
//   digitalWrite(ledPins[currentLedIndex], HIGH);
// }

// void clearEnabledPort() {
//   digitalWrite(muxA_enable, LOW);
//   digitalWrite(muxA_select, LOW);
//   digitalWrite(muxB_enable, LOW);
//   digitalWrite(muxB_select, LOW);
//   digitalWrite(port1LED, LOW);
//   digitalWrite(port2LED, LOW);
//   digitalWrite(port3LED, LOW);
//   digitalWrite(port4LED, LOW);
// }

// void enablePort1() {
//   clearEnabledPort();
//   digitalWrite(muxA_enable, HIGH);
//   digitalWrite(muxA_select, LOW);
//   digitalWrite(port1LED, HIGH);
// }

// void enablePort2() {
//   clearEnabledPort();
//   digitalWrite(muxA_enable, HIGH);
//   digitalWrite(muxA_select, HIGH);
//   digitalWrite(port2LED, HIGH);
// }

// void enablePort3() {
//   clearEnabledPort();
//   digitalWrite(muxB_enable, HIGH);
//   digitalWrite(muxB_select, LOW);
//   digitalWrite(port3LED, HIGH);
// }

// void enablePort4() {
//   clearEnabledPort();
//   digitalWrite(muxB_enable, HIGH);
//   digitalWrite(muxB_select, HIGH);
//   digitalWrite(port4LED, HIGH);
// }

// int currentFuncIndex = 0;
// void (*const enableFunctions[]) () = {enablePort1, enablePort2, enablePort3, enablePort4};
// const int numFuncs = 4;

// void setup() {
//   Serial.begin(115200);
//   Serial.println("\nhere we go...");

//   EEPROM.begin(512);
  
//   pinMode(buttonPin, INPUT);
//   // for (int pinIndex = 0; pinIndex < numLeds; pinIndex++) {
//   //   pinMode(ledPins[pinIndex], OUTPUT);
//   // }
//   // writeLED();
//   pinMode(muxA_enable, OUTPUT);
//   pinMode(muxA_select, OUTPUT);
//   pinMode(muxB_enable, OUTPUT);
//   pinMode(muxB_select, OUTPUT);
//   pinMode(port1LED, OUTPUT);
//   pinMode(port2LED, OUTPUT);
//   pinMode(port3LED, OUTPUT);
//   pinMode(port4LED, OUTPUT);

//   char ssid[16];
//   char pass[16];
//   EEPROM.get(0, ssid);
//   EEPROM.get(16, pass);

//   WiFi.hostname("hdmimux");
//   WiFi.begin("outsideLAN2.4", "tickettoride");  //Connect to the WiFi network
 
//   while (WiFi.status() != WL_CONNECTED) {  //Wait for connection
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println(".");
 
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());  //Print the local IP

//   int eepromFuncIndex = 0;
//   EEPROM.get(255, eepromFuncIndex);
//   Serial.print("EEPROM got "); Serial.println(eepromFuncIndex);

//   if (0 <= eepromFuncIndex && eepromFuncIndex < 4) {
//     currentFuncIndex = eepromFuncIndex;
//   }

//   (*enableFunctions[currentFuncIndex])();

//   server.on("/", []() {
//     server.send(200, "text / plain", "This should show which port is enabled...");
//   });

//   server.on("/port1", []() {
//     enablePort1();
//     server.send(200, "text / plain", "port 1 enabled");
//   });

//   server.on("/port2", []() {
//     enablePort2();
//     server.send(200, "text / plain", "port 2 enabled");
//   });

//   server.on("/port3", []() {
//     enablePort3();
//     server.send(200, "text / plain", "port 3 enabled");
//   });

//   server.on("/port4", []() {
//     enablePort4();
//     server.send(200, "text / plain", "port 4 enabled");
//   });

//   server.begin();                    //Start the server
//   Serial.println("Server listening");
// }

// void loop() {
//   // read the state of the switch into a local variable:
//   int reading = digitalRead(buttonPin);

//   // check to see if you just pressed the button
//   // (i.e. the input went from LOW to HIGH), and you've waited long enough
//   // since the last press to ignore any noise:

//   // If the switch changed, due to noise or pressing:
//   if (reading != lastButtonState) {
//     // reset the debouncing timer
//     lastDebounceTime = millis();
//   }

//   if ((millis() - lastDebounceTime) > debounceDelay) {
//     // whatever the reading is at, it's been there for longer than the debounce
//     // delay, so take it as the actual current state:

//     // if the button state has changed:
//     if (reading != buttonState) {
//       buttonState = reading;

//       // only toggle the LED if the new button state is HIGH
//       if (buttonState == HIGH) {
//         // currentLedIndex = (currentLedIndex + 1) % numLeds;
//         // set the LED:
//         // writeLED();
//         int testval = 0;
//         Serial.print("cur EEPROM func index "); Serial.println(EEPROM.get(255, testval));
//         currentFuncIndex = (currentFuncIndex + 1) % numFuncs;
//         (*enableFunctions[currentFuncIndex])();
//         Serial.print("write func index "); Serial.println(currentFuncIndex);
//         EEPROM.put(255, currentFuncIndex);
//         // EEPROM.commit(); // TODO: Commit the EEPROM once an hour or something
//         //                  // Maybe an hour after last change time? Easy with millis
//       }
//     }
//   }

//   // save the reading. Next time through the loop, it'll be the lastButtonState:
//   lastButtonState = reading;

//   server.handleClient();         //Handling of incoming requests
// }

