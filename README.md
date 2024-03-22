<img src="images/tdisplay.png?raw=true">

<h1>
Sally, the DIY Point of Sale device
</h1>

## Easy setup using browser https://lnpos.lnbits.com/

LNPoS includes:

- LNPoS for online LN payments (former <a href="https://github.com/arcbtc/LNPoS">separate project</a>)
- LNURLPoS for offline LN payments (former <a href="https://github.com/arcbtc/LNURLPoS">separate project</a>)
- OnChain for onchain payments)
- LNURLATM for making offline LN withdraw links
  <br></br>

  What you need:
- The Lilygo <a href="https://www.aliexpress.com/item/33048962331.html">TTGO/Tdisplay</a> 
- A <a href="https://www.aliexpress.com/item/32993999306.html">membrane keybad</a> or the <a href="https://www.aliexpress.com/item/1005003589706292.html">Lilygo Keyboard</a> they made specifically for Sally!
=> or get yourself the full <a href="https://www.aliexpress.com/item/1005003589706292.html">LNPoS Kit</a>, dimensions are 40x52x11mm 
- Any 3.7V lithium iron flat battery with 1.25mm JST with < 1000 mAh to fit inside the standard LNPoS Kit

Original <a href="https://twitter.com/arcbtc/status/1484942260013838336">demo</a>

Video tutorial <a href="https://twitter.com/arcbtc/status/1585605023337168896">https://twitter.com/arcbtc/status/1585605023337168896</a>

Got questions? Join us <a href="https://t.me/makerbits">MakerBits</a>

<h2>
Configuring
</h2>

> Go to `settings` to lanch USB config mode or wifi access point
> If using access opint the device SSID default password is "ToTheMoon1"

### LNPoS – Online Lightning Network payments

Invoices are generated and checked from LNbits install.

#### Setting up

## Easy setup using browser https://lnpos.lnbits.com/

## WiFi Access Point config

Launch portal and enter an LNbits endpoint (ie.legend.lnbits.com), invoice key, and a fiat currency to make things easier for product pricing.

<img src="images/lnpos.png?raw=true" width="75%">

### LNURLPoS – Offline Lightning Network payments

A random pin generated on the device is encrypted and passed trough the payee. When the payment has been made the unencrypted pin is sent as a receipt. This function makes use of LNURL-pay protocol. Uses LNbits LNURLDevice extension.

#### Setting up

Launch portal and enter the string from the LNURLDevice extension on LNbits

<img src="images/lnurlpos.png?raw=true" width="75%">

### OnChain – Generate fresh addresses using an xPub

Use an xPub to generate a fresh address for every payment. Useful for large purchases. Includes a mempool.space QR so the payment can be verified.

#### Setting up

Launch portal and enter BIP39 xPub and a pin, to make the function secure.

<img src="images/onchain.png?raw=true" width="75%">

### LNURLATM – Meatbag ATM, give refunds, accept cash for sats.

Create withdraw/faucet links. Uses more-or-les. This function makes use of LNURL-withdraw protocol. Uses LNbits LNURLDevice extension.

#### Setting up

Launch portal and enter the string from the LNURLDevice extension on LNbits

<img src="images/lnurlatm.png?raw=true" width="75%">

<br></br>

Software installation:
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


