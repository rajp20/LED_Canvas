#include <AMIS30543.h>
//#include <AccelStepper.h>
//#include <MultiStepper.h>
#include <SPI.h>
#include <Arduino.h>

#define X_MOTOR_DIR               6
#define X_MOTOR_STEP              7
#define X_MOTOR_SLAVESELECT       3 // 4  // 8

#define Y_MOTOR_DIR               10 // 5  // 0
#define Y_MOTOR_STEP              9
#define Y_MOTOR_SLAVESELECT       8

// Motors
#define X_MOTOR                   true
#define Y_MOTOR                   false
#define X_MAX                     6094
#define Y_MAX                     2790

// Directions
#define LEFT                      1
#define RIGHT                     0
#define UP                        0
#define DOWN                      1

// The amount of steps needed to move one LED position in the X and Y direction

// Actuator
#define ACTUATOR_UP               5 // 28
#define ACTUATOR_DOWN             4 // 29

// Limit Switches
#define X_LIMIT_SWITCH            1 // 27
#define Y_LIMIT_SWITCH            0 // 26


class MotorController {
  private:
    AMIS30543 x_motor;
    AMIS30543 y_motor;

//    // MultiStepper library used for stepping multiple steppers
//    MultiStepper multiStep;
//
//    // Turn on the accelSteppers
//    AccelStepper accelStepperX;
//    AccelStepper accelStepperY;

    long STEP_X = 6094/60;
    long STEP_Y = 2790/18;


    // Current position of the Motors
    int curX;
    int curY;

    // Setup and calibration
    void setupLimitSwitches(void);
    void setupMotors(void);
    void setupActuator(void);
    void calibrate(void);

    // Motor helpers
    void step(bool motor);
    void setDirection(bool motor, bool direction);


  public:
    // Actuator helpers
    void actuatorUp(void);
    void actuatorDown(void);
    void stopActuator(void);
    void setup(void);
    void move(int x, int y);

    bool run();
};
