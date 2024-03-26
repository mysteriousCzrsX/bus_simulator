#include <Arduino.h>
#include <userIO.h>
#include "config.h"

#define DEBUG

userIO io(DATA_PIN, CLOCK_PIN, LATCH_PIN, button_pins);

void setup() {
  Serial.begin(115200);
}

void loop() {
  io.self_test();
  delay(3000);
}
