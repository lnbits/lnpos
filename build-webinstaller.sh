#!/bin/sh
PROJECT_NAME=lnpos
REPO=https://github.com/lnbits/lnpos/releases/download
INSTALLER_PATH=./hardware-installer/public/firmware

git clone https://github.com/lnbits/hardware-installer

cp INSTALLER.md ./hardware-installer/public/INSTALLER.md
cp versions.json ./hardware-installer/src/versions.json
cp installer/config.js ./hardware-installer/src/config.js

sed -i "s/%title%/$PROJECT_NAME/g" ./hardware-installer/index.html

mkdir -p $INSTALLER_PATH
for device in $(jq -r '.devices[]' ./hardware-installer/src/versions.json); do
    for version in $(jq -r '.versions[]' ./hardware-installer/src/versions.json); do
        mkdir -p $INSTALLER_PATH/$device/$version
        wget $REPO/$version/$PROJECT_NAME.ino.bin
        wget $REPO/$version/$PROJECT_NAME.ino.partitions.bin
        wget $REPO/$version/$PROJECT_NAME.ino.bootloader.bin
        mv $PROJECT_NAME* $INSTALLER_PATH/$device/$version
    done
done
