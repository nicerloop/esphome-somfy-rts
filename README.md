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
Edit `esphome-somfy-rts.yaml` to adapt:
- emitter pin (line 31, default: D1)
- channel count (line 32, default: 16)

Add or remove covers and prog buttons by copy/paste/rename to match the declared channel count.

## Network setup
EEPROM use conflicts with dynamic WiFi settings storage. Captive portal and Improv WiFi cannot be used.

Copy secrets.yaml.example to secrets.yaml and edit:
- WiFi SSID (line 1, wifi_ssid)
- WiFi password (line 2, wifi_password)

## Installation
```bash
esphome run esphome-somfy-rts.yaml
```

## Usage
The [ESPHome](https://esphome.io) device is automatically exposed any [Home Assistant](https://www.home-assistant.io) instance using the [native API](https://esphome.io/components/api.html).

Note that the default [native API](https://esphome.io/components/api.html) configuration make the ESP reboot automatically when no client is connected to the API for 15 minutes.

Each channel is materialized as:
- one [cover](https://www.home-assistant.io/integrations/cover/) with assumed state and Up, Stop, Down buttons
- one 'Prog' [button](https://www.home-assistant.io/integrations/button/)

These materializations are also available through the [ESPHome web server](https://esphome.io/components/web_server.html) directly from the ESP device (default: http://somfy-rts-XXXXXX.local where XXXXXX is [the 6 last characters of the WiFi MAC address in hexadecimal](https://esphome.io/components/esphome.html#esphome-mac-suffix))
