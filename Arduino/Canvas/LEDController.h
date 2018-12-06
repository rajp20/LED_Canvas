#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // Variables
    Adafruit_NeoPixel led_strips[18];

    byte waitingDotsState;

    uint32_t current_RGB;

    uint32_t canvas[18][60];

    int state;

    byte bouncingBallState_x;
    byte bouncingBallState_y;
    int bouncingBallState_x_direction;
    int bouncingBallState_y_direction;
    void undrawBall(int x, int y);
    void drawBall(int x, int y, int directionX, int directionY);

    byte rippleRadius;
    byte rippleOrigin_x;
    byte rippleOrigin_y;
    void circleBres(int xo, int yo, int r);
    void drawCircle(int xo, int yo, int x, int y);

    uint32_t Wheel(byte WheelPos, int strip);
  public:
    // Public Variables
    bool acid;
  
    // Setup
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);

    // Helper for convways life
    bool isAlive(int x, int y);
    int getNumberOfNeighbors(int x, int y);
    void drawExplosionConway(int x, int y);
    

    // Update, being called by inturrupts
    void update(void);

    // Clearing and reseting
    void clearCanvas(void);
    void clearCanvasRow(int row);

    void drawPixel(int, int);

    void setColor(int, int, int);

    // Patterns
    void bouncingBall();
    void ripple();
    void conwayLife();
    void conwayLifeInitial();

    // Bluetooth connection waiting
    void waitingDots(void);
};
