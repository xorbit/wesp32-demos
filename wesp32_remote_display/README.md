# wESP32 Remote Display demo

Since the wESP32 has 13 W of power available when connected to a PoE switch, it can be used to create a remote status display connected by just an Ethernet cable that can be controlled remotely.  Fairly large displays could be used based on available power, this simple piece of demo code just uses an easily available ILI9341 based display.

## Arduino core for ESP32

This demo uses the popular [Arduino environment](https://www.arduino.cc/en/Main/Software) with [Espressif's Arduino core for ESP32](https://github.com/espressif/arduino-esp32).  Follow the instructions on the respective project pages for installation instructions for your platform.

When both Arduino and the ESP32 tools are installed, you should be able to select the wESP32 under "Tools -> Board -> Silicognition wESP32".

## Connections

The ILI9341 display I used is powered by 5 V.  Therefor it is **extremely important to bridge the "5V" solder bridge on the wESP32** to switch the V+ voltage to 5 V instead of the default 12 V.  Different displays have different power requirements, so do what is correct for your display.  Large displays powered from 5 V may require secondary power conversion to be added to produce 5 V while maintaining the full 13 W power output.

This demo requires the following connections between the wESP32 and the ILI9341 display:

| Display pin | wESP32 name | wESP32 pin |
| ----------- | ----------- | ---------- |
| GND         | GND         | 19         |
| VIN         | V+          | 20         |
| CLK         | IO18        | 11         |
| MISO        | IO32        | 7          |
| MOSI        | IO23        | 1          |
| CS          | IO5         | 12         |
| D/C         | IO14        | 14         |
| RST         | IO13        | 13         |

## Generating a different graphic

One of the things this demo does is show a logo on startup.  It is encoded in the `wesp32_logo.h` file.  The steps I used to generate this file from the `wesp32_logo.png` are documented below.  Note that this was done with tools available on a Linux system, you may need to use different tools if these are not available on your platform.

First I used ffmpeg to generate raw binary RGB data in the RGB565 format used by the display:

```bash
ffmpeg -vcodec png -i wesp32_logo.png -vcodec rawvideo -f rawvideo -pix_fmt rgb565 wesp32_logo.rgb
```

Then I converted it from binary to "C" with the `xxd` tool which seems to be part of the `vim` install:

```bash
xxd -i wesp32_logo.rgb > wesp32_logo.h
```

Then I edited the first line of `wesp32_logo.h` and added `const` to it:

```c
const unsigned char wesp32_logo_rgb[] = {
...
```

This prevents the logo from being copied to RAM which would cause a compile error because too much RAM would be needed.

## Software notes

This software demonstrates some extra stuff that isn't strictly necessary for the demo but can come in handy:

* Different ways to define Web server end points
* How to handle "404 Not found"
* MDNS to be able to access `wesp32.local` by name instead of by IP
* Ethernet event handler (which oddly needs to be attached using the WiFi object)

Actually displaying a message on the display is done by sending data in `text` to the `/message` endpoint.  This can be done either by GET or POST.  The `curltest.sh` shows how to do it with [curl](https://curl.haxx.se/).

