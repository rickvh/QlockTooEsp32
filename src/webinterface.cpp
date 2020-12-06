#include "webinterface.h"
#include <RemoteDebugger.h> 
#ifdef ESP2866
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

const int timeout = 2000; // timout for connections in milliseconds
unsigned long currentTime = 0;
unsigned long previousTime = 0;         

Webinterface::Webinterface(int port) {
    this->m_port = port;
    this->m_server = WiFiServer(this->m_port);
}

void Webinterface::loop() {
    WiFiClient client = this->m_server.available();

    if (client) {
        // debugI("New client connected");
    }
}
