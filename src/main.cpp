#define USE_MDNS
#define HOST_NAME       "qlocktoo"
#define LEDSTRIP_PIN    13
//#define DEBUG_DISABLED // uncomment for production release
#define WEBSOCKET_DISABLED // disbale logging via websockets

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <RemoteDebugger.h> 
#include "display.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#else
#include <WiFiClient.h>
#include <DNSServer.h>
#include "ESPmDNS.h"
// #include <WiFiUdp.h>
#endif

RemoteDebug Debug;
void setColor(uint32_t);

const char* ssid = "SKULLFORT";
const char* password = "schattigebabyeendjes.jpg!";


uint16_t hue = 0;
Display display = Display(LEDSTRIP_PIN);


void setupDisplay() {
  display.begin();
}

void setupWifi() {
  debugI("Connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    debugE("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  debugI("Wifi connected");


  #if defined USE_MDNS && defined HOST_NAME
	if (MDNS.begin(HOST_NAME)) {
		debugI("* MDNS responder started. Hostname -> ");
		debugI(HOST_NAME);
	}
  #else
  debugI("mDNS disabled");
  #endif
}

void setupOTA() {
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

void processCmdRemoteDebug() {
  String lastCmd = Debug.getLastCommand();

	if (lastCmd == "clear") {
		debugI("* Clear display");
    // strip.ColorHSV(hue+=50)));
    setColor(display.Color(0,0,0,0));
  } else if (lastCmd == "red") {
    debugI("* Set display to RED");
    setColor(display.Color(255,0,0,0));
  } else if (lastCmd == "rgb") {
		debugI("* Not implemented");
    // strip.ColorHSV(hue+=50)));
    // strip->Color(0, 0, 0);
    // strip->show();
  } else if (lastCmd == "test") {
		debugI("Leds");
    // debugI("%i:%i:%i", 4, 2, strip->numPixels());
  }
}

void setupLogging() {

  Debug.begin("qlocktoo"); // Initialize the WiFi server
	Debug.setResetCmdEnabled(true); // Enable the reset command
  // Debug.showProfiler(true); // Profiler (Good to measure times, to optimize codes)
	Debug.showColors(true); // Colors

	String helpCmd = "clear - Clear display\n";
  helpCmd.concat("red - Set red color");
	helpCmd.concat("rgb <r,g,b> - Set RGB value");

	Debug.setHelpProjectsCmds(helpCmd);
	Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
	Debug.initDebugger(debugGetDebuggerEnabled, debugHandleDebugger, debugGetHelpDebugger, debugProcessCmdDebugger); // Set the callbacks

	// debugInitDebugger(&Debug); // Init the debugger

  // if (debugAddFunctionVoid("benchInt", &benchInt) >= 0) {
  // debugSetLastFunctionDescription("To run a benchmark of integers");
// }

}

void setColor(uint32_t c) {
  debugD("Set color to: ", c);
  for(uint8_t y=0; y < display.getHeight(); y++) {
    for(uint8_t x=0; x < display.getWidth(); x++) {
      display.setPixel(x, y, c); // Draw new pixel
      debugV("Set pixel %x, %y --> %c", x, y, c);
    }
  }
  display.show();
}

void setup() {
  debugI("Booting QlockToo");

  setupWifi();
  setupOTA();
  setupLogging();
  setupDisplay();
}

void loop() {
  ArduinoOTA.handle();

  Debug.handle();

  // chase(strip.gamma32(strip.ColorHSV(hue+=50)));
  // strip.show();

  delay(1000);
  //delayMicroseconds(500);
};