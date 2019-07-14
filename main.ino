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
// Would like to set these as const as well, but they can't be set until after choosing a font,
// and that's done in a deeper scope where they would then be unreadable.
uint8_t digit_width;
uint8_t half_digit_height;

// Might be more accurate to use an unsigned int since it will *always* be positive,
// but I don't need numbers that big.
int roll_result = 0;

// This is updated by the tilt-switch pin interrupt
volatile bool tilted = false;


void init_microview() {
  uView.begin();    // set up the MicroView
  uView.clear(PAGE);// erase hardware memory inside the OLED
  uView.display();  // display the content in the now cleared buffer

  uView.setFontType(2); // set font type 2 (7segment)
  // FIXME: Can I find/make a more D&D/fantasy style font instead?
  //
  digit_width = uView.getFontWidth();
  half_digit_height = uView.getFontHeight() / 2;
}


void drawTime() {
  static float degresssec, secx, secy;
  static float actual_degrees;
  static float whatever_uView_calls_degrees;

  // Clear the current face
  {
    uView.circleFill(midW-1, midH-1, CLOCK_SIZE, BLACK, NORM);
    uView.circle(midW-1, midH-1, CLOCK_SIZE, WHITE, NORM);
  }

  // Figure out how wide the text is, so we can offset from the centre of the display before printing
  if (roll_result >= 100) {  // 3 digits
    uView.setCursor(midW - (digit_width * 1.5), midH - half_digit_height);
  } else if (roll_result >= 10) {  // 2 digits
    uView.setCursor(midW - digit_width, midH - half_digit_height);
  } else {  // 1 digit
    uView.setCursor(midW - (digit_width / 2), midH - half_digit_height);
  }
  // Print the current roll result.
  uView.print(roll_result);


  // DEBUGGING
  // Print a line to indicate what the soft-pot on A0 is set to.
  //
  // So basically, I don't understand any of this math. I just adapted it from what the MicroView example code used.
  {
    actual_degrees = (analogRead(A0) / 1024.0) * 360.0;
    actual_degrees = actual_degrees - 180;
    whatever_uView_calls_degrees = (actual_degrees + 270) * (PI / 180);
    degresssec = whatever_uView_calls_degrees;
    //// Calculate second hand degrees:
    //degresssec = (((second() * 360) / 60) + 270) * (PI / 180);

    // Calculate x,y coordinates of second hand:
    secx = cos(degresssec) * (CLOCK_SIZE / 1.1);
    secy = sin(degresssec) * (CLOCK_SIZE / 1.1);

    // Draw hands with the line function:
    uView.line(midW, midH, midW+secx, midH+secy, WHITE, XOR);
  }

  // Actually draw the hands with the display() function.
  uView.display();
}

void setup() {
  init_microview();

  // The analog pins are input by default, just explicitly setting it to self-document.
  pinMode(A0, INPUT);

  // Enable a pull up resistor built in to the Arduino controller.
  // This just means we get a much more reliable result from the tilt switch.
  // FIXME: Is that built in for every pin?
  pinMode(2, INPUT_PULLUP);

  // Whenever the state changes on D2, toggled the tilted bool.
  // NOTE: This uses a lambda, but I don't actually understand lambda syntax in C.
  // FIXME: Just set tilted = true and let the RNG unset it when finished with that action?
  // FIXME: roll_result is not volatile, it should not be getting set directly here
  // FIXME: Should I update the random seed?
  // FIXME: Use digitalPinToInterrupt(), ref: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  attachInterrupt(INT0, [] () {tilted = !tilted;roll_result = random(100);}, CHANGE);

  Serial.begin(9600);  // DEBUGGING

}

void loop() {
  drawTime();

  uView.invert(tilted);  // DEBUGGING

  Serial.print("D2 = ");  // DEBUGGING
  Serial.print(digitalRead(2));  // DEBUGGING
  Serial.print("; tilted = ");  // DEBUGGING
  Serial.print(tilted);  // DEBUGGING
  Serial.print("; A0 = ");  // DEBUGGING
  Serial.print(analogRead(A0) / 1024.0);  // DEBUGGING
  Serial.print("; roll = ");  // DEBUGGING
  Serial.print(roll_result);  // DEBUGGING
  Serial.println();  // DEBUGGING
}
