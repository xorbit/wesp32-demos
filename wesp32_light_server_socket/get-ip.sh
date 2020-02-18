#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Current wESP32 LAN IP address:"
ampy -p $WESP32PORT run get-ip.py
echo "Resetting the wESP32..."
echo "import machine\rmachine.reset()\r" > $WESP32PORT

