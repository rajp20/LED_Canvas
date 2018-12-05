#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // Variables
    Adafruit_NeoPixel led_strips[18];
    int waitingDotsState;
    int red;
    int green;
    int blue;

    int canvas[18][60];
    
    void colorWipe(uint32_t, uint8_t);
    void undrawBall(int x, int y);
    void drawBall(int x, int y, int directionX, int directionY, int red, int blue, int green);

    uint32_t Wheel(byte WheelPos, int strip);
  public:
    // Setup
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);
    
    void clearCanvas(void);
    void resetScreen(void);

    void drawPixel(int, int);

    void setRGB(int, int, int);

    // Patterns
    void toggleBouncingBall(bool turnOn);

    // Bluetooth connection waiting
    void waitingDots(void);
};
