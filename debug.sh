arduino-cli compile \
    --build-property "build.partitions=min_spiffs" \
    --build-property "upload.maximum_size=1966080" \
    --library ./libraries/TFT_eSPI \
    --library ./libraries/QRCode \
    --build-path build --fqbn esp32:esp32:ttgo-lora32 lnpos && \
arduino-cli upload --input-dir build --fqbn esp32:esp32:ttgo-lora32 -p /dev/ttyACM0 && \
arduino-cli monitor -p /dev/ttyACM0 -c baudrate=115200
