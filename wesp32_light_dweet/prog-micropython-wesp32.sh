#!/bin/sh

. ./wesp32-port.sh

if [ ! $1 ]
  then
  echo "Please provide the MicroPython binary you want to program as a parameter!"
  exit 1
fi

echo "Erasing wESP32 flash..."
esptool.py --port $WESP32PORT --baud 115200 erase_flash
echo "Programming MicroPython..."
esptool.py --port $WESP32PORT --baud 921600 write_flash 0x1000 $1
echo "Waiting for MicroPython to boot..."
sleep 5
echo "Configuring boot.py for automatic network connection..."
ampy -p $WESP32PORT put boot.py
echo "Resetting to load boot.py and connect to the network..."
echo "import machine\rmachine.reset()\r" > $WESP32PORT
echo "Done!"
