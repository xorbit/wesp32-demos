# IDF Ethernet iperf

This is not nice clean code.  This is ugly, hacked code that I adapted from the ESP-IDF WiFi iperf example to work with 
Ethernet instead.  It allowed me to test the throughput of the wESP32 Ethernet subsystem and that was all I wanted from it.

**NOTE: This is not needed any more since
[ESP-IDF now contains Ethernet iperf example code](https://github.com/espressif/esp-idf/tree/master/examples/ethernet/iperf).
I'm keeping it in here for now for posterity.**

## How to build and use

Put this directory under `examples/ethernet` in your ESP-IDF installation.  Get everything configured correctly to be able to build IDF examples according to the IDF instructions, then run `make` in this directory to build the code followed by `make flash` to program the wESP32.

I recommend running `screen /dev/ttyUSB0 115200` (adjust the device as needed) to interact with the iperf command line on the wESP32 instead of `make monitor`, which seems to mess with the RTS and/or DTR lines used for auto programming and causes issues.

It is recommended to run the iperf tests in combination with another host connected to Ethernet on the same network so as not to introduce bandwidth limitations caused by the other side.

## Iperf Example

This example implements the protocol used by the common performance measurement tool [iPerf](https://iperf.fr/). 
Performance can be measured between two ESP32s running this example, or between a single ESP32 and a computer running the iPerf tool (recommended).

Demo steps to test station TCP Tx performance: 

1. Run iperf as server on other computer:

   `iperf -s -i 3`

2. Run iperf as client on ESP32:

   `iperf -c 192.168.10.42 -i 3 -t 60`

The console output, which is printed by station TCP RX throughput test, looks like:

>esp32> sta aptest
>
>I (5325) iperf: sta connecting to 'aptest'
>
>esp32> I (6017) event: ip: 192.168.10.248, mask: 255.255.255.0, gw: 192.168.10.1
>
>esp32> iperf -s -i 3 -t 1000
>
>I (14958) iperf: mode=tcp-server sip=192.168.10.248:5001, dip=0.0.0.0:5001, interval=3, time=1000
>
>Interval Bandwidth
>
>esp32> accept: 192.168.10.42,62958
>
>0-   3 sec       8.43 Mbits/sec
>
>3-   6 sec       36.16 Mbits/sec
>
>6-   9 sec       36.22 Mbits/sec
>
>9-  12 sec       36.44 Mbits/sec
>
>12-  15 sec       36.25 Mbits/sec
>
>15-  18 sec       24.36 Mbits/sec
>
>18-  21 sec       27.79 Mbits/sec


