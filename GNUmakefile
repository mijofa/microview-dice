# FIXME: Add a tag for MicroVIew and ref that instead
BOARD_TAG = uno

ARDUINO_LIBS = MicroView

# This included makefile is from https://github.com/sudar/Arduino-Makefile
# I've installed it on Debian from the arduino-mk package.
include /usr/share/arduino/Arduino.mk

# I'm normally already working inside screen or tmux so using screen for this is a bit jarring.
# I could use picocom which arduino-mk seems to support already,
# but busybox-microcom is already installed literally everywhere.
monitor:
	busybox microcom -s $(MONITOR_BAUDRATE) $(call get_monitor_port)
