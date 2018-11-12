# wESP32 Bluetooth Low Energy beacon scanner

This demo shows a useful ability of the wESP32: to be able to use wired and wireless communication at the same time.  It scans for Bluetooth Low Energy advertisement packets, and send the data to a web server over Ethernet.  It works with the wESP32 out-of-the-box and requires no extra hardware.

## Arduino core for ESP32

This demo uses the popular [Arduino environment](https://www.arduino.cc/en/Main/Software) with [Espressif's Arduino core for ESP32](https://github.com/espressif/arduino-esp32).  Follow the instructions on the respective project pages for installation instructions for your platform.

When both Arduino and the ESP32 tools are installed, you should be able to select the wESP32 under "Tools -> Board -> Silicognition wESP32".

## Customizing the partitions

One difficulty with this demo is that the ESP32 BLE handling code is huge, and the resulting binary is too large to fit in the default flash partition configuration.  So you will have to customize this to be able to compile and load the project into the wESP32.  This can be done as follows.  First locate the `boards.txt` configuration file in the ESP32 Arduino package under `<Arduino project directory>/hardware/espressif/esp32`.  Open it, and locate and change the following configuration keys with these new values:

```
wesp32.upload.maximum_size=1966080
...
wesp32.build.partitions=min_spiffs
```

This changes to a different flash partition layout that reduces the amount of space for the SPI flash file system and allocates more space for the code, enough to fit the demo.

## Simple test web server

To make testing this demo easier, a simple [Flask](http://flask.pocoo.org/) based web server is included that you can run on your development machine.  It requires [Python 3](https://www.python.org/) and Flask to be installed.  To install Python 3 on a Ubuntu or Debian based machine run:

```sh
sudo apt install python3
```

To install Flask for Python 3, run:

```sh
sudo pip3 install flask
```

After this, you can use the provided script to run the test web server:

```sh
./run_server.sh
```

## Software notes

To run the Arduino code and successfully send the scanned BLE advertisement data to the web server, you need to customize the `POST_URL` define to the IP address or URL of your web server.  By default the BLE scan time is 10 seconds but this can be customized with the `SCAN_TIME` define.

The first thing the code does is create a callback handler that prints BLE advertisement information as it comes in.  This is not strictly necessary for the application, but nice to have to see what's going on.

Then in the `setup` function, Ethernet is started and the BLE scanner is initialized.  The `loop` function starts by calling the function that scans for BLE advertisements.  When this function is done, a [JSON](https://www.json.org/) formatted string is contructed that contains all the information to be sent to the server.  After this, an HTTP client object is used to send the data to the web server using POST.

The simple test web server that is included does nothing more than interpret the incoming data as JSON and print it to the terminal.  A real application web server would of course do more with this data.
