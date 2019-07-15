#include <Arduino.h>
#include <MicroView.h>

#include <dice-functions.h>

// This is updated by the tilt-switch pin interrupt
volatile bool tilted = false;

void redraw_dice() {
    static float degresssec, secx, secy;
    static float actual_degrees;
    static float whatever_uView_calls_degrees;

    // D4
    draw_face_d4();
    // D6
    draw_face_d6();

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
        secx = cos(degresssec) * (23 / 1.1);
        secy = sin(degresssec) * (23 / 1.1);

        // Draw hands with the line function:
        uView.line(screen_horizontal_centre, screen_vertical_centre, screen_horizontal_centre+secx, screen_vertical_centre+secy, WHITE, XOR);
    }

    // Actually draw the hands with the display() function.
    uView.display();
}

void setup() {
    uView.begin();    // set up the MicroView
    uView.clear(PAGE);// erase hardware memory inside the OLED
    uView.display();  // display the content in the now cleared buffer

    // The analog pins are input by default, just explicitly setting it to self-document.
    pinMode(A0, INPUT);

    // Enable a pull up resistor built in to the Arduino controller.
    // This just means we get a much more reliable result from the tilt switch.
    // FIXME: Is that built in for every pin?
    pinMode(2, INPUT_PULLUP);

    // Whenever the state changes on D2, toggled the tilted bool.
    // NOTE: This uses a lambda, but I don't actually understand lambda syntax in C.
    // FIXME: Just set tilted = true and let the RNG unset it when finished with that action?
    // FIXME: Use digitalPinToInterrupt(), ref: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
    attachInterrupt(INT0, [] () {tilted = !tilted;roll_dice(6);}, CHANGE);

    Serial.begin(9600);  // DEBUGGING

}

void loop() {
    redraw_dice();

//  uView.invert(tilted);  // DEBUGGING

    Serial.print("D2 = ");  // DEBUGGING
    Serial.print(digitalRead(2));  // DEBUGGING
    Serial.print("; tilted = ");  // DEBUGGING
    Serial.print(tilted);  // DEBUGGING
    Serial.print("; A0 = ");  // DEBUGGING
    Serial.print(analogRead(A0) / 1024.0);  // DEBUGGING
    Serial.println();  // DEBUGGING
}
