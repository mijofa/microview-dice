// I don't properly understand the difference between .cpp & .h files,
// and I'm pretty sure I'm abusing this .h file and treating it as a mish-mash of both

#ifndef DICE_FUNCTIONS_INCLUDE_GUARD
#define DICE_FUNCTIONS_INCLUDE_GUARD

#include <MicroView.h>

// Screen & font size values
const uint8_t screen_margin = 2;
const uint8_t screen_width = uView.getLCDWidth() - screen_margin;
const uint8_t screen_horizontal_centre = (screen_width + screen_margin) / 2;
const uint8_t screen_height = uView.getLCDHeight() - screen_margin;
const uint8_t screen_vertical_centre = (screen_height + screen_margin) / 2;
// Technically I want this to be (screen_width - screen_height) / 2,
// but this works just as well due to having already done the halving.
const uint8_t horizontal_margin_for_squaring = screen_margin + screen_horizontal_centre - screen_vertical_centre;
// Would like to set these as const as well, but they can't be set until after choosing a font,
// and that's done in a deeper scope where they would then be unreadable.
uint8_t digit_width;
uint8_t half_digit_height;


void draw_face_d4() {
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            horizontal_margin_for_squaring, screen_height - screen_margin);  // left
    uView.line(
            horizontal_margin_for_squaring, screen_height - screen_margin,  // left
            screen_width - horizontal_margin_for_squaring, screen_height - screen_margin);  // right
    uView.line(
            screen_width - horizontal_margin_for_squaring, screen_height - screen_margin,  // right
            screen_horizontal_centre, screen_margin);  // top
}

void draw_face_d6() {
    // NOTE: I'm calculating the size based only on height because I want a square not a rectangle
//    uView.rectFill(screen_horizontal_centre - (screen_height / 2),
//                   screen_vertical_centre - (screen_height / 2),
//                   screen_height,
//                   screen_height,
//                   BLACK,
//                   NORM);
    uView.rect(screen_horizontal_centre - (screen_height / 2),
               screen_vertical_centre - (screen_height / 2),
               screen_height,
               screen_height);
}

// FIXME: This is called directly from the ISR, it's not marked volatile enough for that
int roll_dice(int faces) {
    // FIXME: Should I update the random seed?
    // NOTE: random(min, max) generates random number between min and max-1.
    //       Why max is -1 and min is not +1 I have no fucking idea, but that is super confusing so I'm just gonna offset that here.
    int roll_result = random(1, faces+1);

    uView.setFontType(2); // set font type 2 (7segment)
    // FIXME: Can I find/make a more D&D/fantasy style font instead?
    digit_width = uView.getFontWidth();
    half_digit_height = uView.getFontHeight() / 2;

    // Figure out how wide the text is, so we can offset from the centre of the display before printing
    if (roll_result >= 100) {  // 3 digits
        uView.setCursor(screen_horizontal_centre - (digit_width * 1.5), screen_vertical_centre - half_digit_height);
    } else if (roll_result >= 10) {  // 2 digits
        uView.setCursor(screen_horizontal_centre - digit_width, screen_vertical_centre - half_digit_height);
    } else {  // 1 digit
        uView.setCursor(screen_horizontal_centre - (digit_width / 2), screen_vertical_centre - half_digit_height);
    }
    // Print the current roll result.
    uView.print(roll_result);

    return roll_result;
}

#endif
