#!/bin/sh

. ./wesp32-port.sh

# Send Ctrl-C
printf "\3\r" > $WESP32PORT

echo "Resetting the wESP32..."
echo "import machine\rmachine.reset()\r" > $WESP32PORT
echo "Done!"
