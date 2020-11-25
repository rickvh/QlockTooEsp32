#define USE_MDNS
#define HOST_NAME "qlocktoo"



#include <Arduino.h>
#include <ArduinoOTA.h>
//#include <NeoPixelBus.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiClient.h>
#include "display.h"
// #include <SPIFFS.h>
// #include <logging.hpp>
// #include <ets-appender.hpp>
// #include <udp-appender.hpp>

// using namespace esp32m;
#include <RemoteDebugger.h> 

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


// typedef NeoGrbwFeature MyPixelColorFeature;

const uint16_t PixelCount = 110;
const uint16_t PixelPin = 13;
uint16_t hue = 0;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PixelCount, PixelPin, NEO_GRBW + NEO_KHZ800);
//NeoPixelBus<MyPixelColorFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);





#pragma region setupfunctions
void setupDisplay() {
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
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
    setColor(strip.Color(0,0,0));
  } else if (lastCmd == "red") {
    debugI("* Set display to RED");
    setColor(strip.Color(255,0,0));
  } else if (lastCmd == "rgb") {
		debugI("* Clear display");
    // strip.ColorHSV(hue+=50)));
    strip.Color(0, 0, 0);
    strip.show();
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
#pragma endregion

void setColor(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      // strip.setPixelColor()
      // strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
  }
  strip.show();
}

void setup() {
  debugI("Booting QlockToo");

  setupWifi();
  setupOTA();
  setupLogging();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  setupDisplay();
}

void loop() {


  ArduinoOTA.handle();

  Debug.handle();

  // chase(strip.gamma32(strip.ColorHSV(hue+=50)));
  // strip.show();

  delay(1000);
  //delayMicroseconds(500);
}