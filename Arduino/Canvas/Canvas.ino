#include <ArduinoJson.h>
#include <string.h>
#include <Arduino.h>

#include "BluetoothController.h"
#include "LEDController.h"
#include "MotorController.h"

BluetoothController bluetooth;
LEDController led;
MotorController moter;

String buff = "";
bool data_in = false;

void setup() {
  led.setup();
  bluetooth.setup();
}

void loop() {
  String data = bluetooth.readPacket();
  String xy = "";
  if (data != "") {
    if (data == "end") {
      data_in = false;
      //StaticJsonBuffer<200> jsonBuffer;
      //JsonObject& root = jsonBuffer.parseObject(buff);
      Serial.println(buff);
      buff = "";
    }
    if (data_in) {
      buff += data;
    }
    if (data == "start") {
      data_in = true;
    }
  }
}
