# FIXME: Add a tag for MicroVIew and ref that instead
BOARD_TAG = uno

ARDUINO_LIBS = MicroView

# Also include the current directory in the includes.
# I'm a little surprised arduino-mk doesn't already do this or similar,
# but all I was able to find was replacing the USER_LIB_DIR,
# which I don't want to do because that would then stop letting me include Arduino libs
CPPFLAGS = -I$(dir $(realpath $(firstword $(MAKEFILE_LIST))))

# This included makefile is from https://github.com/sudar/Arduino-Makefile
# I've installed it on Debian from the arduino-mk package.
include /usr/share/arduino/Arduino.mk

# I'm normally already working inside screen or tmux so using screen for this is a bit jarring.
# I could use picocom which arduino-mk seems to support already,
# but busybox-microcom is already installed literally everywhere.
monitor:
	busybox microcom -s $(MONITOR_BAUDRATE) $(call get_monitor_port)
