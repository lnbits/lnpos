<img src="images/tdisplay.png?raw=true">

# LNPoS, the DIY Point of Sale device

Check out this [video tutorial](https://twitter.com/arcbtc/status/1585605023337168896) on the Makerbits channel or the
<a href="https://twitter.com/arcbtc/status/1484942260013838336">original demo</a>.

<img src="https://github.com/arbadacarbaYK/lnpos/assets/63317640/6c73f327-6806-4f81-a9e1-651526a8d427" alt="switch_front" width="200">
<img src="https://github.com/arbadacarbaYK/lnpos/assets/63317640/4a7fd0bd-34ed-45f7-853e-f69aeb24bd6b" alt="switch_front" width="200">

# Flash and configure via webinstaller https://lnpos.lnbits.com/

### LNPoS includes

- Online LN payments (former <a href="https://github.com/arcbtc/LNPoS">LNPoS project</a>)
- Offline LN payments (former <a href="https://github.com/arcbtc/LNURLPoS">LNURLPoS project</a>)
- Offline LN withdraw links (LNURLATM)
- Optional onchain payments

### What you need:
- Lilygo <a href="https://www.aliexpress.com/item/33048962331.html">TTGO/Tdisplay</a> 
- The <a href="https://www.aliexpress.com/item/1005003589706292.html">Lilygo Keyboard</a> specifically made for Sally 
=> or get yourself the <a href="https://www.aliexpress.com/item/1005003589706292.html">LNPoS Kit</a>, dimensions 40x52x11mm e.g. in the [LNbits shop](https://shop.lnbits.com/product-category/hardware/point-of-sale-devices)
- 3.7V lithium iron flat battery with 1.25mm JST with < 1000 mAh to fit inside the standard LNPoS Kit
- LNbits LNURLdevice extension

Got questions ? Join us <a href="https://t.me/lnbits">t.me/lnbits</a>, <a href="https://t.me/makerbits">t.me/makerbits</a>


# Configuring the wifi

Launch portal and enter an LNbits endpoint (ie.legend.lnbits.com), invoice key, and a fiat currency to make things easier for product pricing.

Go to `settings` to lanch USB config mode or wifi access point
If using access point the device SSID default password is "ToTheMoon1"

<img src="images/lnpos.png?raw=true" width="75%">


### LNURLPoS – Offline Lightning Network payments

A random pin generated on the device is encrypted and passed trough the payee. When the payment has been made the unencrypted pin is sent as a receipt. This function makes use of LNURL-pay protocol. Uses LNbits LNURLDevice extension.

#### Setting up LNURLPoS

Launch portal and enter the string from the LNURLDevice extension on LNbits

<img src="images/lnurlpos.png?raw=true" width="75%">

### Onchain – Generate fresh addresses using an xPub

Use an xPub to generate a fresh address for every payment. Useful for large purchases. Includes a mempool.space QR so the payment can be verified.

#### Setting up onchain connection

Launch portal and enter BIP39 xPub and a pin, to make the function secure.

<img src="images/onchain.png?raw=true" width="75%">

### LNURLATM – Meatbag ATM, give refunds, accept cash for sats.

Create withdraw/faucet links. Uses more-or-less. This function makes use of LNURL-withdraw protocol. Uses LNbits LNURLDevice extension.

#### Setting up LNURLATM

Launch portal and enter the string from the LNURLDevice extension on LNbits

<img src="images/lnurlatm.png?raw=true" width="75%">



# Alternative installation (without webinstaller)

- Install <a href="https://www.arduino.cc/en/software">Arduino IDE 1.8.19</a>
- Install ESP32 boards, using <a href="https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html#installing-using-boards-manager">boards manager</a>
- Role back ESP32 boards to 2.0.1 in boards manager
![image](https://user-images.githubusercontent.com/33088785/161862832-1269a12e-16ce-427c-9a92-df3ee573a1fb.png)

- Download this repo
- Copy these [libraries](lnPoSTdisplay/libraries) into your Arduino install "libraries" folder
- Open this [LNPoSTdisplay.ino](lnPoSTdisplay/lnPoSTdisplay.ino) file in the Arduino IDE
- Select "TTGO-LoRa32-OLED-V1" from tools>board
- Upload to device

> Press/hold any button on the keypad during startup to launch portal.
> Default password is "ToTheMoon1" (without the quotes)


> _Note: If using MacOS, you will need the CP210x USB to UART Bridge VCP Drivers available here https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers_
> If you are using **MacOS Big Sur or an Mac with M1 chip**, you might encounter the issue `A fatal error occurred: Failed to write to target RAM (result was 0107)`, this is related to the chipsest used by TTGO, you can find the correct driver and more info in this <a href="https://github.com/Xinyuan-LilyGO/LilyGo-T-Call-SIM800/issues/139#issuecomment-904390716">GitHub issue</a>

If you want to build something new from the parts, check out the [LNbits wiki](https://github.com/lnbits/lnbits/wiki/Tooling-&-Building-with-LNbits) to see some examples.
