#pragma once

#include <ESPAsyncWebServer.h>

#include "RemoteDebugger.h"
#include "color.h"
using namespace std;

extern QueueHandle_t xChangeAppQueue;
extern QueueHandle_t xClockConfigQueue;

namespace qlocktoo {
class Webinterface {
   private:
    const char *PARAM_MESSAGE = "message";
    const int timeout = 2000;  // timout for connections in milliseconds
    // int port;
    // AsyncWebServer &server;
    // AsyncWebSocket &ws;
    // AsyncEventSource &events;
    bool testswitch = false;

    RemoteDebug &Debug;
    AsyncWebServer server;
    // auto ws = AsyncWebSocket("/ws");
    // auto event = AsyncEventSource("/events");

    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

   public:
    Webinterface(int port, RemoteDebug &debug_);  // : port(port) { }
    // Webinterface(int port); // : port(port) { }
    // ~Webinterface() { }

    void test(const char *tekst);
    void begin();
};
}