#!/bin/sh
command -v arduino-cli >/dev/null 2>&1 || { echo >&2 "arduino-cli not found. Aborting."; exit 1; }
arduino-cli config --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json init
arduino-cli core update-index
arduino-cli core install esp32:esp32
arduino-cli upgrade
arduino-cli lib install WebSockets ArduinoJson AutoConnect Base64 Keypad PageBuilder uBitcoin
arduino-cli compile \
    --library ./libraries/TFT_eSPI \
    --library ./libraries/QRCode \
    --build-path build --fqbn esp32:esp32:ttgo-lora32 lnpos
