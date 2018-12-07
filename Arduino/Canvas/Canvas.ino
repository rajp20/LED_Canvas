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

byte effect = 0;
bool fade_out = false;

void setup() {
  Serial.begin(115200);
  //    Timer1.initialize(100000);
  Timer3.initialize();
  bluetooth.setup();
  leds.setup();
  leds.pixelTest();
  //  motors.setup();
  BLEDisconnected();
}

void loop() {
  bluetooth.updateBLE(100);
  if (effect == 0) {
    fade_out = true;
  } else {
    fade_out = false;
    if (effect == 1) {
      BouncingBall();
    } else if (effect == 2 || effect == 3) {
      RippleEffect();
    } else if (effect == 4) {
      ConwayLife();
    } else if (effect == 255) {
      WaitingForBLEConnection();
    }
  }
}

void WaitingForBLEConnection() {
  leds.waiting();
}

void BouncingBall() {
  leds.bouncingBall();
}

void RippleEffect() {
  leds.ripple();
}

void ConwayLife() {
  leds.conwayLife();
}

void FadeOutCanvas() {
  leds.fadeOutCanvas(2);
}

// BLUETOOTH CALLBACK FUNCTIONS
void BLEConnected() {
  // If it is already connected, getting bad data and just return
  if (bluetooth.getConnection()) {
    Serial.println("Bad connect");
    return;
  }
  Serial.println("Connected");
  effect = 0;
  DetachInterruptClearCanvas();
  bluetooth.isConnected();
  bluetooth.writePacket("Thx");
}

void BLEDisconnected() {
  bluetooth.disconnect();
  Serial.println("Disconnected");
  Timer3.detachInterrupt();
  effect = 255;
  leds.welcomeScreen();
}

void DetachInterruptClearCanvas() {
  Timer3.detachInterrupt();
  leds.clearCanvas();
  //  Timer3.setPeriod(1000000);
  //  Timer3.attachInterrupt(FadeOutCanvas);
}

void TurnOffEffect() {
  leds.clearCanvas();
  effect = 0;
}

/**
  Once the data is recieved, parse it and call appropiate functions.
  Supported Commands:
  0       - Clear
  p       - Patterns
    1       - Ball Pattern
      1       - On
      0       - Off
    1       - Ripple Effect
      1       - On
      0       - Off
    2       - Ripple Acid Effect
      1       - On
      0       - Off

  c,123,123,123       - Set the RGB color
  x,123,123           - XY for motors and LEDs
*/
void BLEDataReceived(char* data, uint16_t len) {
  // xyz,123,123,123
  // rgb,123,123,123
  Serial.println(data);
  if (data[0] == '0') {
    DetachInterruptClearCanvas();
    effect = 0;
  } else if (data[0] == 'p') { // Patters
    // Ball Patern
    if (data[1] == '1') {
      // Toggle on
      if (data[2] == '1') {
        effect = 1;
        DetachInterruptClearCanvas();
      }
      // Toggle off
      else {
        TurnOffEffect();
      }
    }
    // Ripple
    else if (data[1] == '2') {
      // Toggle on
      if (data[2] == '1') {
        DetachInterruptClearCanvas();
        effect = 2;
      }
      // Toggle off
      else {
        TurnOffEffect();
      }

    }
    // Ripple Acid
    else if (data[1] == '3') {
      // Toggle on
      if (data[2] == '1') {
        DetachInterruptClearCanvas();
        leds.acid = true;
        effect = 3;
      }
      // Toggle off
      else {
        leds.acid = false;
        TurnOffEffect();
      }
    }
    // Conway Algo
    else if (data[1] == '4') {
      // Toggle on
      if (data[2] == '1') {
        effect = 4;
        Timer3.detachInterrupt();
        leds.conwayLifeInitial();
      }
      // Toggle off
      else {
        TurnOffEffect();
      }
    }
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
    if (command == 'c') {
      leds.setColor(parsedData[0], parsedData[1], parsedData[2]);
    } else if (command == 'x') {
      // Call motors and leds
      //      motors.move(parsedData[0], parsedData[1]);
      leds.drawBox(parsedData[0], parsedData[1]);
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
