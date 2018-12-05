//#include "LEDController.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void LEDController::setup(void) {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  // Initialize all of the LED strips
  for (int i = 0; i < 18; i++) {
    led_strips[i] = Adafruit_NeoPixel(60, STARTING_LED_STRIP_PIN + i, NEO_GRB + NEO_KHZ800);
    led_strips[i].setBrightness(75);
    led_strips[i].begin();
    led_strips[i].show(); // Initialize all pixels to 'off'
  }

  waitingDotsState = 0;

  bouncingBallState_x = 0;
  bouncingBallState_y = 0;
  bouncingBallState_x_direction = 1;
  bouncingBallState_y_direction = 1;

  acid = false;

  byte rippleRadius = 1;
  byte rippleOrigin_x = random(1, 59);
  byte rippleOrigin_y = random(1, 17);

  current_RGB = ((uint32_t)74 << 16) | ((uint32_t)255 <<  8) | 181;
}

/**
  Updates and shows all the pixel on the screen.
*/
void LEDController::update(void) {
  //  ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 60; x++) {
      led_strips[y].setPixelColor(x, canvas[y][x]);
    }
    led_strips[y].show();
  }
}

/**
   Tests each pixel with different colors.
*/
void LEDController::pixelTest(void) {
  uint16_t i, j;

  clearCanvas();

  //  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
  for (i = 0; i < 60; i++) {
    for (int strip = 0; strip < 18; strip++) {
      canvas[strip][i] =  Wheel(((i * 256 / 60) + j) & 255, strip);
    }
    update();
    delay(25);
  }
  //  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LEDController::Wheel(byte WheelPos, int strip) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return led_strips[strip].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return led_strips[strip].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return led_strips[strip].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


/**
   Welcome screen for when the system boots up.
   Says "Magic Drawing"
*/
void LEDController::welcomeScreen(void) {
  boolean magic_drawing[5][52] =
  { {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1}
  };

  boolean connecting[5][41] =
  { {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1}
  };

  clearCanvas();
  setColor(74, 255, 181);

  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 52; x++) {
      if (magic_drawing[y][x]) {
        canvas[y + 2][x + 4] = current_RGB;
      }
    }
  }

  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 41; x++) {
      if (connecting[y][x]) {
        canvas[y + 9][x + 8] = current_RGB;
      }
    }
  }

  update();
}

/**
  Clears the entire canvas.
*/
void LEDController::clearCanvas(void) {
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 60; x++) {
      canvas[y][x] = 0;
    }
  }
  update();
}

/**
  Clears the specified row.
*/
void LEDController::clearCanvasRow(int row) {
  for (int x = 0; x < 60; x++) {
    canvas[row][x] = 0;
  }
  update();
}

/**
   Toggles the waiting dots for bluetooth connection.
*/
void LEDController::waitingDots(void) {
  if (waitingDotsState == 0) {
    clearCanvasRow(15);
    clearCanvasRow(16);
    canvas[15][24] = current_RGB;
    canvas[15][25] = current_RGB;
    canvas[16][24] = current_RGB;
    canvas[16][25] = current_RGB;
    waitingDotsState = 1;
  } else if (waitingDotsState == 1) {
    canvas[15][29] = current_RGB;
    canvas[15][30] = current_RGB;
    canvas[16][29] = current_RGB;
    canvas[16][30] = current_RGB;
    waitingDotsState = 2;
  } else if (waitingDotsState == 2) {
    canvas[15][34] = current_RGB;
    canvas[15][35] = current_RGB;
    canvas[16][34] = current_RGB;
    canvas[16][35] = current_RGB;
    waitingDotsState = 3;
  } else {
    clearCanvasRow(15);
    clearCanvasRow(16);
    waitingDotsState = 0;
  }
  update();
}

void LEDController::drawPixel(int x, int y) {
  canvas[y][x] = current_RGB;
  update();
}

/**
  Sets the current RGB color.
*/
void LEDController::setColor(int red, int green, int blue) {
  current_RGB = ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
}


