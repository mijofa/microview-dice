#include <Arduino.h>
#include <MicroView.h>

#include <dice-functions.h>

// This is updated by the tilt-switch pin interrupt
volatile bool tilted = false;

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
    attachInterrupt(INT0, [] () {tilted = true;}, CHANGE);

    Serial.begin(9600);  // DEBUGGING
    Serial.println(millis());
}

// Default values for variables used in the loop
unsigned long last_trigger = 0;
int last_loop_pot_pos = 0;
int pot_pos;
int menu_pos;
int roll;

void loop() {
    if (millis()<last_trigger+200){return;}else{last_trigger=millis();}  // Don't loop more than 5/s

    if (tilted) {
        roll = roll_dice(current_dice);
        tilted = false;
        // FIXME: Inverting is bright and harsh on the eyes, find a better way to indicate "busy".
        //        Alternatively, is it possible to invert only the dice? I doubt it.
        uView.invert(true);
    } else {
        uView.invert(false);
    }

    pot_pos = analogRead(A0);
    // With a fine enough soft-pot it might be sitting on just the edge and flapping back and forth,
    // so the extra && parts here are just to avoid flapping in those cases
    if (last_loop_pot_pos != pot_pos - 1 && last_loop_pot_pos != pot_pos && last_loop_pot_pos != pot_pos + 1) {
        last_loop_pot_pos = pot_pos;
        menu_pos = pot_pos / (1024 / num_of_dice);
    }
    if (dice_types[menu_pos] != current_dice) {
        current_dice = dice_types[menu_pos];
        update_dice_face();
    }

    uView.display();

//  uView.invert(tilted);  // DEBUGGING
    Serial.print("Dice = ");
    Serial.print(current_dice);
    Serial.print("; roll = ");  // DEBUGGING
    Serial.print(roll);  // DEBUGGING
    Serial.println();  // DEBUGGING
}
