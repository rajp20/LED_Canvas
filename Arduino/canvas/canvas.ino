#include <ArduinoJson.h>
#include <string.h>
#include <Arduino.h>


void setup() {
  LED_setup();
  ble_setup();
}

void loop() {
  String data = ble_get_data();
  if (data != "") {
    Serial.println(data); 
  }
  if (data == "Button 1 pressed") {
    LED_turn_on(255, 0, 0);
  }
  if (data == "Button 2 pressed") {
    LED_turn_on(0, 255, 0);
  }
  if (data == "Button 3 pressed") {
    LED_turn_on(0, 0 , 255);
  }
  if (data == "Button 4 pressed") {
    LED_turn_on(0, 0 , 0);
  }
}
