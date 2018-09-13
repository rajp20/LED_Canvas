#include <string.h>
#include <Arduino.h>


void setup() {
  ble_setup();
}

void loop() {
  ble_get_data();
}
