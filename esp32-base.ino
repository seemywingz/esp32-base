
#include <ESPWiFi.h>
#include <IOPin.h>
#include <LittleFS.h>

IOPin led(8, OUTPUT);

// Web Server
const String webServerName = "ESP32-C3";
ESPWiFi wifi(webServerName, "abcd1234");

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  initializeWebServer();
}

void loop() { wifi.handleClient(); }

void initializeWebServer() {
  wifi.setConnectSubroutine([]() {
    Serial.println("Connecting to WiFi");
    Serial.println(wifi.ssid + " " + wifi.password);
    delay(500);
  });
  wifi.enableMDNS(webServerName);
  wifi.start();
}