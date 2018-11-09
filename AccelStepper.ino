/* This example shows how to use the AMIS-30543 stepper motor
driver with the AccelStepper library.

You will need to install the AccelStepper library for this
example to work.  Documentation and installation instructions for
the AccelStepper library are available here:

    http://www.airspayce.com/mikem/arduino/AccelStepper/

Before using this example, be sure to change the
setCurrentMilliamps line to have an appropriate current limit for
your system.  Also, see this library's documentation for
information about how to connect the driver:

    http://pololu.github.io/amis-30543-arduino/
*/

#include <SPI.h>
#include <AMIS30543.h>
#include <AccelStepper.h>

const uint8_t amisDirPin1 = 2;
const uint8_t amisStepPin1 = 3;
const uint8_t amisSlaveSelect1 = 4;

const uint8_t amisDirPin2 = 5;
const uint8_t amisStepPin2 = 6;
const uint8_t amisSlaveSelect2 = 7;

AMIS30543 stepper1;
AMIS30543 stepper2;
//AccelStepper accelStepper(AccelStepper::DRIVER, amisStepPin1, amisDirPin1);
//AccelStepper accelStepper2(AccelStepper::DRIVER, amisStepPin2, amisDirPin2);


void setup()
{
  SPI.begin();
  stepper1.init(amisSlaveSelect1);
  stepper2.init(amisSlaveSelect2);
  delay(1);

  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amisStepPin1, LOW);
  pinMode(amisStepPin1, OUTPUT);
  digitalWrite(amisDirPin1, LOW);
  pinMode(amisDirPin1, OUTPUT);

  digitalWrite(amisStepPin2, LOW);
  pinMode(amisStepPin2, OUTPUT);
  digitalWrite(amisDirPin2, LOW);
  pinMode(amisDirPin2, OUTPUT);

  stepper1.resetSettings();
  stepper1.setCurrentMilliamps(1800);
  stepper1.setStepMode(1);
  stepper1.enableDriver();

  stepper2.resetSettings();
  stepper2.setCurrentMilliamps(1800);
  stepper2.setStepMode(1);
  stepper2.enableDriver();

  
}

void loop()
{
   // Step in the default direction 1000 times.
  setDirection(0, 0);
  setDirection(0, 1);
  for (unsigned int x = 0; x < 2000; x++)
  {
    step(0);
    step(1);
  }

  // Wait for 300 ms.
  delay(300);

  // Step in the other direction 1000 times.
  setDirection(1, 0);
  setDirection(1, 1);
  for (unsigned int x = 0; x < 2000; x++)
  {
    step(0);
    step(1);
  }

  // Wait for 300 ms.
  delay(300);
}

// Sends a pulse on the NXT/STEP pin to tell the driver to take
// one step, and also delays to control the speed of the motor.
void step(bool stepper)
{
  if (stepper){
    // The NXT/STEP minimum high pulse width is 2 microseconds.
    digitalWrite(amisStepPin1, HIGH);
    delayMicroseconds(3);
    digitalWrite(amisStepPin1, LOW);
    delayMicroseconds(3);
  }
  else {
    // The NXT/STEP minimum high pulse width is 2 microseconds.
    digitalWrite(amisStepPin2, HIGH);
    delayMicroseconds(3);
    digitalWrite(amisStepPin2, LOW);
    delayMicroseconds(3);
  }


  // The delay here controls the stepper motor's speed.  You can
  // increase the delay to make the stepper motor go slower.  If
  // you decrease the delay, the stepper motor will go fast, but
  // there is a limit to how fast it can go before it starts
  // missing steps.
  delayMicroseconds(1000);
}

// Writes a high or low value to the direction pin to specify
// what direction to turn the motor.
void setDirection(bool dir, bool stepper)
{
  if (stepper){
    // The NXT/STEP pin must not change for at least 0.5
    // microseconds before and after changing the DIR pin.
    delayMicroseconds(1);
    digitalWrite(amisDirPin1, dir);
    delayMicroseconds(1);
  }
  else {
    // The NXT/STEP pin must not change for at least 0.5
    // microseconds before and after changing the DIR pin.
    delayMicroseconds(1);
    digitalWrite(amisDirPin2, dir);
    delayMicroseconds(1);
  }

}
