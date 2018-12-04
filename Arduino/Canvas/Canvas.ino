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
  
  // xyz,123,123,123
  // rgb,123,123,123
  Serial.println("In:");
  Serial.write(data, len);
  Serial.println(data[0]);

  
  if (data[0] != 'r' && data[0] != 'x'){
    return;
  }

  char values[][2] = {{'0','0'}, {'0','0'}, {'0','0'}};

  // Type 1 is RBG, type 0 is XYZ
  bool type;

  // Count for commas
  int commaCount = -1;

  int characterCount = 0;

  for (int i = 0; i < len; i++){
    if (i == 0){
      if (data[i] == 'x'){
        type = true;
      }
      else {
        type = false;
      }
    }

    if (data[i] == ','){

      // First comma
      if (commaCount == -1){
        commaCount++;
        continue;
      }

      if(characterCount == 1){
        values[commaCount][1] = data[i-1];
      }
      else if (characterCount == 2){
        values[commaCount][1] = data[i-1];
        values[commaCount][0] = data[i-2];
      }

      characterCount = 0;
      commaCount++;
      continue;
    }

    if (commaCount < 0){
      continue;
    }

    characterCount++;
  }
  Serial.println("Out:");
  for (int i = 0; i < 3; i++){
    Serial.write(values[i], 2);
    Serial.println();
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
