#include <AMIS30543.h>

#define X_MOTOR_DIR               2
#define X_MOTOR_STEP              3
#define X_MOTOR_SLAVESELECT       4

#define Y_MOTOR_DIR               5
#define Y_MOTOR_STEP              6
#define Y_MOTOR_SLAVESELECT       7

// Motors
#define X_MOTOR                   true
#define Y_MOTOR                   false
#define X_MAX                     6090
#define Y_MAX                     2820

// Directions
#define LEFT                      1
#define RIGHT                     0
#define UP                        0
#define DOWN                      1

// Actuator
#define ACTUATOR_UP               28
#define ACTUATOR_DOWN             29

// Limit Switches
#define X_LIMIT_SWITCH            27
#define Y_LIMIT_SWITCH            26


class MotorController {
  private:
    AMIS30543 x_motor;
    AMIS30543 y_motor;

    // Setup and calibration
    void setupLimitSwitches(void);
    void setupMotors(void);
    void setupActuator(void);
    void calibrate(void);

    // Motor helpers
    void step(bool motor);
    void setDirection(bool motor, bool direction);

    // Actuator helpers
    void actuatorUp(void);
    void actuatorDown(void);
  public:
    void setup(void);
};
