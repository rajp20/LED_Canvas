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

void setup() {
  Serial.begin(115200);
  Timer1.initialize();
  leds.setup();
  //  leds.toggleBouncingBall(true);
  BLEDisconnected();
  //   motors.setup();
  bluetooth.setup();
}

void loop() {
  bluetooth.updateBLE(200);
  //  motors.move(8, 8);
}

void WaitingForBLEConnection() {
  leds.waitingDots();
}

void BLEConnected() {
  Serial.println("Connected");
  Timer1.detachInterrupt();
  leds.clearCanvas();
}

void BLEDisconnected() {
  Serial.println("Disconnected");
  leds.welcomeScreen();
  Timer1.attachInterrupt(WaitingForBLEConnection);
  // NEED TO CLEAR QUEUE
}

void BLEHandleData() {
  char* data = bluetooth.readPacket();
  Serial.println(data);
  char* parsedMessage[5];

  char *str;
  int i = 0;
  while ((str = strtok_r(data, ",", &data)) != NULL) {
    parsedMessage[i++] = *str;
  }

  Serial.println(parsedMessage[0]);
  Serial.println(parsedMessage[1]);
  Serial.println(parsedMessage[2]);
}
