# ESP8266 Somfy RTS multi-channel remote

This is a [ESPHome](https://esphome.io) project to build a WiFi-enabled [Somfy RTS](https://pushstack.wordpress.com/somfy-rts-protocol/) multi-channel remote for [Home Assistant](https://www.home-assistant.io)

Tailored to turn my [box TOST Corp.](https://www.tostcorp.com/box-somfy-rts) into a fully standalone equipment.

Derived from [Evgeni Golov's ESPHome Somfy RTS config for ESP32](https://www.die-welt.net/2021/06/controlling-somfy-roller-shutters-using-an-esp32-and-esphome/).

## Software
- [Home Assistant](https://www.home-assistant.io)
- [ESPHome](https://esphome.io)
    - [PlatformIO](https://platformio.org)
    - [Arduino](https://www.arduino.cc)
- [Somfy Remote Lib](https://github.com/Legion2/Somfy_Remote_Lib)

## Hardware
- [ESP8266](https://www.espressif.com/en/products/modules/esp8266)
- [FS1000A RF 433 transmitter](https://github.com/Nickduino/Pi-Somfy#2-hardware)
    - 433.42 Mhz crystal (Somfy RTS specific frequency)
    - (optional) 433MHz helical antenna

## Configuration
Edit `esphome-somfy-rts.yaml` to adapt `new SomfyArduinoRemote(D1, 16)` line 33:
- emitter pin (default: D1)
- channel count (default: 16)

Add or remove covers and prog buttons by copy/paste/rename to match the declared channel count.

## Installation
```bash
esphome run esphome-somfy-rts.yaml
```
## Network setup

The ESPHome device is setup with either:
- a [captive WiFi access point](https://esphome.io/components/captive_portal.html) from which to configure the WiFi client
- an [Improv WiFi](https://www.improv-wifi.com) serial server to configure the WiFi client from the USB/serial interface

To use the [ESPHome captive WiFi access point](https://esphome.io/components/captive_portal.html):
- connect to the open WiFi network named `somfy-rts-XXXXXX` (where XXXXXX is [the 6 last characters of the WiFi MAC address in hexadecimal](https://esphome.io/components/esphome.html#esphome-mac-suffix))
- the web interface should open automatically; if that does not work, you can navigate to http://192.168.4.1/ manually in your browser
- select your WiFi network and procide the credentials

![ESPHome captive portal UI](https://esphome.io/_images/captive_portal-ui.png)

To use [Improv WiFi serial](https://www.improv-wifi.com/serial/) configuration, you can use:
- the [Improv WiFi webapp](https://www.improv-wifi.com/) from a WebSerial supporting browser (Google Chrome, Microsoft Edge and other browsers based on the Blink engine)
- the [Improv WiFi serial client](https://github.com/nicerloop/improv-wifi-serial-client) from the command line

## Usage

The [ESPHome](https://esphome.io) device is automatically exposed any [Home Assistant](https://www.home-assistant.io) instance using the [native API](https://esphome.io/components/api.html).

Note that the default [native API](https://esphome.io/components/api.html) configuration make the ESP reboot automatically when no client connected to the API for 15 minutes.

Each channel is materialized as:
- one [cover](https://www.home-assistant.io/integrations/cover/) with assumed state and Up, Stop, Down buttons
- one 'Prog' [button](https://www.home-assistant.io/integrations/button/)

These materializations are also available through the [ESPHome web server](https://esphome.io/components/web_server.html) directly from the ESP device (default: http://somfy-rts-XXXXXX.local)
