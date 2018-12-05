#include <QueueArray.h>
#include <Arduino.h>

#include "TimerOne.h"
#include "TimerThree.h"
#include "BluetoothController.h"
#include "LEDController.h"
#include "MotorController.h"

BluetoothController bluetooth;
LEDController leds;
MotorController motors;

//QueueArray<char*> queue;

String buff = "";
bool data_in = false;

int32_t charid_string;
int32_t charid_number;

void setup() {
  Serial.begin(115200);
  //  Timer1.initialize(100000);
  bluetooth.setup();
  leds.setup();
  leds.pixelTest();
  //  motors.setup();
  BLEDisconnected();
}

void loop() {
  bluetooth.updateBLE(100);
}

void WaitingForBLEConnection() {
  leds.waitingDots();
}

void BouncingBall() {
  leds.bouncingBall();
}

void RippleEffect() {
  leds.ripple();
}

// BLUETOOTH CALLBACK FUNCTIONS
void BLEConnected() {
  Serial.println("Connected");
  Timer3.detachInterrupt();
  leds.clearCanvas();
  bluetooth.writePacket("Thx");
}

void BLEDisconnected() {
  Serial.println("Disconnected");
  leds.welcomeScreen();
  Timer3.setPeriod(1000000);
  Timer3.attachInterrupt(WaitingForBLEConnection);
}

/**
  Once the data is recieved, parse it and call appropiate functions.
  Supported Commands:
  xyz,123,123,123   -> Moves the motors and turns on the pixels on led strips
  rgb,123,123,123   -> Sets the RGB color
  rst               -> Reset the canvas
*/
void BLEDataReceived(char* data, uint16_t len) {
  // xyz,123,123,123
  // rgb,123,123,123
  //  Serial.println(data);
  if (strcmp(data, "rst") == 0) {
    leds.clearCanvas();
  } else if (strcmp(data, "tbb1") == 0) {
    leds.clearCanvas();
    Timer3.setPeriod(100000);
    Timer3.attachInterrupt(BouncingBall);
  } else if (strcmp(data, "tbb0") == 0) {
    Timer3.detachInterrupt();
    leds.clearCanvas();
  } else if (strcmp(data, "rip1") == 0) {
    leds.clearCanvas();
    Timer3.setPeriod(100000);
    Timer3.attachInterrupt(RippleEffect);
  } else if (strcmp(data, "rip0") == 0) {
    Timer3.detachInterrupt();
    leds.clearCanvas();
  } else if (strcmp(data, "acid1") == 0) {
    leds.acid = true;
    leds.clearCanvas();
    Timer3.setPeriod(100000);
    Timer3.attachInterrupt(RippleEffect);
  } else if (strcmp(data, "acid0") == 0) {
    leds.acid = false;
    Timer3.detachInterrupt();
    leds.clearCanvas();
  } else {
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

    //  Serial.println(command);
    if (command == 'r') {
      leds.setColor(parsedData[0], parsedData[1], parsedData[2]);
    } else if (command == 'x') {
      // Call motors and leds
      leds.drawPixel(parsedData[0], parsedData[1]);
    }
  }
  bluetooth.writePacket("1");
  //  Serial.println();
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
