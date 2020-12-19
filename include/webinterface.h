#pragma once

#include "RemoteDebugger.h"
#include <ESPAsyncWebServer.h>
using namespace std;

class Webinterface {
    private:
        // int port;
        // AsyncWebServer &server;
        // AsyncWebSocket &ws;
        // AsyncEventSource &events;
        
        RemoteDebug &Debug;
        void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);

    public:
        Webinterface(int port, RemoteDebug &debug_); // : port(port) { }
        // Webinterface(int port); // : port(port) { }
        // ~Webinterface() { }

        void test(const char* tekst);
};
