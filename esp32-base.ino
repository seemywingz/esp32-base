
#include "ESPWiFi.h"

// Web Server
ESPWiFi wifi("ESP32", "abcd1234");

void setup() { wifi.start(); }

void loop() {
  wifi.handleClient();
  delay(10);
}
