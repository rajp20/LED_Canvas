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
    led_strips[i].setBrightness(200);
    led_strips[i].begin();
    led_strips[i].show(); // Initialize all pixels to 'off'
  }

  waiting_state = 0;

  bouncingBallState_x = 0;
  bouncingBallState_y = 0;
  bouncingBallState_x_direction = 1;
  bouncingBallState_y_direction = 1;

  acid = false;

  byte rippleRadius = 1;
  byte rippleOrigin_x = random(1, 59);
  byte rippleOrigin_y = random(1, 17);

  current_RGB = encodeColor(74, 255, 181);
  //  current_RGB = ((uint32_t)74 << 16) | ((uint32_t)255 <<  8) | 181;
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
  Fades out the canvas by subtracting the given number from the RGB
  values.
*/
void LEDController::fadeOutCanvas(int fade_out) {
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 60; x++) {
      if (canvas[y][x] != 0) {
        uint8_t red = canvas[y][x] >> 16;
        uint8_t green = canvas[y][x] >> 8;
        uint8_t blue = canvas[y][x];
        canvas[y][x] = encodeColor(red / fade_out, green / fade_out, blue / fade_out);
      }
    }
  }
  update();
}

/**
  Fades out the canvas by subtracting the given number from the RGB
  values.
*/
void LEDController::fadeOutRow(int fade_out, int row) {
  for (int x = 0; x < 60; x++) {
    if (canvas[row][x] != 0) {
      uint8_t red = canvas[row][x] >> 16;
      uint8_t green = canvas[row][x] >> 8;
      uint8_t blue = canvas[row][x];
      uint32_t faded_color = encodeColor(red / fade_out, green / fade_out, blue / fade_out);
      canvas[row][x] = faded_color;
    }
    led_strips[row].setPixelColor(x, canvas[row][x]);
  }
  led_strips[row].show();

}

/**
  Draws a 2x2 pixel onto the canvas
*/
void LEDController::drawBox(uint8_t x, uint8_t y) {
  //  fadeOutCanvas(2);
  canvas[y][x] = current_RGB;
  if (y + 1 < 18) {
    canvas[y + 1][x] = current_RGB;
    if (x + 1 < 60) {
      canvas[y + 1][x + 1] = current_RGB;
    }
  }
  if (x + 1 < 60) {
    canvas[y][x + 1] = current_RGB;
  }
  update();
}

/**
  Sets the pixel at (x, y) with the current RGB color.
*/
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

