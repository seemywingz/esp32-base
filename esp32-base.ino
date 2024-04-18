
#include <IOPin.h>

IOPin led(8, OUTPUT);

void setup() {
  Serial.begin(921600);
  Serial.println("Hello, ESP32!");
}

void loop() {
  led.off();
  delay(100);
  led.on();
  delay(100);
}