// Make sure to reset the LEDs if you exit out of this method
void LEDController::bouncingBall() {
  int lowerBound = 1;
  int upperBound = 255;

  undrawBall(bouncingBallState_x, bouncingBallState_y);
  // Check the boundaries

  // X boundary
  if (bouncingBallState_x == led_strips[0].numPixels() - 2 && bouncingBallState_x_direction == 1) {
    bouncingBallState_x_direction = -1;
    setColor(random(lowerBound, upperBound), random(lowerBound, upperBound), random(lowerBound, upperBound));
  }
  if (bouncingBallState_x == 0 && bouncingBallState_x_direction == -1) {
    bouncingBallState_x_direction = 1;
    setColor(random(lowerBound, upperBound), random(lowerBound, upperBound), random(lowerBound, upperBound));
  }

  // Y boundary
  if (bouncingBallState_y == 16 && bouncingBallState_y_direction == 1) {
    bouncingBallState_y_direction = -1;
    setColor(random(lowerBound, upperBound), random(lowerBound, upperBound), random(lowerBound, upperBound));
  }
  if (bouncingBallState_y == 0 && bouncingBallState_y_direction == -1) {
    bouncingBallState_y_direction = 1;
    setColor(random(50, 255), random(50, 255), random(50, 255));
  }

  bouncingBallState_x += bouncingBallState_x_direction;
  bouncingBallState_y += bouncingBallState_y_direction;
  drawBall(bouncingBallState_x, bouncingBallState_y, bouncingBallState_x_direction, bouncingBallState_y_direction);

  update();
}

void LEDController::undrawBall(int x, int y) {

  // Turn off the upper row of the ball
  canvas[y][x] = 0;
  canvas[y][x + 1] = 0;

  // Turn off the lower row of the ball
  canvas[y + 1][x] = 0;
  canvas[y + 1][x + 1] = 0;
}

void LEDController::drawBall(int x, int y, int directionX, int directionY) {

  // Turn on the upper row of the ball
  canvas[y][x] = current_RGB;
  canvas[y][x + 1] = current_RGB;

  // Turn on the lower row of the ball
  canvas[y + 1][x] = current_RGB;
  canvas[y + 1][x + 1] = current_RGB;
}

void LEDController::ripple() {
  if (!acid) {
    //    circleBres(rippleOrigin_x, rippleOrigin_y, rippleRadius - 1, true);
    clearCanvas();
  }
  circleBres(rippleOrigin_x, rippleOrigin_y, rippleRadius, false);
  rippleRadius++;
  setColor(random(25, 255), random(25, 255), random(25, 255));
  update();
}

void LEDController::circleBres(int xo, int yo, int r, bool erase) {
  int x = 0, y = r;
  int d = 3 - 2 * r;
  drawCircle(xo, yo, x, y, erase ? 0 : current_RGB);
  while (y >= x)
  {
    // for each pixel we will
    // draw all eight pixels

    x++;

    // check for decision parameter
    // and correspondingly
    // update d, x, y
    if (d > 0)
    {
      y--;
      d = d + 4 * (x - y) + 10;
    }
    else {
      d = d + 4 * x + 6;
    }
    drawCircle(xo, yo, x, y, erase ? 0 : current_RGB);
  }
}

void LEDController::drawCircle(int xo, int yo, int x, int y, uint32_t color) {
  byte inbound = 0;
  if ((xo + x < 60) && (yo + y < 18)) {
    canvas[yo + y][xo + x] = color;
    inbound++;
  }
  if ((xo - x >= 0) && (yo + y < 18)) {
    canvas[yo + y][xo - x] = color;
    inbound++;
  }
  if ((xo + x < 60) && (yo - y >= 0)) {
    canvas[yo - y][xo + x] = color;
    inbound++;
  }
  if ((xo - x >= 0) && (yo - y >= 0)) {
    canvas[yo - y][xo - x] = color;
    inbound++;
  }
  if ((xo + y < 60) && (yo + x < 18)) {
    canvas[yo + x][xo + y] = color;
    inbound++;
  }
  if ((xo - y >= 0) && (yo + x < 18)) {
    canvas[yo + x][xo - y] = color;
    inbound++;
  }
  if ((xo + y < 60) && (yo - x >= 0)) {
    canvas[yo - x][xo + y] = color;
    inbound++;
  }
  if ((xo - y >= 0) && (yo - x >= 0)) {
    canvas[yo - x][xo - y] = color;
    inbound++;
  }
  if (inbound == 0) {
    rippleRadius = 0;
    rippleOrigin_x = random(1, 59);
    rippleOrigin_y = random(1, 17);
  }
}
