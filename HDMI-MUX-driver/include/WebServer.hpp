/*
 * HDMI-MUX: WebServer.hpp
 */

#ifndef __WEBSERVER_HPP__
#define __WEBSERVER_HPP__

#include <WiFiManager.h>

#include "MuxDriver.hpp"

class WebServer
{
    private:
        bool serverReady = false;
        int resetFlag = false;
        ESP8266WebServer server;
        MuxDriver *muxDriver;
        WiFiManager *wifiManager;

        void handleNotFound();
        void handleStatic();
        void handlePortconfig();
        void handleReset();

    public:
        WebServer(MuxDriver &muxDriver, WiFiManager &wifiManager);
        void begin();
        void handleClient();
};

#endif // __WEBSERVER_HPP__
