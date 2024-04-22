
#include "src/ESPWiFi.h"

// Web Server
ESPWiFi wifi;

bool runOnce = false;

void setup() { wifi.start(); }

void loop() {
  wifi.handleClient();
  if (!runOnce) {
    runOnce = true;
    wifi.openAI_TTS("Hello, world!", "/hello.mp3");
  }
  delay(10);
}
