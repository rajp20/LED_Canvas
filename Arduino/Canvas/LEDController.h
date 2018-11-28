#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 21     // Starting LED Strip PIN number

class LEDController {
  private:
    Adafruit_NeoPixel led_strips[18];
    void colorWipe(uint32_t, uint8_t);
    void undrawBall(int x, int y);
    void drawBall(int x, int y, int directionX, int directionY);
  public:
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);
    void resetScreen(void);
    
    void setLED(int, int, int);
    void toggleBouncingBall(bool turnOn);
};
