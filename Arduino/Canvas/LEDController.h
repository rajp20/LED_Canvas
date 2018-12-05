#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // Variables
    Adafruit_NeoPixel led_strips[18];
    int waitingDotsState;

    uint32_t current_RGB;
  
    uint32_t canvas[18][60];
 
    void undrawBall(int x, int y);
    void drawBall(int x, int y, int directionX, int directionY);

    uint32_t Wheel(byte WheelPos, int strip);
  public:
    // Setup
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);

    // Update, being called by inturrupts
    void update(void);

    // Clearing and reseting
    void clearCanvas(void);
    void clearCanvasRow(int row);
    void resetCanvas(void);
        
    void drawPixel(int, int);

    void setColor(int, int, int);

    // Patterns
    void toggleBouncingBall(bool turnOn);

    // Bluetooth connection waiting
    void waitingDots(void);
};
