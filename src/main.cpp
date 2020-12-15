#define USE_MDNS
#define HOST_NAME       "qlocktoo"
#define LEDSTRIP_PIN    13
//#define DEBUG_DISABLED // uncomment for production release
#define WEBSOCKET_DISABLED // disbale logging via websockets

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <RemoteDebugger.h> 
// #include "display.h"
#include <Adafruit_NeoMatrix.h>

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


uint8_t frame = 1;
uint8_t x_start = 1;
uint8_t y_start = 1;
uint16_t hue = 0;

int x    = 11;
int pass = 0;


// Display display = Display(LEDSTRIP_PIN);
Adafruit_NeoMatrix display = Adafruit_NeoMatrix(11, 10, LEDSTRIP_PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_RGBW           + NEO_KHZ800);


uint16_t remapPixels(uint16_t x, uint16_t y) {
  static uint8_t mapping[11][10] = {
    {73,80,102,96,58,51,22,7,30,0},
    {81,66,65,88,89,15,14,44,43,37},
    {72,103,82,59,95,21,50,29,6,38},
    {74,79,101,97,57,52,23,8,31,1},
    {104,67,64,87,90,16,13,45,42,36},
    {71,109,83,60,94,20,49,28,5,39},
    {75,78,100,98,56,53,24,9,32,2},
    {105,68,63,86,91,17,12,46,41,35},
    {70,108,84,61,93,19,48,27,4,40},
    {76,77,107,99,55,54,25,10,33,3},
    {106,69,62,85,92,18,11,47,26,34}
  };

  debugD("Getting mapping for: %x, %y", x, y);
  return mapping[x][9-y];
}

void setupDisplay() {
  display.setRemapFunction(remapPixels);
  display.begin();


  display.setTextWrap(false);
  display.setBrightness(5);
  display.setTextColor(display.Color(0, 200, 0));
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
    setColor(display.Color(0,0,0));
  } else if (lastCmd == "red") {
    debugI("* Set display to RED");
    setColor(display.Color(255,0,0));
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
  display.fill(c);
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

  delay(100);
  //delayMicroseconds(500);

  // switch(frame++){
  //   case 1:
  //     display.fill(display.Color(255,0,0));
  //     break;
  //   case 2:
  //     display.fill(display.Color(0,255,0));
  //     break;
  //   case 3:
  //     display.fill(display.Color(0,0,255));
  //     break;
  //   default:
      // display.drawLine(x_start, y_start, display.width() - x_start, display.height() - y_start, display.ColorHSV(hue+=100, 200, 30));
      // if (x_start == display.width()){
      //   if (y_start == display.height()) {
      //     x_start = 0;
      //   } else {
      //     y_start++;
      //   }
      // } else {
      //   y_start = 0;
      //   x_start++;
      // }
      // break;

      display.fillScreen(0);
      display.setCursor(x, 0);
      display.print(F("Charlie is een schatje!"));
      if(--x < -136) {
        x = display.width();
        if(++pass >= 3) pass = 0;
        display.setTextColor(display.ColorHSV(hue+=5000, 100, 100));
      }
      // break;

      display.show();
  // }
  // display.show();
};