#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Uploading main.py..."
ampy -p $WESP32PORT put main.py
echo "Creating directory /static..."
ampy -p $WESP32PORT mkdir static > /dev/null
echo "Uploading index.html..."
ampy -p $WESP32PORT put static/index.html static/index.html
echo "Uploading wesp32-logo.png..."
ampy -p $WESP32PORT put static/wesp32-logo.png static/wesp32-logo.png
echo "Current LAN IP address of the wESP32:"
ampy -p $WESP32PORT run get-ip.py
echo "Resetting the wESP32..."
echo "import machine\rmachine.reset()\r" > $WESP32PORT
echo "Done!"
