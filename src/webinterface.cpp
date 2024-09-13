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
#include "apps/clock.h"
#include "configservice.h"
#include "buildinfo.h"

using namespace std;
using namespace qlocktoo;

Webinterface::Webinterface(int port) : server(AsyncWebServer(port)) {
    ESP_LOGI(LOG_TAG, "Webinterface started on port: %u", port);
}

void Webinterface::begin() {
    // Host the static Angular application which should be stored in SPIFFS.
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    // API: Get current clock configuration
    server.on("/api/clock", HTTP_GET, [&](AsyncWebServerRequest *request) {
        ESP_LOGI(LOG_TAG, "Current clock config requested");
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
        
        // TODO: tempcode om naar clock-mode te gaan zonder dat er een kleurconfig wordt opgeslagen. TODO: kleurconfig loskoppelen van mode setten
        auto newMode = Mode::Clock;
        xQueueOverwrite(xChangeAppQueue, &newMode);

        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    });

    // API: Get current network configuration
    server.on("/api/network", HTTP_GET, [&](AsyncWebServerRequest *request) {
        ESP_LOGI(LOG_TAG, "Current network config requested");
        auto config = &ConfigService::CONFIG.networkConfig;
        
        StaticJsonDocument<256> jsonDoc;
        jsonDoc[KEY_HOSTNAME] = config->hostname;
        jsonDoc[KEY_SSID] = config->ssid;
        jsonDoc[KEY_WIFI_CONNECTED] = ConfigService::connectedToWifi;
        
        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    });

    server.on("/api/status", HTTP_GET, [&](AsyncWebServerRequest *request) {
        ESP_LOGI(LOG_TAG, "Current status requested");
        auto config = &ConfigService::CONFIG.networkConfig;
        
        // TODO: move network status here
        StaticJsonDocument<256> jsonDoc;
        jsonDoc[KEY_VERSION] = BuildInfo::version;

        String response;
        serializeJsonPretty(jsonDoc, response);
        request->send(200, "application/json", response);
    });

    server.on("/api/clock", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<512> jsonDoc;
        if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
            ESP_LOGI(LOG_TAG, "Updating clock config");
            auto config = &ConfigService::CONFIG.clockConfig;
            float h, s, v;

            auto color = jsonDoc["colorItIs"];
            h = color[KEY_HUE] | 0.0f;
            s = color[KEY_SATURATION] | 0.0f;
            v = color[KEY_VALUE] | 0.0f;
            config->colorItIs = HsbColor(h, s, v);
            ESP_LOGD(LOG_TAG, "HSV colorItIs: %f, %f, %f", h, s, v);

            color = jsonDoc["colorWords"];
            h = color[KEY_HUE] | 0.0f;
            s = color[KEY_SATURATION] | 0.0f;
            v = color[KEY_VALUE] | 0.0f;
            config->colorWords = HsbColor(h, s, v);
            ESP_LOGD(LOG_TAG, "HSV colorWords: %f, %f, %f", h, s, v);

            color = jsonDoc["colorHour"];
            h = color[KEY_HUE] | 0.0f;
            s = color[KEY_SATURATION] | 0.0f;
            v = color[KEY_VALUE] | 0.0f;
            config->colorHour = HsbColor(h, s, v);
            ESP_LOGD(LOG_TAG, "HSV colorHour: %f, %f, %f", h, s, v);   
            
            // TODO: Only save when there's a difference with the already stored 
            ConfigService::saveEventually();
        }
        auto newMode = Mode::Clock;
        xQueueOverwrite(xChangeAppQueue, &newMode);
        request->send(200, "application/json", "{ \"status\": \"success\" }");
    });

    server.on("/api/network", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<256> jsonDoc;
        if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
            ESP_LOGI(LOG_TAG, "Updating network config");
            NetworkConfig networkConfig;
            strlcpy(networkConfig.hostname, jsonDoc[KEY_HOSTNAME] | "", sizeof(networkConfig.hostname));
            strlcpy(networkConfig.ssid, jsonDoc[KEY_SSID] | "", sizeof(networkConfig.ssid));
            strlcpy(networkConfig.password, jsonDoc[KEY_PASSWORD] | "", sizeof(networkConfig.password));
            xQueueSend(xWifiConfigChangedQueue, &networkConfig, 0);
        }
        request->send(200, "application/json", "{ \"status\": \"success\" }");
    });

    server.on("/api/image", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        StaticJsonDocument<256> jsonDoc;
        if (DeserializationError::Ok == deserializeJson(jsonDoc, (const char *)data)) {
            String image = jsonDoc["image"] | "";
            ESP_LOGI(LOG_TAG, "Mode set to show image: %s", image);
            
            Mode newMode;
            if (image.equals("xmas")) {
                newMode = Mode::Xmas;
            } else if (image.equals("snowman")) {
                newMode = Mode::Snow;
            } else if (image.equals("error")) {
                newMode = Mode::Error;
            } else if (image.equals("wifi")) {
                newMode = Mode::WifiConnecting;
            } else {
                request->send(400, "application/json", "{ \"status\": \"image does not exist\" }");
                return;
            }
            xQueueSend(xChangeAppQueue, &newMode, 0);
            request->send(200, "application/json", "{ \"status\": \"success\" }");
        }
        request->send(400, "application/json", "{ \"status\": \"specify 'image'\" }");
    });

    server.on("/api/swirl", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        ESP_LOGI(LOG_TAG, "Mode set to SWIRL");
        auto newMode = Mode::Swirl;
        xQueueOverwrite(xChangeAppQueue, &newMode);
        request->send(200, "application/json", "{ \"status\": \"success\" }");
    });

    server.on("/api/ledtest", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        ESP_LOGI(LOG_TAG, "Mode set to LEDTEST");
        auto newMode = Mode::Ledtest;
        xQueueOverwrite(xChangeAppQueue, &newMode);
        request->send(200, "application/json", "{ \"status\": \"success\" }");
    });

    server.on("/api/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        ESP_LOGI(LOG_TAG, "Rebooting");        
        request->send(200, "application/json", "{ \"status\": \"success\" }");
        delay(1000);
        ESP.restart();
    });

    server.onNotFound([&](AsyncWebServerRequest *request) {
        ESP_LOGI(LOG_TAG, "Not found: %s %s", request->url().c_str(), request->methodToString());
        request->send(404, "text/plain", "Not found");
    });

    server.begin();
}