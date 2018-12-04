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

void BLEHandleData(char* data, uint16_t len) {

  Serial.write(data, len);

  //  char* msgField[5];

  //  byte sentencePos = 0;
  //  int commaCount = 0;
  //  msgField[commaCount] = "";
  //  while (sentencePos < len)
  //  {
  //    if (data[sentencePos] == ',')
  //    {
  //      commaCount++;
  //      msgField[commaCount] = "";
  //      sentencePos++;
  //    }
  //    else
  //    {
  //      msgField[commaCount] += data[sentencePos];
  //      sentencePos++;
  //    }
  //  }
  //
  //  Serial.println(msgField[0]);
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
