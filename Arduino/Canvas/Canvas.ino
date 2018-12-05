#include <QueueArray.h>
#include <string.h>
#include <Arduino.h>

#include "TimerOne.h"
#include "BluetoothController.h"
#include "LEDController.h"
#include "MotorController.h"

BluetoothController bluetooth;
LEDController leds;
MotorController motors;

QueueArray<char*> queue;

String buff = "";
bool data_in = false;

int32_t charid_string;
int32_t charid_number;

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
  bluetooth.updateBLE(50);
  if (!queue.isEmpty()) {
    BLEHandleData(queue.dequeue());
  }
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

void BLEDataReceived(char* data, uint16_t len) {
  Serial.write(data);
  Serial.println();
  char *str = strtok(data, ";");
  while (str != NULL) { // delimiter is the semicolon
    queue.enqueue(str);
    str = strtok(NULL, ";");
  }
}

void BLEHandleData(char* data) {
  // xyz,123,123,123
  // rgb,123,123,123


  char command;
  int parsedData[3];
  int index = 0;
  char *str = strtok(data, ",");
  while (str != NULL) { // delimiter is the semicolon
    if (index == 0) {
//      Serial.println(str);
      command = str[0];
    } else {
      parsedData[index - 1] = atoi(str);
    }
    index++;
    str = strtok(NULL, ",");
  }
  
  Serial.println(command);
  if (command == 'r') {
    leds.setRGB(parsedData[0], parsedData[1], parsedData[2]);
  } else if (command == 'x') {
    // Call motors and leds
    leds.drawPixel(parsedData[0], parsedData[1]);
  }

  Serial.println();
}

void BleGattRX(int32_t chars_id, uint8_t data[], uint16_t len)
{
  Serial.print( F("[BLE GATT RX] (" ) );
  Serial.print(chars_id);
  Serial.print(") ");

  if (chars_id == charid_string)
  {
    Serial.write(data, len);
    Serial.println();
  } else if (chars_id == charid_number)
  {
    int32_t val;
    memcpy(&val, data, len);
    Serial.println(val);
  }
}
