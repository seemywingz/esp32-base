
#include <ESPWiFi.h>

// Web Server
const String webServerName = "ESP32-C3";
ESPWiFi wifi(webServerName, "abcd1234");

void setup() {
  wifi.enableMDNS(webServerName);
  wifi.start();
}

void loop() { wifi.handleClient(); }
