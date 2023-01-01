#pragma once

#include <ESPAsyncWebServer.h>

using namespace std;

extern QueueHandle_t xChangeAppQueue;
extern QueueHandle_t xWifiConfigChangedQueue;

namespace qlocktoo {
class Webinterface {
   private:
    static constexpr const char* LOG_TAG = "webinterface";
    const int timeout = 2000;  // timout for connections in milliseconds
    const char *KEY_HUE = "h";
    const char *KEY_SATURATION = "s";
    const char *KEY_VALUE = "v";

    const char *KEY_HOSTNAME = "hostname";
    const char *KEY_SSID = "ssid";
    const char *KEY_PASSWORD = "password";
    const char *KEY_WIFI_CONNECTED = "connectedToWifi";

    const char *KEY_VERSION = "version";

    AsyncWebServer server;
    
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

   public:
    Webinterface(int port);
    void begin();
};
}