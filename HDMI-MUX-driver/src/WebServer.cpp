/*
 * HDMI-MUX: WebServer.cpp
 */

#include "WebServer.hpp"

WebServer::WebServer(MuxDriver& muxDriver, WiFiManager& wifiManager) {
    this->muxDriver = &muxDriver;
    this->wifiManager = &wifiManager;

    this->server.on("/", [this]() {
        char response[32] = "";
        snprintf(response, sizeof(response), "Current port: %d", this->muxDriver->enabledPort());
        this->server.send(200, "text/plain", response);
    });

    this->server.on("/next", [this]() {
        this->muxDriver->enableNextPort();
        char response[64] = "";
        snprintf(response, sizeof(response), "Next port enabled\nCurrent port: %d", this->muxDriver->enabledPort());
        this->server.send(200, "text/plain", response);
    });

    this->server.on("/port1", [this]() {
        this->muxDriver->enablePort1();
        this->server.send(200, "text / plain", "port 1 enabled");
    });

    this->server.on("/port2", [this]() {
        this->muxDriver->enablePort2();
        this->server.send(200, "text / plain", "port 2 enabled");
    });

    this->server.on("/port3", [this]() {
        this->muxDriver->enablePort3();
        this->server.send(200, "text / plain", "port 3 enabled");
    });

    this->server.on("/port4", [this]() {
        this->muxDriver->enablePort4();
        this->server.send(200, "text / plain", "port 4 enabled");
    });

    this->server.on("/reset", [this]() {
        this->server.send(200, "text/plain", "WiFi creds reset and rebooting...");
        this->wifiManager->resetSettings();
        resetFlag = true;
    });
}

void WebServer::begin() {
    this->serverReady = true;
    this->server.begin();
}

void WebServer::handleClient() {
    if (resetFlag) {
        delay(1000);
        ESP.restart();
    }

    if (this->serverReady) {
        this->server.handleClient();
    }
}