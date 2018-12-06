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

typedef struct {
   int x;
   int y;
} XYData;

QueueArray<XYData> queue;

String buff = "";
bool data_in = false;

int32_t charid_string;
int32_t charid_number;

void setup() {
  Serial.begin(115200);
  //    Timer1.initialize(100000);
  Timer3.initialize();
  bluetooth.setup();
  leds.setup();
  leds.pixelTest();
  motors.setup();
  BLEDisconnected();
}

void loop() {
  bluetooth.updateBLE(100);
  if (!queue.isEmpty()) {
    XYData xy = queue.dequeue();
    motors.move(xy.x, xy.y);
    leds.drawPixel(xy.x, xy.y);
  }
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

void ConwayLife() {
  leds.conwayLife();
}

// BLUETOOTH CALLBACK FUNCTIONS
void BLEConnected() {
  // If it is already connected, getting bad data and just return
  if (bluetooth.getConnection()) {
    Serial.println("Bad connect");
    return;
  }
  Serial.println("Connected");
  Timer3.detachInterrupt();
  leds.clearCanvas();
  bluetooth.isConnected();
  bluetooth.writePacket("Thx");
}

void BLEDisconnected() {
  bluetooth.disconnect();
  Serial.println("Disconnected");
  Timer3.detachInterrupt();
  leds.welcomeScreen();
  Timer3.setPeriod(1000000);
  Timer3.attachInterrupt(WaitingForBLEConnection);
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
  //  Serial.println(data);
  if (data[0] == '0') {
    Timer3.detachInterrupt();
    leds.clearCanvas();
  } else if (data[0] == 'p') { // Patters
    // Ball Patern
    if (data[1] == '1') {
      // Toggle on
      if (data[2] == '1') {
        Timer3.detachInterrupt();
        leds.clearCanvas();
        Timer3.setPeriod(100000);
        Timer3.attachInterrupt(BouncingBall);
      }
      // Toggle off
      else {
        Timer3.detachInterrupt();
        leds.clearCanvas();
      }
    }
    // Ripple
    else if (data[1] == '2') {
      // Toggle on
      if (data[2] == '1') {
        Timer3.detachInterrupt();
        Serial.println("Ripple On");
        leds.clearCanvas();
        Timer3.setPeriod(200000);
        Timer3.attachInterrupt(RippleEffect);
      }
      // Toggle off
      else {
        Serial.println("Ripple Off");
        Timer3.detachInterrupt();
        leds.clearCanvas();
      }

    }
    // Ripple Acid
    else if (data[1] == '3') {
      // Toggle on
      if (data[2] == '1') {
        Timer3.detachInterrupt();
        Serial.println("Acid On");
        leds.acid = true;
        leds.clearCanvas();
        Timer3.setPeriod(100000);
        Timer3.attachInterrupt(RippleEffect);
      }
      // Toggle off
      else {
        Serial.println("Acid Off");
        leds.acid = false;
        Timer3.detachInterrupt();
        leds.clearCanvas();
      }
    }
    // Conway Algo
    else if (data[1] == '4') {
      // Toggle on
      if (data[2] == '1') {
        Timer3.detachInterrupt();
        leds.conwayLifeInitial();
        Timer3.setPeriod(100000);
        Timer3.attachInterrupt(ConwayLife);
      }
      // Toggle off
      else {
        Timer3.detachInterrupt();
        leds.clearCanvas();
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

      XYData data = {
        parsedData[0],
        parsedData[1]
      };
      queue.enqueue(data);
      // Call motors and leds
      //      motors.move(parsedData[0], parsedData[1]);
      //      leds.drawPixel(parsedData[0], parsedData[1]);
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
