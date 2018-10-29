# wESP32 MicroPython demo: dimmable LED light controlled from dweet.io

This demo provides example code for using [MicroPython](https://micropython.org/) on the wESP32.  It implements a web client to monitor an endpoint on [dweet.io](https://dweet.io/) that can be accessed to set the PWM level of the IO23 output.

Various scripts are provided to simplify some of the complexities that come with using a powerful firmware like MicroPython.  Note that error handling in these scripts is minimal, this is done to keep them clear so they can be used to learn how each operation works.  And because I'm lazy. :)

## Hardware

In the minimum hardware configuration, you can just connect a low power LED with current limiting resistor between IO23 and GND.  For practical light output, you can connect IO23 to the PWM control input of a high power LED driver that powers the LED from V+ (12 V).

## Dependencies

All my development is done on [Ubuntu Linux](https://www.ubuntu.com/).  If you use a different platform you may need to adjust some of these installation steps and some of the scripts to your platform.

All the helper scripts get their serial port setting from the `wesp32-port.sh` script, this makes it easy to adjust the serial port setting to your system by just editing this single file.

Binaries for MicroPython can be found on the [MicroPython download page](http://micropython.org/download).  Download a binary for the ESP32 platform to use on the wESP32.

To be able to flash the MicroPython image to the wESP32, you need to have [Espressif's esptool.py](https://github.com/espressif/esptool) installed.  This can be done by running:

```sh
sudo pip3 install esptool
```

This depends on [Python 3](https://www.python.org/), which likely is present on most Linux systems.  If you use Python 2 just use `pip` instead of `pip3`.  If you need to install Python 3, you can do so by running:

```sh
sudo apt install python3
```

A variety of tools exist to manage transfer of files from your host to the wESP32 running MicroPython.  The one I used in the scripts for this example is [Adafruit's ampy](https://github.com/adafruit/ampy).  This can be installed by running:

```sh
sudo pip3 install adafruit-ampy
```

The `terminal.sh` script to get an interactive terminal to the wESP32 uses the `screen` command, you can install it by running:

```sh
sudo apt install screen
```

The `set-light-level.sh` script uses `curl` to send a value to dweet.io, you can install it by running:

```sh
sudo apt install curl
```

## Software installation

The installation of this demo software to your wESP32 can be done in two steps that are simplified by helper scripts.

### Installing MicroPython

A helper script is provided to install a MicroPython image to the wESP32.  Just run the following command in the project directory:

```sh
./prog-micropython-wesp32.sh <esp32-xxxxxxxx.bin>
```

Replace the placeholder with the name of your downloaded MicroPython image.

The script not only flashes MicroPython to the wESP32 but it also updates the `boot.py` file to automatically configure the ESP32 networking subsystem correctly for the wESP32.  This makes it so your own scripts don't even need to bother to do anything to get the LAN connected!  You can just access the `lan` variable to interact with the `LAN` object and get useful information such as the IP address.

### Uploading the application files

Since there are no extra modules this project depends on, we can now upload the files for the application to the wESP32.  This is automated with another script, so you can just run:

```sh
./upload-app.sh
```

This will send the application file to the wESP32, reset and print the wESP32's IP address, the board's unique ID and a dweet.io URL that can be used to control the light level.  It will also write the unique ID to a file called `uid.txt` that is used by the `set-light-level.sh` script to access the correct URL that will be monitored by the board.  The application will automatically start to run after boot.

## Other helper scripts

Some other helper scripts are provided that can make your life easier:

- `get-ip-uid.sh` prints the IP address, unique ID and dweet.io URL of the connected wESP32.  This only works when MicroPython and the `boot.py` script have been installed.
- `reset.sh` can be used to reset the connected wESP32.  This only works when MicroPython has been installed.
- `terminal.sh` uses `screen` to open an interactive terminal session to the wESP32.  Very handy to access the MicroPython REPL command line to interactively get yourself familiar with the environment.  If the application is running, you need to hit Ctrl-C to get the command line.
- `set-light-level.sh` can be used to send the provided light level parameter to the dweet.io endpoint monitored by the board.  It uses the `uid.txt` file saved by `upload-app.sh` to construct the correct URL.

## Information on the Python code

The Python application code for this demo is quite simple.  It depends on the `boot.py` code to initialize the Ethernet peripheral correctly for the wESP32.  Then it initializes the PWM output that will drive the LED.  The ESP32's unique ID is read out and turned into a string.

After this the main loop starts.  It constructs the correct URL, then uses the built-in `urequests` module to get the data from dweet.io, parses it as [JSON](https://www.json.org/) and extracts the `light` value we're interested in.  Then this value is used to set the PWM output.

If anything goes wrong in this sequence, the exception handler makes sure the PWM output doesn't get changed.  Whether everything works or not, the loop sleeps for 2 seconds and then does the same thing again.

