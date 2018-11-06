#define STARTING_LED_STRIP_PIN 6     // Starting LED Strip PIN number

class LEDController {
  void colorWipe(uint32_t, uint8_t);
  public:
    void setup(void);
    void setLED(int, int, int);
    void toggleBouncingBall(bool turnOn);
};
