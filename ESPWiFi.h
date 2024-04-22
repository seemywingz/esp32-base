#ifndef ESPWiFi_h
#define ESPWiFi_h

#include <Arduino.h>
#include <ESPmDNS.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>

class ESPWiFi {
 public:
  String ssid;
  String domain;
  String password;
  String credentialsFile = "/wifi-credentials";

  WebServer webServer;

  bool APEnabled = false;

  ESPWiFi(String apSsid, String apPassword) {
    this->ssid = apSsid;
    this->domain = apSsid;
    this->password = apPassword;
  }

  void start() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial) {
      delay(10);
    }

    if (!LittleFS.begin()) {
      Serial.println("An Error has occurred while mounting LittleFS");
    }

    if (!readWifiCredentials()) {
      Serial.println("Failed to read WiFi credentials: " + credentialsFile);
      Serial.println("Starting as Access Point:");
      Serial.println("\tSSID: " + ssid);
      Serial.println("\tPassword: " + password);
      APEnabled = true;
    } else {
      connectToWifi();
    }

    startWebServer();
    startMDNS();
  }

  void connectToWifi() {
    WiFi.begin(ssid, password);
    Serial.println("\n\nConnecting to: " + ssid);
    while (WiFi.status() != WL_CONNECTED) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.print(WiFi.localIP());
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
    if (!MDNS.begin(domain)) {
      Serial.println("Error setting up MDNS responder!");
    } else {
      MDNS.addService("http", "tcp", 80);
    }
  }

  void startWebServer() {
    // Route for root / web page
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
    webServer.begin();
  }

  bool readWifiCredentials() {
    File credFile = LittleFS.open(credentialsFile, "r");
    if (!credFile) {
      Serial.println("Credentials file not found");
      return false;
    }

    String ssid = credFile.readStringUntil('\n');
    String password = credFile.readStringUntil('\n');
    credFile.close();

    ssid.trim();
    password.trim();

    if (ssid.length() > 0 && password.length() > 0) {
      this->ssid = ssid;
      this->password = password;
      return true;
    }

    return false;
  }
};
#endif  // ESPWiFi_h