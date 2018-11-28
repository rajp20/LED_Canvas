#include <QueueArray.h>
#include <string.h>
#include <Arduino.h>

#include "BluetoothController.h"
#include "LEDController.h"
#include "MotorController.h"

BluetoothController bluetooth;
LEDController leds;
MotorController motors;

String buff = "";
bool data_in = false;

void setup() {
  leds.setup();
  leds.welcomeScreen();
  bluetooth.setup();
  motors.setup();
}

void loop() {
//  char* data = bluetooth.readPacket();
//
//  char* parsedMessage[5];
//
//  char *str;
//  int i = 0;
//  while ((str = strtok_r(data, ",", &data)) != NULL) {
//    parsedMessage[i++] = *str;
//  }
//
//  String xy = "";
//  if (data != "") {
//    if (data == "end") {
//      data_in = false;
//      //StaticJsonBuffer<200> jsonBuffer;
//      //JsonObject& root = jsonBuffer.parseObject(buff);
//      Serial.println(buff);
//      buff = "";
//    }
//    if (data_in) {
//      buff += data;
//    }
//    if (data == "start") {
//      data_in = true;
//    }
//  }
}
