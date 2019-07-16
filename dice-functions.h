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

// Number of faces each type of die can have
const int dice_types[8] = {2, 4, 6, 8, 10, 12, 20, 100};
const int num_of_dice = 8;  // C doesn't support any useful way of counting how many items are in an array
int current_dice;  // FIXME: Default to where the soft-pot is set

void draw_face_d2() {
    uView.circle(
            screen_horizontal_centre, screen_vertical_centre,
            screen_vertical_centre - screen_margin);
}

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

void draw_face_d8() {
    // FIXME: 3Dify this one
    int twothird_height = ((screen_height - (screen_margin * 2)) / 3) * 2;
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            horizontal_margin_for_squaring, twothird_height);  // left
    // The digit doesn't fit in the top section, so just don't split the sections
//    uView.line(
//            horizontal_margin_for_squaring, twothird_height,  // left
//            screen_width - horizontal_margin_for_squaring, twothird_height);  // right
    uView.line(
            screen_width - horizontal_margin_for_squaring, twothird_height,  // right
            screen_horizontal_centre, screen_margin);  // top
    uView.line(
            horizontal_margin_for_squaring, twothird_height,  // left
            screen_horizontal_centre, screen_height - screen_margin);  // bottom
    uView.line(
            screen_horizontal_centre, screen_height - screen_margin,  // bottom
            screen_width - horizontal_margin_for_squaring, twothird_height);  // right
}

void draw_face_d10() {
    // FIXME: Maybe add some of the lower background faces
    int fivesixth_height = ((screen_height - (screen_margin * 2)) / 6) * 5;
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            horizontal_margin_for_squaring, fivesixth_height);  // left
    // The digit doesn't fit in the top section, so just don't split the sections
//    uView.line(
//            horizontal_margin_for_squaring, fivesixth_height,  // left
//            screen_width - horizontal_margin_for_squaring, fivesixth_height);  // right
    uView.line(
            screen_width - horizontal_margin_for_squaring, fivesixth_height,  // right
            screen_horizontal_centre, screen_margin);  // top
    uView.line(
            horizontal_margin_for_squaring, fivesixth_height,  // left
            screen_horizontal_centre, screen_height - screen_margin);  // bottom
    uView.line(
            screen_horizontal_centre, screen_height - screen_margin,  // bottom
            screen_width - horizontal_margin_for_squaring, fivesixth_height);  // right

    // Background faces
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            horizontal_margin_for_squaring, screen_vertical_centre);  // left
    uView.line(
            screen_width - horizontal_margin_for_squaring, screen_vertical_centre,  // right
            screen_horizontal_centre, screen_margin);  // top

    uView.line(
            horizontal_margin_for_squaring, screen_vertical_centre,  // left
            horizontal_margin_for_squaring, fivesixth_height);  // foreground left
    uView.line(
            screen_width - horizontal_margin_for_squaring, fivesixth_height,  // foreground right
            screen_width - horizontal_margin_for_squaring, screen_vertical_centre);  // right
}

void draw_face_d12() {
    int twofifth_height = ((screen_height - (screen_margin * 2)) / 5) * 2;
    int quart_width = ((screen_horizontal_centre - horizontal_margin_for_squaring) / 2);
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            horizontal_margin_for_squaring, twofifth_height + screen_margin);  // left
    uView.line(
            screen_horizontal_centre, screen_margin,  // top
            screen_width - horizontal_margin_for_squaring, twofifth_height + screen_margin);  // right

    uView.line(
            horizontal_margin_for_squaring, twofifth_height + screen_margin,  // left
            horizontal_margin_for_squaring + quart_width, screen_height - screen_margin);  // bottom-left
    uView.line(
            screen_width - horizontal_margin_for_squaring, twofifth_height + screen_margin,  // right
            screen_horizontal_centre + quart_width, screen_height - screen_margin);  // bottom-right
    uView.line(
            horizontal_margin_for_squaring + quart_width, screen_height - screen_margin,  // bottom-left
            screen_horizontal_centre + quart_width, screen_height - screen_margin);  // bottom-right
}

void draw_digit(int digit) {
    uView.setFontType(2);  // set font type 2 (7segment)
    // FIXME: Can I find/make a more D&D/fantasy style font instead?
    int digit_width = uView.getFontWidth();
    int digit_height = uView.getFontHeight();
    int half_digit_height = digit_height / 2;

    int hundreds_left_origin = screen_horizontal_centre - (digit_width * 1.5);
    int tens_left_origin = screen_horizontal_centre - digit_width;
    int ones_left_origin = screen_horizontal_centre - (digit_width / 2);
    int top_origin = screen_vertical_centre - half_digit_height;

    // NOTE: 10 & 100 get printed as 0 & 00 respectively, to match how the text usually printed on D10s & D100s 

    // Clear the previous digit.
    // Normally uView.print will clear the pixels underneath the new characters,
    // but when shifting from 2 digits to 1 digit it gets a bit messy.
    if (current_dice > 100) {
        uView.rectFill(
                hundreds_left_origin, top_origin,
                // FIXME: The .1 here is effectively a magic number only used to account for the spacing between digits,
                //        it might not be correct, but it looks alright so far.
                digit_width * 3.1, digit_height,
                BLACK, NORM);
    } else if (current_dice > 10) {
        uView.rectFill(
                tens_left_origin, top_origin,
                // FIXME: The .1 here is effectively a magic number only used to account for the spacing between digits,
                //        it might not be correct, but it looks alright so far.
                digit_width * 2.1, digit_height,
                BLACK, NORM);
    }

    // Figure out how wide the text is, so we can offset from the centre of the display before printing
    if (digit > 100) {  // 3 digits
        uView.setCursor(hundreds_left_origin, top_origin);
    } else if (digit > 10) {  // 2 digits
        uView.setCursor(tens_left_origin, top_origin);
    } else {  // 1 digit
        uView.setCursor(ones_left_origin, top_origin);
    }
    // Print the current roll result.
    if (current_dice == 10 && digit == 10) {
        uView.print('0');
    } else if (current_dice == 100 && digit == 100) {
        uView.print("00");
    } else {
        uView.print(digit);
    }
}

// FIXME: This is called directly from the ISR, it's not marked volatile enough for that
int roll_dice(int faces) {
    // FIXME: Should I update the random seed?
    // NOTE: random(min, max) generates random number between min and max-1.
    //       Why max is -1 and min is not +1 I have no fucking idea, but that is super confusing so I'm just gonna offset that here.
    int roll_result = random(1, faces + 1);
    draw_digit(roll_result);
    uView.display();
    return roll_result;
}

void update_dice_face() {
    uView.clear(PAGE);
    if (current_dice == 2) {
        draw_face_d2();
    } else if (current_dice == 4) {
        draw_face_d4();
    } else if (current_dice == 6) {
        draw_face_d6();
    } else if (current_dice == 8) {
        draw_face_d8();
    } else if (current_dice == 10) {
        draw_face_d10();
    } else if (current_dice == 12) {
        draw_face_d12();
    }
    draw_digit(current_dice);
    uView.display();
}

#endif
