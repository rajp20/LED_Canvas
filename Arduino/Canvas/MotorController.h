#define X_LIMIT_SWITCH 1
#define Y_LIMIT_SWITCH 2

#define X_MOTOR_DIR = 2;
#define X_MOTOR_STEP = 3;
#define X_MOTOR_SLAVESELECT = 4;

#define Y_MOTOR_DIR = 5;
#define Y_MOTOR_STEP = 6;
#define Y_MOTOR_SLAVESELECT = 7;

class MotorController {
  private:
    AMIS30543 x_motor;
    AMIS30543 y_motor;
    void setupLimitSwitches(void);
    void setupMotors(void);
  public:
    void setup(void);
};
