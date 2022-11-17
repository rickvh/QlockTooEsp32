#include "webinterface.h"
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include "Arduino.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "clock.h"
#include "control.h"
#include "configservice.h"
#include "buildinfo.h"

using namespace std;

namespace qlocktoo {
Webinterface::Webinterface(int port) : server(AsyncWebServer(port)) {
    Serial.printf("Webinterface started on port: %u\r\n", port);
}

void Webinterface::begin() {
    // Host the static Angular application which should be stored in SPIFFS.
    server.serveStatic("/", SPIFFS, "/qlocktoo-portal").setDefaultFile("index.html");

    // API: Get current clock configuration
    server.on("/api/clock", HTTP_GET, [&](AsyncWebServerRequest *request) {
        Serial.println("Current clock config requested");
        auto config = &ConfigService::CONFIG.clockConfig;
        
        StaticJsonDocument<256> jsonDoc;
        auto colorItIs = jsonDoc.createNestedObject("colorItIs");
        auto colorWords = jsonDoc.createNestedObject("colorWords");
        auto colorHour = jsonDoc.createNestedObject("colorHour");
        colorItIs[KEY_HUE] = config->colorItIs.H;
        colorItIs[KEY_SATURATION] = config->colorItIs.S;
        colorItIs[KEY_VALUE] = config->colorItIs.B;
        colorWords[KEY_HUE] = config->colorWords.H;
        colorWords[KEY_SATURATION] = config->colorWords.S;
        colorWords[KEY_VALUE] = config->colorWords.B;
        colorHour[KEY_HUE] = config->colorHour.H;
        colorHour[KEY_SATURATION] = config->colorHour.S;
        colorHour[KEY_VALUE] = config->colorHour.B;
        
        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    }
    );

    // API: Get current network configuration
    server.on("/api/network", HTTP_GET, [&](AsyncWebServerRequest *request) {
        Serial.println("Current network config requested");
        auto config = &ConfigService::CONFIG.networkConfig;
        
        StaticJsonDocument<256> jsonDoc;
        jsonDoc[KEY_HOSTNAME] = config->hostname;
        jsonDoc[KEY_SSID] = config->ssid;
        jsonDoc[KEY_WIFI_CONNECTED] = ConfigService::connectedToWifi;
        
        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    }
    );

    server.on("/api/status", HTTP_GET, [&](AsyncWebServerRequest *request) {
        Serial.println("Current status requested");
        auto config = &ConfigService::CONFIG.networkConfig;
        
        // TODO: move network status here
        StaticJsonDocument<256> jsonDoc;
        jsonDoc[KEY_VERSION] = BuildInfo::version;

        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    }
    );

    server.onRequestBody(
        [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            // API: Get current clock configuration
            if (request->url() == "/api/clock") {
                StaticJsonDocument<256> jsonDoc;

                if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
                    Serial.println("Updating clock config");
                    auto config = &ConfigService::CONFIG.clockConfig;
                    float h, s, v;

                    auto color = jsonDoc["colorItIs"];
                    h = color[KEY_HUE] | 0.0f;
                    s = color[KEY_SATURATION] | 0.0f;
                    v = color[KEY_VALUE] | 0.0f;
                    config->colorItIs = HsbColor(h, s, v);
                    Serial.printf("HSV colorItIs: %f, %f, %f\r\n", h, s, v);

                    color = jsonDoc["colorWords"];
                    h = color[KEY_HUE] | 0.0f;
                    s = color[KEY_SATURATION] | 0.0f;
                    v = color[KEY_VALUE] | 0.0f;
                    config->colorWords = HsbColor(h, s, v);
                    Serial.printf("HSV colorWords: %f, %f, %f\r\n", h, s, v);

                    color = jsonDoc["colorHour"];
                    h = color[KEY_HUE] | 0.0f;
                    s = color[KEY_SATURATION] | 0.0f;
                    v = color[KEY_VALUE] | 0.0f;
                    config->colorHour = HsbColor(h, s, v);
                    Serial.printf("HSV colorHour: %f, %f, %f\r\n", h, s, v);

                    Serial.println("/api/clock");
                    auto newMode = Mode::Clock;
                    xQueueSend(xChangeAppQueue, &newMode, 0);
                    // xQueueSend(xClockConfigQueue, &config, 0);
                }
                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }

            if (request->url() == "/api/network") {
                StaticJsonDocument<256> jsonDoc;
                if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
                    Serial.println("Updating network config");
                    NetworkConfig networkConfig;
                    strlcpy(networkConfig.hostname, jsonDoc[KEY_HOSTNAME] | "", sizeof(networkConfig.hostname));
                    strlcpy(networkConfig.ssid, jsonDoc[KEY_SSID] | "", sizeof(networkConfig.ssid));
                    strlcpy(networkConfig.password, jsonDoc[KEY_PASSWORD] | "", sizeof(networkConfig.password));
                    xQueueSend(xWifiConfigChangedQueue, &networkConfig, 0);
                }
                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }

            if (request->url() == "/api/swirl") {
                Serial.println("Mode set to SWIRL");
                auto newMode = Mode::Swirl;
                xQueueSend(xChangeAppQueue, &newMode, 0);
                request->send(200, "application/json", "{ \"status\": \"success\" }");
            }
        }
    );

    server.onNotFound([&](AsyncWebServerRequest *request) {
        Serial.println("Not found");
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
}
}