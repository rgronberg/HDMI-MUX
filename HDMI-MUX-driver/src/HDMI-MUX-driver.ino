//HDMI-MUX-driver.ino

#include <ESP8266NetBIOS.h>
#include <WiFiManager.h>

#include "MuxDriver.hpp"
#include "WebServer.hpp"

ESP8266NetBIOS netBIOS;
MuxDriver muxDriver;
WiFiManager wifiManager;
WebServer webServer(muxDriver, wifiManager);

void setup() {
    Serial.begin(115200);
    Serial.println();

    muxDriver.begin(1);

    char hostname[32] = "";
    snprintf(hostname, sizeof(hostname), "hdmimux-%d", ESP.getChipId());

    WiFi.mode(WIFI_STA);
    WiFi.hostname(hostname);
    netBIOS.begin(hostname);

    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setConfigPortalTimeout(120);

    if(wifiManager.autoConnect(hostname, "defaultpassword")){
        // Connected to saved creds, otherwise the portal is run
        webServer.begin();
    }
}

void loop() {
    // Only returns true once, setup webServer when connected
    if (wifiManager.process()) {
        // Connected to AP, enable the webServer
        webServer.begin();
    }

    muxDriver.handleButton();
    webServer.handleClient();
}
