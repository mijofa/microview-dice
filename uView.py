"""
Doing a full round trip of writing, compiling, & uploading every time I wanted to try some shape drawing was getting annoying.
So here's some Python that should work pretty much the same as the uView drawing functions.

It doesn't implement all of the drawing functions, specifically neglects font & character functions.
Also doesn't implement colour or mode selecting. (yet?)
"""
import turtle


MICROVIEW_WIDTH = 64
MICROVIEW_HEIGHT = 48
_size_multiplier = 10


def _translate_coords(x, y):
    """
    uView's 0,0 = top-left corner
    Turtle's 0,0 = centre screen
    Turtle's positive Y = Up
    uView's Positive Y = Down
    Turtle's Positive X = Left
    uView's Positive x = Right

    And because it's near impossible to see things properly on a real screen
    uView's pixel size = 1
    Turtle's pixel size = 10
    """
    x -= MICROVIEW_WIDTH / 2
    y -= MICROVIEW_HEIGHT / 2
    x *= 10
    y *= -10

    return x, y


def setup():
    # So Turtle's pixels are tiny, I can't figure out how to just zoom in or lower the resolution,
    # so let's just multiply it all by 10
    turtle.setup(MICROVIEW_WIDTH * _size_multiplier, MICROVIEW_HEIGHT * _size_multiplier)
    ## FIXME: Increase the speed of the turtle?
    ##        Microview is not slow, but Turtle is slow to help make sense of what's going on, and that's a good reason.
    # turtle.speed(100)
    clear()


def display():
    pass


def clear():
    turtle.clearscreen()
    turtle.pensize(_size_multiplier)
    turtle.bgcolor('black')
    turtle.pencolor('blue')
    turtle.fillcolor('blue')
    turtle.penup()


def setCursor(x, y):
    turtle.penup()
    turtle.goto(*_translate_coords(x, y))


def getLCDWidth():
    return MICROVIEW_WIDTH


def getLCDHeight():
    return MICROVIEW_HEIGHT


# Font
# Pixels
def pixel(x, y, color=None, mode=None):
    if color or mode:
        raise NotImplementedError()

    turtle.penup()
    turtle.goto(*_translate_coords(x, y))
    turtle.pendown()


# Lines
def line(x0, y0, x1, y1, color=None, mode=None):
    if color or mode:
        raise NotImplementedError()

    turtle.pendown()
    turtle.goto(*_translate_coords(x0, y0))
    turtle.goto(*_translate_coords(x1, y1))
    turtle.penup()


def lineH(x, y, width, color=None, mode=None):
    line(x, y, x + width, y, color, mode)


def lineV(x, y, height, color=None, mode=None):
    line(x, y, x, y + height, color, mode)


# Rectangles
def rect(x, y, width, height, color=None, mode=None):
    if color or mode:
        raise NotImplementedError()

    turtle.goto(*_translate_coords(x, y))
    turtle.pendown()
    turtle.goto(*_translate_coords(x + width, y))
    turtle.goto(*_translate_coords(x + width, y + height))
    turtle.goto(*_translate_coords(x, y + height))
    turtle.goto(*_translate_coords(x, y))
    turtle.penup()


def rectFill(*args):
    turtle.begin_fill()
    rect(*args)
    turtle.end_fill()


# Circles
def circle(x, y, radius, color=None, mode=None):
    """
    uView draws a circle centered on x,y
    Turtle draws a circle centred on x+radius,y

    Both are horribly confusing, but maybe that's just my paradigms
    """
    if color or mode:
        raise NotImplementedError()

    setCursor(x - radius, y)
    turtle.setheading(-90)
    turtle.pendown()
    turtle.circle(radius * _size_multiplier)
    turtle.penup()


def circleFill(*args):
    turtle.begin_fill()
    circle(*args)
    turtle.end_fill()

# MISC
