# wESP32 Mongoose OS demo: LED light controlled from mDash mobile app

This demo provides example code for using [Mongoose OS](https://mongoose-os.com) on the wESP32.  With very little code, it can turn an LED connected to IO23 on and off from [Mongoose OS's powerful mDash dashboard](https://dash.mongoose-os.com) or a mobile web app generated from the dashboard.  The dashboard provides powerful functions such as remote configuration of the device and even allows you to edit the application code remotely!  Plus OTA firware update functionality is built-in for paying customers.

## Hardware

In the minimum hardware configuration, you can just connect a low power LED with current limiting resistor between IO23 and GND.  For practical light output, you can connect IO23 to the control input of a high power LED driver that powers the LED from V+ (12 V).

## Mongoose OS

Mongoose OS describes itself as "An ideal framework for IoT product pilot".  As many recent news stories have highlighted, it is _hard_ to do IoT right.  If you have an idea for an IoT application but you are not an expert when it comes to the backend infrastructure needed to create reliable, secure and updateable IoT devices that won't be hacked instantly, you are best off leaving these concerns to experts.

Mongoose OS is an excellent place to start.  It provides a solid full-featured base that takes care of all these infrastructure concerns for you so you can focus on your application.  And with the ability to develop your application in C or Javascript, built-in backends for several popular cloud services, tons of libraries, excellent documentation and easy-to-use management tools, you can get to a professional, deployable product very quickly.

## Quick start guide

The easiest way to get started with this example is to follow the excellent [Mongoose OS quick start guide](https://mongoose-os.com/docs/quickstart/setup.md).  It first guides you through installing the `mos` tool on your development system.  Once installed, this tool provides a convenient local interface to the wESP32 over the USB serial port where you can execute commands and observe console output from the board.  Follow the directions in the quick start guide on how to install USB-to-serial drivers if your platform needs them.  The wESP32-Prog submodule requires the SiLabs drivers, if you use another programmer you need to install whatever driver you need for it.  On Linux, most likely things will work without installing anything.

Once you are connected to the wESP32, follow the directions but don't create a new app (skip step 4).  Instead, clone this repo to your development machine instead and either start the `mos` tool while in this example's directory or `cd` to it instead.  After that you can continue following the guide to build the app and flash it to the wESP32.

Since the wESP32 is connected via Ethernet, you can skip the "Configure WiFi" step (step 7) as well.  Pick back up at step 8 where you'll create a new device in the mDash and folow the directions to get the token and configure your board to use it.  This example doesn't export the "uptime" value, only the "on" value, so when you create the mobile app you may need to remove that from the config.

## Ethernet configuration

Mongoose OS has a library to support Ethernet on the ESP32, which works fine on the wESP32.  Mongoose OS also has a very flexible configuration system that allows you to set the Ethernet configuration correctly for the wESP32 in your project.  These are the relevant parts of the `mos.yml` file to make Ethernet work:

```yaml
config_schema:
  - ["eth.clk_mode", 0]
  - ["eth.mdc_gpio", 16]
  - ["eth.mdio_gpio", 17]
  - ["eth.phy_addr", 0]
  - ["eth.enable", true]

libs:
  - origin: https://github.com/mongoose-os-libs/ethernet
```

## The code

The main program code of the demo lives in the `fs/init.js` file.  It uses Mongoose OS'es convenient [shadow API](https://mongoose-os.com/docs/mdash/shadow.md).  This defines a structure that is maintained in the cloud and is supposed to reflect the current state of the device.  There is a `desired` state and a `reported` state, and the system automatically tries to make them consistent.

The code first gets the pin definition for the LED and configures it as an output.  After that is a handler that deals with updating the LED and updating the cloud backend with the current state.  There is no main loop as the whole thing is properly event driven.

## WARNING

During my testing with Mongoose OS, I blew up the Ethernet phy on my wESP32.  I haven't tracked down the exact library that caused this issue, but it happened when I included the `js-demo-bundle` library in my `mos.yml` file.  Therefor, **DO NOT USE THIS LIBRARY WITH THE WESP32** and **DO NOT TRY ANY OF THE DEMOS THAT LOAD THIS LIBRARY WITH THE WESP32**.  The demo bundle library pulls in a whole slew of hardware driver libraries and my theory is that one of them reconfigures some pins of the ESP32 in a way that is damaging to the phy.

I hope to have a more fine-grained idea of what the root cause of this is in the future, but for now, this is all I know about the issue.
