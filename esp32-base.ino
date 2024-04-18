
#include <IOPin.h>

IOPin led(8, OUTPUT);

void setup() {}

void loop() {
  led.on();
  delay(500);
  led.off();
}
