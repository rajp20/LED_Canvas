#define ARDUINO_2_INTER     19

#include <QueueArray.h>
#include <Arduino.h>
//#include "TimerOne.h"
//#include "TimerThree.h"
#include "BluetoothController.h"
#include "LEDController.h"
//#include "MotorController.h"

#include <Wire.h>

BluetoothController bluetooth;
//MotorController motors;
LEDController leds;

uint16_t queue[100];
int front = 0;
int end = 0;
int size = 0;
uint16_t previous_xy = 0;
bool queue_initialized = false;
bool actuator_up = false;

String buff = "";
bool data_in = false;

int32_t charid_string;
int32_t charid_number;

byte effect = 0;
bool fade_out = false;

void setup() {
  Serial.begin(115200);
  //  Wire.setClock(400000);
  Wire.begin();
  pinMode(ARDUINO_2_INTER, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ARDUINO_2_INTER), SecondaryArduinoInterrupt, HIGH);
  bluetooth.setup();
  leds.setup();
  leds.pixelTest();
  leds.overflow();
  BLEDisconnected();
}

void loop() {
  bluetooth.updateBLE(100);
  if (bluetooth.getConnection()) {
    bluetooth.writePacket("1");
    char* data = bluetooth.readPacket();
    if (strcmp(data, "") != 0) {
      BLEDataReceived(data, 0);
    }
  }

  if (effect == 0) {
    fade_out = true;
  } else {
    EmptyQueue();
    fade_out = false;
    if (effect == 1) {
      BouncingBall();
    } else if (effect == 2) {
      RippleEffect();
    } else if (effect == 4) {
      ConwayLife();
    } else if (effect == 5) {
      leds.overflow();
      TurnOffEffect();
    } else if (effect == 255) {
      WaitingForBLEConnection();
    }
  }
  if (fade_out) {
    FadeOutCanvas();
  }
}

void SecondaryArduinoInterrupt() {
  //  Serial.println("In");
  if (size != 0) {
    uint8_t previous_x = previous_xy >> 8;
    uint8_t previous_y = previous_xy;
    leds.drawBox(previous_x, previous_y);
    uint16_t current = queue[front];
    front++;
    size--;
    if (front > 100) {
      front = 0;
    }
    previous_xy = current;
    uint8_t current_x = current >> 8;
    uint8_t current_y = current;
    //    Serial.print("Sending: ");
    //    Serial.println(current_x);
    //    Serial.println(current_y);
    Wire.beginTransmission(8); // transmit to device #8
    Wire.write((byte)current_x);
    Wire.write((byte)current_y);
    Wire.endTransmission();    // stop transmitting
    if (size == 0) {
      actuator_up = false;
      Wire.beginTransmission(8); // transmit to device #8
      Wire.write('d');
      Wire.endTransmission();    // stop transmitting
    }
  } else {
    //    if (actuator_up) {
    //      actuator_up = false;
    //      Wire.beginTransmission(8); // transmit to device #8
    //      Wire.write('d');
    //      Wire.endTransmission();    // stop transmitting
    //    }
    queue_initialized = false;
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

void EmptyQueue() {
  front = 0;
  end = 0;
  size = 0;
}

// BLUETOOTH CALLBACK FUNCTIONS
void BLEConnected() {
  // If it is already connected, getting bad data and just return
  if (bluetooth.getConnection()) {
    Serial.println("Bad connect");
    return;
  }
  Serial.println("Connected");
  TurnOffEffect();
  bluetooth.isConnected();
  bluetooth.writePacket("Thx");
}

void BLEDisconnected() {
  bluetooth.disconnect();
  Serial.println("Disconnected");
  effect = 255;
  leds.welcomeScreen();
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
    leds.clearCanvas();
    effect = 0;
    EmptyQueue();
  } else if (data[0] == 'p') { // Patters
    // Ball Patern
    if (data[1] == '1') {
      // Toggle on
      if (data[2] == '1') {
        effect = 1;
        leds.clearCanvas();
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
        leds.clearCanvas();
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
        leds.clearCanvas();
        leds.acid = true;
        effect = 2;
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
        leds.conwayLifeInitial();
      }
      // Toggle off
      else {
        TurnOffEffect();
      }
    }
    // Overflow
    else if (data[1] == '5') {
      front = 0;
      end = 0;
      size = 0;
      leds.clearCanvas();
      effect = 5;
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
      if (size == 0 && !queue_initialized) {
        queue_initialized = true;
        previous_xy = ((uint16_t)parsedData[0] << 8) | parsedData[1];
        Wire.beginTransmission(8); // transmit to device #8
        Wire.write((byte)parsedData[0]);
        Wire.write((byte)parsedData[1]);
        Wire.endTransmission();    // stop transmitting
      } else {
        //        Serial.println(parsedData[1]);
        queue[end] = (((uint16_t)parsedData[0] << 8) | parsedData[1]);
        end++;
        size++;
        if (end > 100) {
          end = 0;
        }
      }
    }
  }
  //  bluetooth.writePacket("1");
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
