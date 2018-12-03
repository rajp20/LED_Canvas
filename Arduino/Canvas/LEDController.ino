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

  pixelTest();
}

/**
   Tests each pixel with different colors.
*/
void LEDController::pixelTest(void) {
  uint16_t i, j;

  for (int y = 0; y < 18; y++) {
    led_strips[y].clear();
    led_strips[y].show();
  }

  //  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
  for (i = 0; i < 60; i++) {
    for (int strip = 0; strip < 18; strip++) {
      led_strips[strip].setPixelColor(i, Wheel(((i * 256 / 60) + j) & 255, strip));
      led_strips[strip].show();
    }
    delay(50);
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
  int magic_drawing[5][52] =
  { {1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1}
  };

  int connecting[5][41] =
  { {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1}
  };

  for (int y = 0; y < 18; y++) {
    led_strips[y].clear();
    led_strips[y].show();
  }

  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 52; x++) {
      if (magic_drawing[y][x]) {
        led_strips[y + 2].setPixelColor(x + 4, 74, 255, 181);
      }
      else {
        led_strips[y + 2].setPixelColor(x + 4, 0, 0, 0);
      }
    }
    led_strips[y + 2].show();
  }

  for (int y = 0; y < 5; y++) {
    led_strips[y + 9].clear();
    for (int x = 0; x < 41; x++) {
      if (connecting[y][x]) {
        led_strips[y + 9].setPixelColor(x + 8, 74, 255, 181);
      }
      else {
        led_strips[y + 9].setPixelColor(x + 8, 0, 0, 0);
      }
    }
    led_strips[y + 9].show();
  }

}

/**
   Clear screen for when resetting the motors.
   Says "Clearing"
*/
void LEDController::resetScreen(void) {
  int resetting[5][35] =
  { {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0},
    {1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1},
  };

  for (int y = 0; y < 18; y++) {
    led_strips[y].clear();
    led_strips[y].show();
  }

  for (int y = 0; y < 5; y++) {
    for (int x = 0; x < 35; x++) {
      if (resetting[y][x]) {
        led_strips[y + 6].setPixelColor(x + 12, 74, 255, 181);
      }
      else {
        led_strips[y + 6].setPixelColor(x + 12, 0, 0, 0);
      }
    }
    led_strips[y + 6].show();
  }
}

void LEDController::clearCanvas(void) {
  for (int y = 0; y < 18; y++) {
    led_strips[y].clear();
    led_strips[y].show();
  }
}

/**
   Toggles the waiting dots for bluetooth connection.
*/
void LEDController::waitingDots(void) {
  if (waitingDotsState == 0) {
    led_strips[15].clear();
    led_strips[16].clear();
    led_strips[15].setPixelColor(24, 4, 255, 181);
    led_strips[15].setPixelColor(25, 4, 255, 181);
    led_strips[16].setPixelColor(24, 4, 255, 181);
    led_strips[16].setPixelColor(25, 4, 255, 181);
    waitingDotsState = 1;
  } else if (waitingDotsState == 1) {
    led_strips[15].setPixelColor(29, 4, 255, 181);
    led_strips[15].setPixelColor(30, 4, 255, 181);
    led_strips[16].setPixelColor(29, 4, 255, 181);
    led_strips[16].setPixelColor(30, 4, 255, 181);
    waitingDotsState = 2;
  } else if (waitingDotsState == 2) {
    led_strips[15].setPixelColor(34, 4, 255, 181);
    led_strips[15].setPixelColor(35, 4, 255, 181);
    led_strips[16].setPixelColor(34, 4, 255, 181);
    led_strips[16].setPixelColor(35, 4, 255, 181);
    waitingDotsState = 3;
  } else {
    led_strips[15].clear();
    led_strips[16].clear();
    waitingDotsState = 0;
  }
  led_strips[15].show();
  led_strips[16].show();
}

void LEDController::setLED(int red, int green, int blue) {
  colorWipe(led_strips[0].Color(red, green, blue), 0); // Red
}


// Make sure to reset the LEDs if you exit out of this method
void LEDController::toggleBouncingBall(bool turnOn) {

  clearCanvas();

  int current_position_x = 0;
  int current_position_y = 0;

  int directionX = 1;
  int directionY = 1;

  while (true) {
    undrawBall(current_position_x, current_position_y);
    // Check the boundaries

    // X boundary
    if (current_position_x == led_strips[0].numPixels() - 2 && directionX == 1) {
      directionX = -1;
    }
    if (current_position_x == 0 && directionX == -1) {
      directionX = 1;
    }

    // Y boundary
    if (current_position_y == 16 && directionY == 1) {
      directionY = -1;
    }
    if (current_position_y == 0 && directionY == -1) {
      directionY = 1;
    }

    current_position_x += directionX;
    current_position_y += directionY;
    drawBall(current_position_x, current_position_y, directionX, directionY);

    delay(50);
  }

}


/**
   PRIVATE
   HELPER FUNCTIONS
*/

// Fill the dots one after the other with a color
void LEDController::colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < led_strips[0].numPixels() / 2; i++) {
    led_strips[0].setPixelColor(i, c);
  }
  c = led_strips[0].Color(0, 255, 0);
  for (uint16_t i = led_strips[0].numPixels() / 2; i < led_strips[0].numPixels(); i++) {
    led_strips[0].setPixelColor(i, c);
  }
  led_strips[0].show();
}



void LEDController::undrawBall(int x, int y) {

  // Turn off the upper row of the ball
  led_strips[y].setPixelColor(x, led_strips[0].Color(0, 0, 0));
  led_strips[y].setPixelColor(x + 1, led_strips[0].Color(0, 0, 0));

  // Turn off the lower row of the ball
  led_strips[y + 1].setPixelColor(x, led_strips[0].Color(0, 0, 0));
  led_strips[y + 1].setPixelColor(x + 1, led_strips[0].Color(0, 0, 0));

  led_strips[y].show();
  led_strips[y + 1].show();
}

void LEDController::drawBall(int x, int y, int directionX, int directionY) {

  // Turn on the upper row of the ball
  led_strips[y].setPixelColor(x, led_strips[0].Color(0, 255, 0));
  led_strips[y].setPixelColor(x + 1, led_strips[0].Color(0, 255, 0));

  // Turn on the lower row of the ball
  led_strips[y + 1].setPixelColor(x, led_strips[0].Color(0, 255, 0));
  led_strips[y + 1].setPixelColor(x + 1, led_strips[0].Color(0, 255, 0));

  led_strips[y].show();
  led_strips[y + 1].show();
}
