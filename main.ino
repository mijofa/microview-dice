/*****************************************************************
MicroView_Analog_Clock.ino
SFE_LSM9DS0 Library Simple Example Code
Jim Lindblom @ SparkFun Electronics
Original Creation Date: February 28, 2015
https://github.com/sparkfun/MicroView/tree/master/Libraries

Turn your MicroView into an analog clock!

This sketch requires the Arduino time library. Get it from
here: http://playground.arduino.cc/Code/Time


Development environment specifics:
	IDE: Arduino 1.6.0
	Hardware Platform: MicroView

This code is beerware. If you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, please 
buy us a round!

Distributed as-is; no warranty is given.
*****************************************************************/
#include <Arduino.h>
#include <MicroView.h>

// Define how big the clock is. Don't make it larger than 23
// This is the radius of the clock:
// NOTE: The screen is 47 tall, but since that's not divisible by 2, 23 is the max radius we can set.
#define CLOCK_SIZE 23

const uint8_t maxW = uView.getLCDWidth();
const uint8_t midW = maxW/2;
const uint8_t maxH = uView.getLCDHeight();
const uint8_t midH = maxH/2;
uint8_t digit_width;
uint8_t half_digit_height;

// Might be more accurate to use an unsigned int since it will *always* be positive,
// but I don't need numbers that big.
int roll_result = 0;

// This is updated by the tilt-switch pin interrupt
volatile bool tilted = false;

void drawTime() {
  static boolean firstDraw = false;
  static unsigned long mSec = millis() + 1000;
  static float degresshour, degressmin, degresssec,
               hourx, houry, minx, miny, secx, secy;

  float actual_degrees;
  float whatever_uView_calls_degrees;

  // First time draw requires extra line to set up XOR's:
  // NOTE: "first"Draw is a lie, I think this is undrawing the line from the previous run.
  if (firstDraw) {
    uView.circleFill(midW-1, midH-1, CLOCK_SIZE, BLACK, NORM);
    uView.circle(midW-1, midH-1, CLOCK_SIZE, WHITE, NORM);
//    uView.line(midW, midH, 32 + secx, 24 + secy, WHITE, XOR);
  }

  if (roll_result >= 100) {  // 3 digits
    uView.setCursor(midW - (digit_width * 1.5), midH - half_digit_height);
  } else if (roll_result >= 10) {  // 2 digits
    uView.setCursor(midW - digit_width, midH - half_digit_height);
  } else {  // 1 digit
    uView.setCursor(midW - (digit_width / 2), midH - half_digit_height);
  }
  uView.print(roll_result);



  actual_degrees = (analogRead(A0) / 1024.0) * 360.0;
  actual_degrees = actual_degrees - 180;
  // So basically, I don't understand any of this math. I just adapted it from what the original example used.
  whatever_uView_calls_degrees = (actual_degrees + 270) * (PI / 180);
  degresssec = whatever_uView_calls_degrees;
  //// Calculate second hand degrees:
  //degresssec = (((second() * 360) / 60) + 270) * (PI / 180);

  // Calculate x,y coordinates of second hand:
  secx = cos(degresssec) * (CLOCK_SIZE / 1.1);
  secy = sin(degresssec) * (CLOCK_SIZE / 1.1);

  // Draw hands with the line function:
  uView.line(midW, midH, midW+secx, midH+secy, WHITE, XOR);

  // Set firstDraw flag to true, so we don't do it again.
  // NOTE: Uh, that's a lie, it does that when it is true.
  firstDraw = true;

  // Actually draw the hands with the display() function.
  uView.display();
}

void setup() {
  uView.begin();    // set up the MicroView
  uView.clear(PAGE);// erase hardware memory inside the OLED
  uView.display();  // display the content in the now cleared buffer

  uView.setFontType(2); // set font type 2 (7segment)
  // FIXME: Can I find/make a more D&D/fantasy style font instead?
  //
  digit_width = uView.getFontWidth();
  half_digit_height = uView.getFontHeight() / 2;

  // Draw clock face (circle outline & text):
  drawFace();

  Serial.begin(9600);
  // Enable a pull up resistor built in to the Arduino controller.
  // FIXME: Is that built in for every pin?
  pinMode(2, INPUT_PULLUP);

  // Whenever the state changes on D2, change toggled the tilted bool.
  // NOTE: This uses a lambda, but I don't actually understand lambda syntax in C.
  // FIXME: Just set tilted = true and let the RNG unset it when finished with that action?
  // FIXME: roll_result is not volatile, it should not be getting set directly here
  // FIXME: Should I update the random seed?
  attachInterrupt(digitalPinToInterrupt(2), [] () {tilted = !tilted;roll_result = random(100);}, CHANGE);
}

void loop() {
  drawTime();
  uView.invert(tilted);
  Serial.print("D2 = ");
  Serial.print(digitalRead(2));
  Serial.print("; tilted = ");
  Serial.print(tilted);
  Serial.print("; A0 = ");
  Serial.print(analogRead(A0) / 1024.0);
  Serial.print("; roll = ");
  Serial.print(roll_result);
  Serial.println();
}
