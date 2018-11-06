#include <Adafruit_NeoPixel.h>
#include "LEDController.h"
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

Adafruit_NeoPixel led_strips[18];

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

  for (int i = 0; i < 18; i++) {
    led_strips[i] = Adafruit_NeoPixel(60, STARTING_LED_STRIP_PIN + i, NEO_GRB + NEO_KHZ800);
    led_strips[i].begin();
    led_strips[i].show(); // Initialize all pixels to 'off'
  }
}

void LEDController::setLED(int red, int green, int blue) {
  colorWipe(led_strips[0].Color(red, green, blue), 0); // Red
}


// Make sure to reset the LEDs if you exit out of this method
void LEDController::toggleBouncingBall(bool turnOn) {

  int current_position_x = 0;
  int current_position_y = 0;

  int directionX = 1;
  int directionY = 1;

  while(true){
    undrawBall(&current_position_x, &current_position_y);
    drawBall(&current_position_x, &current_position_y, &directionX, &directionY);
  }

}


/**
 * PRIVATE
 * HELPER FUNCTIONS
 */

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<led_strips[0].numPixels()/2; i++) {
    led_strips[0].setPixelColor(i, c);
  }
  c = led_strips[0].Color(0, 255, 0);
  for(uint16_t i=led_strips[0].numPixels()/2; i<led_strips[0].numPixels(); i++) {
    led_strips[0].setPixelColor(i, c);
  }
  led_strips[0].show();
}



void undrawBall(int x, int y){

  // Turn off the upper row of the ball
  led_strips[x].setPixelColor(y, led_strips[0].Color(0,0,0))
  led_strips[x].setPixelColor(y + 1, led_strips[0].Color(0,0,0))

  // Turn off the lower row of the ball
  led_strips[x+1].setPixelColor(y, led_strips[0].Color(0,0,0))
  led_strips[x+1].setPixelColor(y + 1, led_strips[0].Color(0,0,0))
}

void drawBall(int &x, int &y, int &directionX, int &directionY){

  // Check the boundaries

  // X boundary
  if (x == led_strips[0].numPixels() - 2 && directionX == 1){
    directionX = -1;
  }
  if (x == 0 && directionX == -1){
    directionX = 1
  }

  // Y boundary
  if (y == 16 && directionY== 1){
    directionY = -1;
  }
  if (y == 0 && directionY == -1){
    directionY = 1
  }

  x += directionX;
  y += directionY;

  // Turn on the upper row of the ball
  led_strips[x].setPixelColor(y, led_strips[0].Color(0,255,0))
  led_strips[x].setPixelColor(y + 1, led_strips[0].Color(0,255,0))

  // Turn on the lower row of the ball
  led_strips[x+1].setPixelColor(y, led_strips[0].Color(0,255,0))
  led_strips[x+1].setPixelColor(y + 1, led_strips[0].Color(0,255,0))


}
