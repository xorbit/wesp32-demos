#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Uploading main.py..."
ampy -p $WESP32PORT put main.py
echo "Current LAN IP address and unique ID of the wESP32:"
ampy -p $WESP32PORT run get-ip-uid.py
ampy -p $WESP32PORT run get-uid.py > uid.txt
echo "Resetting the wESP32..."
echo "import machine\rmachine.reset()\r" > $WESP32PORT
echo "Done!"
