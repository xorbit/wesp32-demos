#!/bin/sh

curl -d "text=This wESP32 demo works!" -X POST http://wesp32.local/message

sleep 2

curl http://wesp32.local/message?text=Using%20a%20GET%20works%20too%21
