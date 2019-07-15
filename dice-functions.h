// I don't properly understand the difference between .cpp & .h files,
// and I'm pretty sure I'm abusing this .h file and treating it as a mish-mash of both

#ifndef DICE_FUNCTIONS_INCLUDE_GUARD
#define DICE_FUNCTIONS_INCLUDE_GUARD

#include <MicroView.h>

// Screen & font size values
const int screen_margin = 2;
const int screen_width = uView.getLCDWidth() - screen_margin;
const int screen_horizontal_centre = (screen_width + screen_margin) / 2;
const int screen_height = uView.getLCDHeight() - screen_margin;
const int screen_vertical_centre = (screen_height + screen_margin) / 2;
// Technically I want this to be (screen_width - screen_height) / 2,
// but this works just as well due to having already done the halving.
const int horizontal_margin_for_squaring = screen_margin + screen_horizontal_centre - screen_vertical_centre;
// Would like to set these as const as well, but they can't be set until after choosing a font,
// and that's done in a deeper scope where they would then be unreadable.
int digit_width;
int half_digit_height;

// Number of faces each type of die can have
const int dice_types[8] = {2, 4, 6, 8, 10, 12, 20, 100};
const int num_of_dice = 8;  // C doesn't support any useful way of counting how many items are in an array
int current_dice = 20;  // FIXME: Default to where the soft-pot is set

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
    // NOTE: I'm calculating the size based only on height because it's easier than using the horizontal_margin_for_squaring variable
    uView.rect(screen_horizontal_centre - screen_vertical_centre, screen_vertical_centre - screen_vertical_centre,
               screen_height, screen_height);
}

void draw_digit(int digit) {
    uView.setFontType(2); // set font type 2 (7segment)
    // FIXME: Can I find/make a more D&D/fantasy style font instead?
    digit_width = uView.getFontWidth();
    half_digit_height = uView.getFontHeight() / 2;

    // Figure out how wide the text is, so we can offset from the centre of the display before printing
    if (digit >= 100) {  // 3 digits
        uView.setCursor(screen_horizontal_centre - (digit_width * 1.5), screen_vertical_centre - half_digit_height);
    } else if (digit >= 10) {  // 2 digits
        uView.setCursor(screen_horizontal_centre - digit_width, screen_vertical_centre - half_digit_height);
    } else {  // 1 digit
        uView.setCursor(screen_horizontal_centre - (digit_width / 2), screen_vertical_centre - half_digit_height);
    }
    // Print the current roll result.
    uView.print(digit);
}

// FIXME: This is called directly from the ISR, it's not marked volatile enough for that
int roll_dice(int faces) {
    // FIXME: Should I update the random seed?
    // NOTE: random(min, max) generates random number between min and max-1.
    //       Why max is -1 and min is not +1 I have no fucking idea, but that is super confusing so I'm just gonna offset that here.
    int roll_result = random(1, faces+1);
    draw_digit(roll_result);
    return roll_result;
}

void update_dice_face() {
    uView.clear(PAGE);
    if (current_dice == 4) {
        draw_face_d4();
    } else if (current_dice == 6) {
        draw_face_d6();
    }
    draw_digit(current_dice);
}

#endif
