/*
 * HDMI-MUX: WebServer.cpp
 */

#include <LittleFS.h>

#include "WebServer.hpp"

WebServer::WebServer(MuxDriver& muxDriver, WiFiManager& wifiManager) {
    this->muxDriver = &muxDriver;
    this->wifiManager = &wifiManager;

    this->handleNotFound();
    this->handleStatic();
    this->handlePortconfig();
    this->handleReset();
}

void WebServer::begin() {
    LittleFS.begin();
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

void WebServer::handleNotFound() {
    this->server.onNotFound([this]() {
        this->server.sendHeader("Location", "/", true);
        this->server.send(302, "text/plane", "");
    });
}

void WebServer::handleStatic() {
    this->server.on("/favicon.ico", [this]() {
        File favicon = LittleFS.open("/favicon.ico", "r");
        if (favicon) {
            this->server.streamFile(favicon, "image/x-icon");
            favicon.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
        }
    });

    this->server.on("/static/main.js", [this]() {
        File mainJs = LittleFS.open("/main.js", "r");
        if (mainJs) {
            this->server.streamFile(mainJs, "application/javascript");
            mainJs.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
        }
    });

    this->server.on("/static/bootstrap.min.css", [this]() {
        File bootstrapCss = LittleFS.open("/bootstrap.min.css.gz", "r");
        if (bootstrapCss) {
            this->server.streamFile(bootstrapCss, "text/css");
            bootstrapCss.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
        }
    });

    this->server.on("/static/bootstrap.min.css.map", [this]() {
        File bootstrapMap = LittleFS.open("/bootstrap.min.css.map.gz", "r");
        if (bootstrapMap) {
            this->server.streamFile(bootstrapMap, "text/json");
            bootstrapMap.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
        }
    });

    this->server.on("/", [this]() {
        File indexHtml = LittleFS.open("/index.html", "r");
        if (indexHtml) {
            Serial.println("sending the index");
            this->server.streamFile(indexHtml, "text/html");
            indexHtml.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
       }
    });

}

void WebServer::handlePortconfig() {
    this->server.on("/selected", [this]() {
        char response[4] = "";
        snprintf(response, sizeof(response), "%d", this->muxDriver->enabledPort());
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
}

void WebServer::handleReset() {
    this->server.on("/reset", [this]() {
        File indexHtml = LittleFS.open("/reset.html", "r");
        if (indexHtml) {
            this->server.streamFile(indexHtml, "text/html");
            indexHtml.close();
        }
        else {
            this->server.send(503, "text/plain", "server error");
       }
    });

    this->server.on("/reallyreset", [this]() {
        this->server.send(200, "text/plain", "WiFi creds reset and rebooting...");
        this->wifiManager->resetSettings();
        resetFlag = true;
    });
}
