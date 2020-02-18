# wESP32 MicroPython demo: LED light on/off with built-in web UI

This demo provides example code for using [MicroPython](https://micropython.org/) on the wESP32.  It implements a very simple web server that can be accessed to turn the IO23 output on and off.

This code was adapted (read: stolen) from this [excellent post at Random Nerd Tutorials](https://randomnerdtutorials.com/esp32-esp8266-micropython-web-server/). :)

Various scripts are provided to simplify some of the complexities that come with using a powerful firmware like MicroPython.  Note that error handling in these scripts is minimal, this is done to keep them clear so they can be used to learn how each operation works.  And because I'm lazy. :)

## Hardware

In the minimum hardware configuration, you can just connect a low power LED with current limiting resistor between IO23 and GND.  For practical light output, you can connect IO23 to the control input of a high power LED driver that powers the LED from V+ (12 V).

## Dependencies

All my development is done on [Ubuntu Linux](https://www.ubuntu.com/).  If you use a different platform you may need to adjust some of these installation steps and some of the scripts to your platform.

All the helper scripts get their serial port setting from the `wesp32-port.sh` script, this makes it easy to adjust the serial port setting to your system by just editing this single file.

Binaries for MicroPython can be found on the [MicroPython download page](http://micropython.org/download).  Download a binary for the ESP32 platform with Ethernet support to use on the wESP32.

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

## Software installation

The installation of this demo software to your wESP32 can be done in two steps that are simplified by helper scripts.

### Installing MicroPython

A helper script is provided to install a MicroPython image to the wESP32.  Just run the following command in the project directory:

```sh
./prog-micropython-wesp32.sh <esp32-xxxxxxxx.bin>
```

Replace the placeholder with the name of your downloaded MicroPython image.

The script not only flashes MicroPython to the wESP32 but it also updates the `boot.py` file to automatically configure the ESP32 networking subsystem correctly for the wESP32.  This makes it so your own scripts don't even need to bother to do anything to get the LAN connected!  You can just access the `lan` variable to interact with the `LAN` object and get useful information such as the IP address.

Note that MicroPython comes pre-installed on the wESP32 from the factory, so on a new unit you can skip this step!

### Uploading the application files

Last we need to upload the files for the application to the wESP32.  This is automated with another script, so you can just run:

```sh
./upload-app.sh
```

This will send the application files to the correct places, reset and print the wESP32's IP address.  The application's `main.py` will automatically start to run after boot when the network is connected.  You can now point your browser to the wESP32's IP address and it will serve a simple web interface that allows you to control the on/off status of IO23 pin and the connected LED.

## Other helper scripts

Some other helper scripts are provided that can make your life easier:

- `get-ip.sh` prints the IP address of the connected wESP32.  This only works when MicroPython and the `boot.py` script have been installed.
- `reset.sh` can be used to reset the connected wESP32.  This only works when MicroPython has been installed.
- `terminal.sh` uses `screen` to open an interactive terminal session to the wESP32.  Very handy to access the MicroPython REPL command line to interactively get yourself familiar with the environment.  If the application is running, you need to hit Ctrl-C to get the command line.

## Information on the Python code

The first thing the code does after loading modules is test whether `boot.py` already set up the Ethernet system.  If not, it takes care of doing that.  Then the IO23 pin is configured as GPIO output.  This is followed by a function that generates the web page HTML.

Next there is the main loop that accepts connections and searches for `/?led=off` and `/?led=on` query strings in the request to control the LED accordingly, then sends the HTML to the client and closes the connection, after which it goes back to waiting for a new connection.

