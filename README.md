# ESP8266 Somfy RTS multi-channel remote
This is a [ESPHome](https://esphome.io) project to build a WiFi-enabled [Somfy RTS](https://pushstack.wordpress.com/somfy-rts-protocol/) multi-channel remote for [Home Assistant](https://www.home-assistant.io)

Tailored to turn my [box TOST Corp.](https://www.tostcorp.com/box-somfy-rts) into a fully standalone equipment.

Derived from [Evgeni Golov's ESPHome Somfy RTS config for ESP32](https://www.die-welt.net/2021/06/controlling-somfy-roller-shutters-using-an-esp32-and-esphome/).

## Warning
The ESPHome device will store RTS rolling codes in flash memory. Be carefull when re-flashing the device or you will loose you remote channel associations and waste a channel registration in your covers.

To avoid any problem, you can [Unregister remote channel](#unregister-remote-channel) for all registered covers before any flash modifications.

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

## Somfy RTS configuration
Edit `esphome-somfy-rts.yaml` to adapt:
- emitter pin (line 35, default: D1)
- channel count (line 36, default: 16)

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
- select your WiFi network and provide the credentials

![ESPHome captive portal UI](https://esphome.io/_images/captive_portal-ui.png)

To use [Improv WiFi serial](https://www.improv-wifi.com/serial/) configuration, you can use:
- the [Improv WiFi webapp](https://www.improv-wifi.com/) from a WebSerial supporting browser (Google Chrome, Microsoft Edge and other browsers based on the Blink engine)
- the [Improv WiFi serial client](https://github.com/nicerloop/improv-wifi-serial-client) from the command line

## Connection
The [ESPHome](https://esphome.io) device is automatically exposed to any [Home Assistant](https://www.home-assistant.io) instance using the [native API](https://esphome.io/components/api.html).

Note that the default [native API](https://esphome.io/components/api.html) configuration make the ESP reboot automatically when no client is connected to the API for 15 minutes.

Each channel is materialized as:
- one [cover](https://www.home-assistant.io/integrations/cover/) with assumed state and UP, STOP, DOWN buttons
- one PROG [button](https://www.home-assistant.io/integrations/button/)

These materializations are also available through the [ESPHome web server](https://esphome.io/components/web_server.html) directly from the ESP device (default: http://somfy-rts-XXXXXX.local)

## Register remote channel
To register one channel to control a cover:
1. set the cover half-open, with room for up and down move
2. press PROG for 5 seconds on an already paired cover remote
3. the cover will shortly move up and down to acknowledge switching to PROG mode
4. press PROG on the desired remote or channel button
5. the cover will shortly move up and down to acknowledge registering the new remote

## Unregister remote channel
Replay the same steps as for registering the remote channel. After the second acknowledgement, the remote channel will be unregistered from the cover.
