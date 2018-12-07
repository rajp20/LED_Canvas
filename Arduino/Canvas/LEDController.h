#include <Adafruit_NeoPixel.h>
#define STARTING_LED_STRIP_PIN 30     // Starting LED Strip PIN number

class LEDController {
  private:
    // 18 LED Strips
    Adafruit_NeoPixel led_strips[18];

    // Main Canvas. This is basically the current 'frame' that is being displayed.
    uint32_t canvas[18][60];

    // The current RGB color for the 'paint brush'.
    uint32_t current_RGB;

    // The state of the loading bar for welcome screen.
    byte waiting_state;

    // BOUNCING BALL EFFECT
    // Bouncing Ball states for (x, y) and their direction.
    byte bouncingBallState_x;
    byte bouncingBallState_y;
    int bouncingBallState_x_direction;
    int bouncingBallState_y_direction;

    //RIPPLE EFFECT
    // Ripple effect radius and (x, y) origin.
    byte rippleRadius;
    byte rippleOrigin_x;
    byte rippleOrigin_y;

    void drawBall(int x, int y, uint32_t);

    void circleBres(int xo, int yo, int r);
    void drawCircle(int xo, int yo, int x, int y);

    uint32_t Wheel(byte WheelPos, int strip);

    uint32_t encodeColor(uint8_t red, uint8_t green, uint8_t blue);

    // Helper for convways life
    bool isAlive(int x, int y);
    int getNumberOfNeighbors(int x, int y);
    void drawExplosionConway(int x, int y);


  public:
    // Toggles acid on/off.
    bool acid;

    // Setup
    void setup(void);
    void pixelTest(void);
    void welcomeScreen(void);

    void update(void);

    // Clearing and reseting
    void clearCanvas(void);
    void clearCanvasRow(int row);

    void drawPixel(int, int);
    void drawBox(int, int);
    void fadeOutCanvas(int fade_out);
    void fadeOutRow(int fade_out, int row);

    void setColor(int, int, int);

    // Patterns
    void bouncingBall();
    void ripple();
    void conwayLife();
    void conwayLifeInitial();

    // Bluetooth connection waiting
    void waiting(void);
};
