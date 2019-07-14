microview-dice
==============
The purpose of this project is to make a battery powered dice that you can "roll" (shake a tilt-switch) for a selectable number of sides.
I do not intend to make the number of sides infinite, but rather just to the standard D&D dice of D2, D4, D6, D8, D10, D12, D20, and maybe D100.

I'm using a `Sparkfun Microview <https://www.sparkfun.com/products/12923>`_ as the base controller, mostly because I have about a dozen of these from their original Kickstarter.
I'm using a tilt switch instead of an accelerometer mostly to keep it smaller, but also I figured it will probably use less battery. However I have some concerns about how random the random number generator will be that I suspect using an acellerometer could improve, so that might be a change for V2.
I'm currently planning to just use a coin-cell or 2 for the battery, but I have no idea how well that's going to work at the moment and will figure that after I've sorted out the code. Perhaps take a look at what battery was used for `Microview Wordy <https://hackaday.io/project/3577-wordy>`_ if a coin-cell is not good enough.



FIXME: Mock up a circuit diagram with Fritzing. I know it's tiny & stupid simple, but do it anyway.

FIXME: Don't dump all the code in a single main.ino file. Arduino-mk doesn't like anything else, but it's impossible to keep up with.
