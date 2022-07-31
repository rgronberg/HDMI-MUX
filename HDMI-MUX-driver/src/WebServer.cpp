/*
 * HDMI-MUX: WebServer.cpp
 */

#include <LittleFS.h>

#include "WebServer.hpp"

WebServer::WebServer(MuxDriver& muxDriver, WiFiManager& wifiManager) {
    this->muxDriver = &muxDriver;
    this->wifiManager = &wifiManager;

    handleNotFound();
    handleStatic();
    handlePortconfig();
    handleReset();
}

void WebServer::begin() {
    LittleFS.begin();
    serverReady = true;
    server.begin();
}

void WebServer::handleClient() {
    if (resetFlag || muxDriver->shouldReset()) { // or muxDriver calls for a reset
        wifiManager->resetSettings();
        muxDriver->resetSettings();
        delay(1000);
        ESP.restart();
    }

    if (serverReady) {
        server.handleClient();
    }
}

void WebServer::handleNotFound() {
    server.onNotFound([this]() {
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plane", "");
    });
}

void WebServer::handleStatic() {
    server.on("/favicon.ico", [this]() {
        File favicon = LittleFS.open("/favicon.ico", "r");
        if (favicon) {
            server.streamFile(favicon, "image/x-icon");
            favicon.close();
        }
        else {
            server.send(503, "text/plain", "server error");
        }
    });

    server.on("/static/main.js", [this]() {
        File mainJs = LittleFS.open("/main.js", "r");
        if (mainJs) {
            server.streamFile(mainJs, "application/javascript");
            mainJs.close();
        }
        else {
            server.send(503, "text/plain", "server error");
        }
    });

    server.on("/static/bootstrap.min.css", [this]() {
        File bootstrapCss = LittleFS.open("/bootstrap.min.css.gz", "r");
        if (bootstrapCss) {
            server.streamFile(bootstrapCss, "text/css");
            bootstrapCss.close();
        }
        else {
            server.send(503, "text/plain", "server error");
        }
    });

    server.on("/static/bootstrap.min.css.map", [this]() {
        File bootstrapMap = LittleFS.open("/bootstrap.min.css.map.gz", "r");
        if (bootstrapMap) {
            server.streamFile(bootstrapMap, "text/json");
            bootstrapMap.close();
        }
        else {
            server.send(503, "text/plain", "server error");
        }
    });


}

void WebServer::handlePortconfig() {
    server.on("/", [this]() {
        File indexHtml = LittleFS.open("/index.html", "r");
        if (indexHtml) {
            server.streamFile(indexHtml, "text/html");
            indexHtml.close();
        }
        else {
            server.send(503, "text/plain", "server error");
       }
    });

    server.on("/selected", [this]() {
        char response[4] = "";
        snprintf(response, sizeof(response), "%d", muxDriver->enabledPort());
        server.send(200, "text/plain", response);
    });

    server.on("/port1", [this]() {
        muxDriver->enablePort1();
        server.send(200, "text / plain", "port 1 enabled");
    });

    server.on("/port2", [this]() {
        muxDriver->enablePort2();
        server.send(200, "text / plain", "port 2 enabled");
    });

    server.on("/port3", [this]() {
        muxDriver->enablePort3();
        server.send(200, "text / plain", "port 3 enabled");
    });

    server.on("/port4", [this]() {
        muxDriver->enablePort4();
        server.send(200, "text / plain", "port 4 enabled");
    });

    server.on("/next", [this]() {
        muxDriver->enableNextPort();
        char response[64] = "";
        snprintf(response, sizeof(response), "Next port enabled\nCurrent port: %d", muxDriver->enabledPort());
        server.send(200, "text/plain", response);
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
        resetFlag = true;
    });
}
