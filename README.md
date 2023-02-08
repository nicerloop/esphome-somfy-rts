# ESP8266 Somfy RTS multi-channel remote

This is a [ESPHome](https://esphome.io) project to build a WiFi-enabled Somfy RTS multi-channle remote for [Home Assistant](https://www.home-assistant.io)

Tailored to turn my [box TOST Corp.](https://www.tostcorp.com/box-somfy-rts) into a fully standalone equipment.

Derived from [Evgeni Golov's ESPHome Somfy RTS config for ESP32](https://github.com/evgeni/esphome-configs).

## Tech Stack
- [Home Assistant](https://www.home-assistant.io)
- [ESPHome](https://esphome.io)
- [PlatformIO](https://platformio.org)
- [Arduino](https://www.arduino.cc)
- [Somfy Remote Lib](https://github.com/Legion2/Somfy_Remote_Lib)

## Requirements
- [Home Assistant](https://www.home-assistant.io)
- [ESPHome](https://esphome.io)
- [ESP8266](https://www.espressif.com/en/products/modules/esp8266)
- any 433.42 Mhz RF transmitter

## Configuration
Edit `esphome-somfy-rts.yaml` to adapt `new SomfyArduinoRemote(D1, 16)` line 33:
- emitter pin (default: D1)
- channel count (default: 16)

Add or remove covers and prog buttons by copy/pasting/rename to match the declared channel count.

## Installation
```bash
esphome run esphome-somfy-rts.yaml
```
