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
#include <MicroView.h>
#include <Time.h>

// Define how big the clock is. Don't make it larger than 23
// This is the radius of the clock:
#define CLOCK_SIZE 23

const uint8_t maxW = uView.getLCDWidth();
const uint8_t midW = maxW/2;
const uint8_t maxH = uView.getLCDHeight();
const uint8_t midH = maxH/2;

volatile bool tilted = false;

void setup()
{
  uView.begin();    // set up the MicroView
  uView.clear(PAGE);// erase hardware memory inside the OLED
  uView.display();  // display the content in the now cleared buffer

  uView.setFontType(2); // set font type 2 (sevensegment)

  // Draw clock face (circle outline & text):
  drawFace();
  
  Serial.begin(9600);
  // Enable a pull up resistor built in to the Arduino controller.
  // FIXME: Is that built in for every pin?
  pinMode(2, INPUT_PULLUP);
  
  // Whenever the state changes on D2, change toggled the tilted bool.
  // NOTE: This uses a lambda, but I don't actually understand lambda syntax in C.
  // FIXME: Just set tilted = true and let the RNG unset it when finished with that action?
  attachInterrupt(digitalPinToInterrupt(2), [] () {tilted = !tilted;}, CHANGE);
}

void loop() 
{
  drawTime();
  uView.invert(tilted);
  Serial.print("D2 = ");
  Serial.print(digitalRead(2));
  Serial.print("; tilted = ");
  Serial.print(tilted);
  Serial.print("; A0 = ");
  Serial.print(analogRead(A0) / 1024.0);
  Serial.println();
}

void drawTime()
{
  static boolean firstDraw = false;
  static unsigned long mSec = millis() + 1000;
  static float degresshour, degressmin, degresssec,
               hourx, houry, minx, miny, secx, secy;

  float actual_degrees;
  float whatever_uView_calls_degrees;
  // If mSec
  if (mSec != (unsigned long)second()) 
  {
    // First time draw requires extra line to set up XOR's:
    // NOTE: "first"Draw is a lie, I think this is undrawing the line from the previous run.
    if (firstDraw) 
    {
      uView.line(midW, midH, 32 + secx, 24 + secy, WHITE, XOR);
    }

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
}

// Draw the clock face. That includes the circle outline and
// the 12, 3, 6, and 9 text.
void drawFace()
{
  uint8_t fontW = uView.getFontWidth();
  uint8_t fontH = uView.getFontHeight();
  
  //uView.setCursor(27, 0); // points cursor to x=27 y=0
  uView.setCursor(midW-fontW-1, midH-CLOCK_SIZE+1);
  uView.print(20);  // Print the "12"
  uView.setCursor(midW-(fontW/2)-1, midH+CLOCK_SIZE-fontH-1);
  uView.print(2);  // Print the "6"
  uView.setCursor(midW-CLOCK_SIZE+1, midH-fontH/2);
  uView.print(6);  // Print the "9"
  uView.setCursor(midW+CLOCK_SIZE-fontW-2, midH-fontH/2);
  uView.print(4);  // Print the "3"
  uView.circle(midW-1, midH-1, CLOCK_SIZE);
  
  //Draw the clock
  uView.display();
}
