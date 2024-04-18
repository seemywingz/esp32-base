#include <Arduino.h>
#line 1 "/Users/kjj/git/esp32-base/esp32-base.ino"

#include <IOPin.h>

IOPin led(8, OUTPUT);

#line 6 "/Users/kjj/git/esp32-base/esp32-base.ino"
void setup();
#line 8 "/Users/kjj/git/esp32-base/esp32-base.ino"
void loop();
#line 6 "/Users/kjj/git/esp32-base/esp32-base.ino"
void setup() {}

void loop() {
  led.on();
  delay(500);
  led.off();
}

