#include <AMIS30543.h>

#define X_LIMIT_SWITCH            1
#define Y_LIMIT_SWITCH            2

#define X_MOTOR_DIR               2
#define X_MOTOR_STEP              3
#define X_MOTOR_SLAVESELECT       4

#define Y_MOTOR_DIR               5
#define Y_MOTOR_STEP              6
#define Y_MOTOR_SLAVESELECT       7

// Motors
#define X_MOTOR                   1
#define Y_MOTOR                   0

// Directions
#define LEFT                      0
#define RIGHT                     1
#define UP                        0
#define DOWN                      1


class MotorController {
  private:
    AMIS30543 x_motor;
    AMIS30543 y_motor;
    void setupLimitSwitches(void);
    void setupMotors(void);
    void step(bool motor);
    void setDirection(bool motor, bool direction);
    void calibrateMotors(void);
  public:
    void setup(void);
};
