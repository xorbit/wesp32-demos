#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Running installer for required modules on wESP32..."
ampy -p $WESP32PORT run install-modules.py
echo "Done!"
