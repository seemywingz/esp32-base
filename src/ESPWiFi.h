#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>

class ESPWiFi {
 public:
  DynamicJsonDocument config = DynamicJsonDocument(256);

  String configFile = "/config.json";

  WebServer webServer;

  ESPWiFi() {}

  void init() {
    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }

    if (!LittleFS.begin()) {
      Serial.println("An Error has occurred while mounting LittleFS");
    }
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  }

  void start() {
    init();

    if (!readConfig()) {
      Serial.println("Failed to read config file, starting AP");
      startAP();
    } else {
      connectToWifi();
    }

    startWebServer();
    startMDNS();
  }

  void connectToWifi() {
    String ssid = config["client"]["ssid"];
    String password = config["client"]["password"];
    WiFi.begin(ssid, password);
    Serial.println("\n\nConnecting to: " + ssid);
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("\tWiFi connected");
    Serial.print("\tIP Address: ");
    Serial.println(WiFi.localIP());
  }

  void startAP() {
    String ssid = "ESP32-" + String(ESP.getEfuseMac(), HEX);
    String password = "abcd1234";
    WiFi.softAP(ssid, password);
    Serial.println("\n\nStarting Access Point:");
    Serial.println("\tSSID: " + ssid);
    Serial.println("\tPassword: " + password);
    Serial.print("\tIP Address: ");
    Serial.println(WiFi.softAPIP());
  }

  String getContentType(String filename) {
    if (filename.endsWith(".html"))
      return "text/html";
    else if (filename.endsWith(".css"))
      return "text/css";
    else if (filename.endsWith(".js"))
      return "application/javascript";
    else if (filename.endsWith(".png"))
      return "image/png";
    else if (filename.endsWith(".jpg"))
      return "image/jpeg";
    else if (filename.endsWith(".gif"))
      return "image/gif";
    else if (filename.endsWith(".ico"))
      return "image/x-icon";
    else if (filename.endsWith(".xml"))
      return "text/xml";
    else if (filename.endsWith(".pdf"))
      return "application/x-pdf";
    else if (filename.endsWith(".zip"))
      return "application/x-zip";
    else if (filename.endsWith(".gz"))
      return "application/x-gzip";
    // Add more MIME types here as needed
    return "text/plain";
  }

  void handleClient() { webServer.handleClient(); }

 private:
  void startMDNS() {
    String domain = config["mdns"]["domain"];
    if (!MDNS.begin(domain)) {
      Serial.println("Error setting up MDNS responder!");
    } else {
      MDNS.addService("http", "tcp", 80);
      Serial.println("\tDomain Name: " + domain + ".local");
    }
  }

  void startWebServer() {
    webServer.on("/", HTTP_GET, [this]() {
      File file = LittleFS.open("/index.html", "r");
      if (file) {
        webServer.streamFile(file, "text/html");
        file.close();
      }
    });

    // Generic handler for all file requests
    webServer.onNotFound([this]() {
      String path = webServer.uri();
      if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        String contentType =
            getContentType(path);  // Determine the file's MIME type
        webServer.streamFile(file, contentType);
        file.close();
      } else {
        webServer.send(200, "text/html", "404: Not Found");
      }
    });

    // List all files in the LittleFS

    webServer.begin();
  }

  bool readConfig() {
    // Open config.json file
    File file = LittleFS.open(configFile, "r");
    if (!file) {
      Serial.println("Failed to open config file");
      return false;
    }

    // Parse the JSON object
    DeserializationError error = deserializeJson(config, file);
    if (error) {
      Serial.println("Failed to read file, using default configuration");
      return false;
    }
    return true;
  }
};
#endif  // ESPWiFi_h