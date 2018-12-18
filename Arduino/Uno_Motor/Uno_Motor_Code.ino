#include <Arduino.h>
#include "MotorController.h"
#include <Wire.h>
#include "TimerOne.h"

MotorController motors;

byte nextX;
byte nextY;

bool newData;
bool firstPoint;

int c;

void setup() {
  Wire.begin(8);
  Wire.onReceive(receivedMotorCoords);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  // Y MOTOR DIR
  Serial.begin(9600);
  motors.setup();
  motors.actuatorUp();
  delay(1000);
  motors.actuatorDown();
  Serial.println("Done setup");
  delay(1000);
  newData = false;
  firstPoint = true;
  c = 1;
}

// horizontal, vertical

void loop() {
  
  if (newData){

    if (nextX > 62 || nextY > 18){
      return;
    }
    
    Serial.print("ND: ");
    Serial.println(newData);
    c++;

    Serial.println(nextX);
    Serial.println(nextY);

    // Move the motors
    motors.move(nextX, nextY);

    if (firstPoint && c){
      Serial.println("UP!");
      motors.actuatorUp();
      c = 0;
      firstPoint = false;
    }

    // Reset interrupt
    digitalWrite(2, HIGH);

    // Reset flag
    newData = false;
  }
}

// UNO
// 4 is SDA
// 5 is SCL

// MEGA
// 20 is SDA
// 21 is SCL

void receivedMotorCoords(int howMany) {

    // Let other board know we got it
  digitalWrite(2, LOW);

  
  bool firstByte = true;
  while (Wire.available() > 0) {

    if (firstByte) {
      nextX = Wire.read();

      if ((char)nextX == 'd'){
        Serial.println("D!");
        firstPoint = true;
        motors.actuatorDown();
        c = 0;
        break;
      }
      
      firstByte = false;
    }
    else {
      nextY = Wire.read();
    }
  }

  Serial.print("X:");
  Serial.println(nextX);
  Serial.print("Y:");
  Serial.println(nextY);
  Serial.println();

  newData = true;

}
