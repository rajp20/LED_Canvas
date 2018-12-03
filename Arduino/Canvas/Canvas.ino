// #include <QueueArray.h>
#include <string.h>
#include <Arduino.h>

#include "TimerOne.h"
#include "BluetoothController.h"
#include "LEDController.h"
#include "MotorController.h"

BluetoothController bluetooth;
LEDController leds;
MotorController motors;

String buff = "";
bool data_in = false;
bool bluetooth_connected = false;

void setup() {
  Serial.begin(115200);
  Timer1.initialize();
  // leds.setup();
  // BLEDisconnected();
   motors.setup();
  // bluetooth.setup();
}

void loop() {
  bluetooth.updateBLE(200);
  if (bluetooth_connected) {
    char* data = bluetooth.readPacket();
  }

  //  if (strcmp(data, "connecting")) {
  //    Serial.println("Connected to iOS app. Clearing canvas.");
  //    Timer1.detachInterrupt();
  //    leds.clearCanvas();
  //  }

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

  motors.move(8,0);
  motors.move(8, 8);
}

void WaitingForBLEConnection() {
  leds.waitingDots();
}

void BLEConnected() {
  bluetooth_connected = true;
  Serial.println("Connected");
  Timer1.detachInterrupt();
  leds.clearCanvas();
}

void BLEDisconnected() {
  bluetooth_connected = false;
  leds.welcomeScreen();
  Timer1.attachInterrupt(WaitingForBLEConnection);
}
