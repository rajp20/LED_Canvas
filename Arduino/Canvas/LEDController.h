#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // Variables
    Adafruit_NeoPixel led_strips[18];

    // Pattern states
    byte waitingDotsState;
    byte bouncingBallState_x;
    byte bouncingBallState_y;
    byte bouncingBallState_x_direction;
    byte bouncingBallState_y_direction;

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
        
    void drawPixel(int, int);

    void setColor(int, int, int);

    // Patterns
    void bouncingBall();

    // Bluetooth connection waiting
    void waitingDots(void);
};
