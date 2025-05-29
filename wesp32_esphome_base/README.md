# ESPHome base config for the wESP32

A user requested an [ESPHome](https://esphome.io/) example, so that's why this basic config is here. It's a bare
minimum config to get the Ethernet component configured correctly for the wESP32.  It is a config for the wESP32
revision 7 and higher with RTL8201 Ethernet PHY.  The ESPHome documentation also
[documents Ethernet configs](https://esphome.io/components/ethernet.html)
for the wESP32, both for revision 7 and up using the RTL8201 and for older revisions using the LAN8720.

Hopefully this will dispell some misinformation floating around the internet that the switch to the RTL8201 broke
ESPHome compatibility, this was a temporary situation as software support was catching up and for most of its
existence the wESP32 has been fully supported by ESPHome.  All revisions of the wESP32 are fully supported by
ESPHome at this time.

## How to build

After [installing ESPHome](https://esphome.io/guides/installing_esphome.html),
use the following command in this directory to build the firmware:

```sh
esphome compile wesp32_esphome_base.yaml
```

