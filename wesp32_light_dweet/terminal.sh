#!/bin/bash

. ./wesp32-port.sh

echo "Opening a screen session to the wESP32"
echo "To exit, hit Ctrl-A -> \ -> y"
echo "Press any key to continue..."
read -n 1 -s
screen $WESP32PORT 115200
