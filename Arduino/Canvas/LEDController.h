#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // Variables
    Adafruit_NeoPixel led_strips[18];
    int waitingDotsState;
    
    void colorWipe(uint32_t, uint8_t);
    void undrawBall(int x, int y);
    void drawBall(int x, int y, int directionX, int directionY, int red, int blue, int green);

    uint32_t Wheel(byte WheelPos, int strip);
  public:
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);
    void resetScreen(void);

    void clearCanvas(void);

    void setLED(int, int, int);
    void toggleBouncingBall(bool turnOn);

    void waitingDots(void);
};
