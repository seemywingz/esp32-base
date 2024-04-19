
#include <ESPWiFi.h>
#include <IOPin.h>
#include <SPIFFS.h>

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
  // List all files in the SPIFFS
  wifi.webServer.on("/files", HTTP_GET, []() {
    File root = SPIFFS.open("/");
    if (!root) {
      wifi.webServer.send(500, "text/html", "Failed to open directory");
      return;
    }
    if (!root.isDirectory()) {
      wifi.webServer.send(500, "text/html", "No directory found");
      return;
    }

    String message = "Files on SPIFFS:<br>";
    File file = root.openNextFile();
    while (file) {
      String fileName = file.name();
      message += "<a href=\"" + fileName + "\">" + fileName + "</a><br>";
      file = root.openNextFile();
    }
    wifi.webServer.send(200, "text/html", message);
  });
  wifi.enableMDNS(webServerName);
  wifi.start();
}