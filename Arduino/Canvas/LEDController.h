#define PIN 6   // LED Strip 1

class LEDController {
  void colorWipe(uint32_t, uint8_t);
  public:
    void setup(void);
    void setLED(int, int, int);
};
