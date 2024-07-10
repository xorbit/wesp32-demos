#!/bin/sh

UID=$(cat uid.txt)
echo "Light level $1 to device with unique ID $UID..."
curl "https://dweet.io/dweet/for/wesp32-light-demo-$UID?light=$1"
echo
echo "Done!"
