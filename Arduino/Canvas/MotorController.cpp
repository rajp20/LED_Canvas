#include "MotorController.h"
#include <SPI.h>

void MotorController::setup(void) {
//  setupLimitSwitches();
//  setupMotors();
  setupActuator();
  calibrate();

  //  // Step in the default direction 1000 times.
  //  setDirection(0, 0);
  //  setDirection(0, 1);
  //  for (unsigned int x = 0; x < 2000; x++)
  //  {
  //    step(0);
  //    step(1);
  //  }
  //
  //  // Wait for 300 ms.
  //  delay(300);
  //
  //  // Step in the other direction 1000 times.
  //  setDirection(1, 0);
  //  setDirection(1, 1);
  //  for (unsigned int x = 0; x < 2000; x++)
  //  {
  //    step(0);
  //    step(1);
  //  }
  //
  //  // Wait for 300 ms.
  //  delay(300);
}

/**
   Sets up the X and Y limit switches pins.
*/
void MotorController::setupLimitSwitches(void) {
  pinMode(X_LIMIT_SWITCH, INPUT);
  pinMode(Y_LIMIT_SWITCH, INPUT);
}

/**
   Sets up the pins for the actuator.
*/
void MotorController::setupActuator(void) {
  pinMode(ACTUATOR_UP, OUTPUT);
  pinMode(ACTUATOR_DOWN, OUTPUT);
}

/**
   Configures the SPI connection and the motor settings for the X and Y motors.
*/
void MotorController::setupMotors(void) {
  SPI.begin();
  x_motor.init(X_MOTOR_SLAVESELECT);
  y_motor.init(Y_MOTOR_SLAVESELECT);
  delay(1);

  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(X_MOTOR_STEP, LOW);
  pinMode(X_MOTOR_STEP, OUTPUT);
  digitalWrite(X_MOTOR_DIR, LOW);
  pinMode(X_MOTOR_DIR, OUTPUT);

  digitalWrite(Y_MOTOR_STEP, LOW);
  pinMode(Y_MOTOR_STEP, OUTPUT);
  digitalWrite(Y_MOTOR_DIR, LOW);
  pinMode(Y_MOTOR_DIR, OUTPUT);

  x_motor.resetSettings();
  x_motor.setCurrentMilliamps(1800);
  x_motor.setStepMode(1);
  x_motor.enableDriver();

  y_motor.resetSettings();
  y_motor.setCurrentMilliamps(1800);
  y_motor.setStepMode(1);
  y_motor.enableDriver();
}

/**
   Bring the motors to home position. Also makes the actuator
   go up and down to check if it is working properly.
*/
void MotorController::calibrate(void) {
//  setDirection(X_MOTOR, LEFT);
//  setDirection(Y_MOTOR, UP);

  actuatorUp();
//  while (true) {
//    if (digitalRead(X_LIMIT_SWITCH) == LOW && digitalRead(Y_LIMIT_SWITCH) == LOW) {
//      break;
//    }
//    if (digitalRead(X_LIMIT_SWITCH) != LOW) {
//      step(X_MOTOR);
//    }
//    if (digitalRead(Y_LIMIT_SWITCH) != LOW) {
//      step(Y_MOTOR);
//    }
//  }
  delay(3000);
  actuatorDown();
}

/**
   Sends a pulse on the NXT/STEP pin to tell the driver to take
   one step, and also delays to control the speed of the motor.
*/
void MotorController::step(bool motor)
{
  if (motor == X_MOTOR) {
    // The NXT/STEP minimum high pulse width is 2 microseconds.
    digitalWrite(X_MOTOR_STEP, HIGH);
    delayMicroseconds(3);
    digitalWrite(X_MOTOR_STEP, LOW);
    delayMicroseconds(3);
  }
  else {
    // The NXT/STEP minimum high pulse width is 2 microseconds.
    digitalWrite(Y_MOTOR_STEP, HIGH);
    delayMicroseconds(3);
    digitalWrite(Y_MOTOR_STEP, LOW);
    delayMicroseconds(3);
  }


  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(1000);
}

/**
   Write a high or low value to the direction pin to specify
   what direction to turn the motor.
*/
// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void MotorController::setDirection(bool motor, bool dir)
{
  if (motor == X_MOTOR) {
    // The NXT/STEP pin must not change for at least 0.5
    // microseconds before and after changing the DIR pin.
    delayMicroseconds(1);
    digitalWrite(X_MOTOR_DIR, dir);
    delayMicroseconds(1);
  }
  else {
    // The NXT/STEP pin must not change for at least 0.5
    // microseconds before and after changing the DIR pin.
    delayMicroseconds(1);
    digitalWrite(Y_MOTOR_DIR, dir);
    delayMicroseconds(1);
  }
}

/**
   Brings the actuator all the way out.
*/
void MotorController::actuatorUp(void) {
  digitalWrite(ACTUATOR_UP, HIGH);
  digitalWrite(ACTUATOR_DOWN, LOW);
}

/**
   Brings the actuator all the way in.
*/
void MotorController::actuatorDown(void) {
  digitalWrite(ACTUATOR_DOWN, HIGH);
  digitalWrite(ACTUATOR_UP, LOW);
}
