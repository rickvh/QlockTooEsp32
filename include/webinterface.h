#pragma once

#include <WiFiServer.h>

class Webinterface {
    private:
        int m_port;
        WiFiServer m_server;

    public:
        Webinterface(int port);
        void loop();
};
