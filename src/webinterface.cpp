#include "webinterface.h"
#ifdef ESP2866
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

Webinterface::Webinterface(int port) {
    this->m_port = port;

    WiFiServer server(this->m_port);
}
