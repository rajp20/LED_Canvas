#include <ArduinoJson.h>
#include <string.h>
#include <Arduino.h>

#include "BluetoothController.h"
#include "LEDController.h"

BluetoothController bluetooth;
LEDController led;

void setup() {
  led.setup();
  bluetooth.setup();
}

void loop() {
  String data = bluetooth.getData();
  if (data != "") {
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(data);
    int x = root["x"];
    int y = root["y"];
    String toPrint = String(x) + " " + String(y);
    Serial.print(F("[Recv] ")); Serial.println(toPrint);
  }
}
