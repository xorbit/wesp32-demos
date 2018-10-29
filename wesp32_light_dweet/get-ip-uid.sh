#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Current LAN IP address and unique ID of the wESP32:"
ampy -p $WESP32PORT run get-ip-uid.py