uint32_t LEDController::encodeColor(uint8_t red, uint8_t green, uint8_t blue) {
  if (red < 0) {
    red = 0;
  }
  if (green < 0) {
    green = 0;
  }
  if (blue < 0) {
    blue = 0;
  }
  if (red == 0 && green == 0 && blue == 0) {
    return 0;
  }
  return ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
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
void LEDController::waiting(void) {
  fadeOutRow(2, 15);
  fadeOutRow(2, 16);
  canvas[15][18 + waiting_state] = current_RGB;
  canvas[16][18 + waiting_state] = current_RGB;
  waiting_state++;
  if (waiting_state > 26) {
    waiting_state = 0;
  }
  //  if (waitingDotsState == 0) {
  //    clearCanvasRow(15);
  //    clearCanvasRow(16);
  //    canvas[15][24] = current_RGB;
  //    canvas[15][25] = current_RGB;
  //    canvas[16][24] = current_RGB;
  //    canvas[16][25] = current_RGB;
  //    waitingDotsState = 1;
  //  } else if (waitingDotsState == 1) {
  //    canvas[15][29] = current_RGB;
  //    canvas[15][30] = current_RGB;
  //    canvas[16][29] = current_RGB;
  //    canvas[16][30] = current_RGB;
  //    waitingDotsState = 2;
  //  } else if (waitingDotsState == 2) {
  //    canvas[15][34] = current_RGB;
  //    canvas[15][35] = current_RGB;
  //    canvas[16][34] = current_RGB;
  //    canvas[16][35] = current_RGB;
  //    waitingDotsState = 3;
  //  } else {
  //    clearCanvasRow(15);
  //    clearCanvasRow(16);
  //    waitingDotsState = 0;
  //  }
  update();
}

// Make sure to reset the LEDs if you exit out of this method
void LEDController::bouncingBall() {
  int lowerBound = 1;
  int upperBound = 255;

  drawBall(bouncingBallState_x, bouncingBallState_y, 0);
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
  drawBall(bouncingBallState_x, bouncingBallState_y, current_RGB);

  update();
}

void LEDController::drawBall(int x, int y, uint32_t color) {

  // Turn on the upper row of the ball
  canvas[y][x] = color;
  canvas[y][x + 1] = color;

  // Turn on the lower row of the ball
  canvas[y + 1][x] = color;
  canvas[y + 1][x + 1] = color;
}

bool LEDController::isAlive(int x, int y) {
  bool alive = (canvas[x][y] >> 16) & (0x1);
  return alive;
}

int LEDController::getNumberOfNeighbors(int x, int y) {
  int numberOfNeighbors = 0;

  int xLeftIndex = x - 1;
  int xRightIndex = x + 1;
  int yUpIndex = y - 1;
  int yDownIndex = y + 1;

  // Check x bounds
  if (x == 0) {
    xLeftIndex = 17;
  }

  if (x == 17) {
    xRightIndex = 0;
  }

  // Check y bounds
  if (y == 0) {
    yUpIndex = 59;
  }

  if (y == 59) {
    yDownIndex = 0;
  }

  // Add up all 8 neighbors
  // Top Row
  numberOfNeighbors += isAlive(xLeftIndex, yUpIndex);
  numberOfNeighbors += isAlive(x, yUpIndex);
  numberOfNeighbors += isAlive(xRightIndex, yUpIndex);

  // Middle Row
  numberOfNeighbors += isAlive(xLeftIndex, y);
  numberOfNeighbors += isAlive(xRightIndex, y);

  // Bottom Row
  numberOfNeighbors += isAlive(xLeftIndex, yDownIndex);
  numberOfNeighbors += isAlive(x, yDownIndex);
  numberOfNeighbors += isAlive(xRightIndex, yDownIndex);


  return numberOfNeighbors;
}

void LEDController::conwayLife() {

  bool changed = false;

  int numNeighbors;
  for (int x = 0; x < 18; x++) {
    for (int y = 0; y < 60; y++) {


      numNeighbors = getNumberOfNeighbors(x, y);

      // Dies
      if (isAlive(x, y) && numNeighbors < 2) {
        changed = true;
        continue;
      }
      // Dies
      if (isAlive(x, y) && numNeighbors > 3) {
        changed = true;
        continue;
      }

      // Born
      if (!isAlive(x, y) && numNeighbors == 3) {
        changed = true;
        // Set upper bits to high
        //        canvas[x][y] = encodeColor(0, 0, 255);
        canvas[x][y] = 0;
        canvas[x][y] |= 0xFF000000;
        continue;
      }

      // Cell remains in its same state
      if (isAlive(x, y) && (numNeighbors == 3 || numNeighbors == 2)) {
        // Shift the current bits to future state
        canvas[x][y] &= 0x00FF0000;
        canvas[x][y] |= (canvas[x][y] << 8);
        continue;
      }
    }
  }

  // If nothing changed this state, reintialize
  //  if (changed == false) {
  //    conwayLifeInitial();
  //    update();
  //    return;
  //  }

  // Update to future state
  for (int x = 0; x < 18; x++) {
    for (int y = 0; y < 60; y++) {
      // Clear the current state bits
      canvas[x][y] &= 0xFF000000 | canvas[x][y];
      // Shift future state to current state
      canvas[x][y] >>= 8;
    }
  }

  update();
}

void LEDController::conwayLifeInitial() {
  clearCanvas();
  setColor(255, 0, 0);
  for (int i = 0; i < 18; i++) {
    for (int j = 0; j < 60; j++) {
      int shouldSetColor = random(0, 100);
      if (shouldSetColor > 50) {
        canvas[i][j] = current_RGB;
      }
    }
  }

  //  int x = random(2, 15);
  //  int y = random(3, 57);
  //
  //  drawExplosionConway(x, y);

  update();
}

void LEDController::drawExplosionConway(int x, int y) {
  // Small explosion intialization
  canvas[x][y] = current_RGB;
  canvas[x + 1][y - 1] = current_RGB;
  canvas[x + 1][y] = current_RGB;
  canvas[x + 1][y + 1] = current_RGB;
  canvas[x + 2][y - 1] = current_RGB;
  canvas[x + 2][y + 1] = current_RGB;
  canvas[x + 3][y] = current_RGB;

  //  canvas[8][30] = current_RGB;
  //  canvas[9][29] = current_RGB;
  //  canvas[9][30] = current_RGB;
  //  canvas[9][31] = current_RGB;
  //  canvas[10][29] = current_RGB;
  //  canvas[10][31] = current_RGB;
  //  canvas[11][30] = current_RGB;

}

/**
  Creates a ripple effect on canvas.
*/
void LEDController::ripple() {
  if (!acid) {
    //    clearCanvas();
    fadeOutCanvas(2);
  }

  circleBres(rippleOrigin_x, rippleOrigin_y, rippleRadius);
  rippleRadius++;

  setColor(random(50, 255), random(50, 255), random(50, 255));
  update();
}

///**
//  Creates a single ripple effect when the iOS app detects too much
//  user data.
// */
void LEDController::overflow() {
  clearCanvas();
  rippleRadius = 1;
  while (true) {
    circleBres(29, 8, rippleRadius);
    rippleRadius++;
    setColor(random(0, 255), random(0, 255), random(0, 255));
    update();
    if (rippleRadius == 1) {
      break;
    }
  }
}

void LEDController::circleBres(int xo, int yo, int r) {
  int x = 0, y = r;
  int d = 3 - 2 * r;
  drawCircle(xo, yo, x, y);
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
    drawCircle(xo, yo, x, y);
  }
}

void LEDController::drawCircle(int xo, int yo, int x, int y) {
  byte inbound = 0;
  if ((xo + x < 60) && (yo + y < 18)) {
    canvas[yo + y][xo + x] = current_RGB;
    inbound++;
  }
  if ((xo - x >= 0) && (yo + y < 18)) {
    canvas[yo + y][xo - x] = current_RGB;
    inbound++;
  }
  if ((xo + x < 60) && (yo - y >= 0)) {
    canvas[yo - y][xo + x] = current_RGB;
    inbound++;
  }
  if ((xo - x >= 0) && (yo - y >= 0)) {
    canvas[yo - y][xo - x] = current_RGB;
    inbound++;
  }
  if ((xo + y < 60) && (yo + x < 18)) {
    canvas[yo + x][xo + y] = current_RGB;
    inbound++;
  }
  if ((xo - y >= 0) && (yo + x < 18)) {
    canvas[yo + x][xo - y] = current_RGB;
    inbound++;
  }
  if ((xo + y < 60) && (yo - x >= 0)) {
    canvas[yo - x][xo + y] = current_RGB;
    inbound++;
  }
  if ((xo - y >= 0) && (yo - x >= 0)) {
    canvas[yo - x][xo - y] = current_RGB;
    inbound++;
  }
  if (inbound == 0) {
    rippleRadius = 0;
    rippleOrigin_x = random(1, 59);
    rippleOrigin_y = random(1, 17);
  }
}
