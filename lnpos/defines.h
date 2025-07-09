#ifndef DEFINES_H
#define DEFINES_H

#include "settings.h" // Include user settings
#include "config.h"
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <math.h>
fs::SPIFFSFS &FlashFS = SPIFFS;
#define FORMAT_ON_FAIL true
#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdio.h>
#include "qrcoded.h"
#include <WiFiClientSecure.h>
#include "mbedtls/aes.h"
#include "mbedtls/md5.h"

// ArduinoJson, Keypad and uBitcoin should be installed using the Arduino Library Manager.
// The latest versions should work, verified with ArduinoJson 7.2.1, Keypad 3.1.1 and uBitcoin 0.2.0
#include <Hash.h>
#include <Bitcoin.h>
#include <Keypad.h>
#include <ArduinoJson.h>

#define VERSION "1.0.0"
#define PARAM_FILE "/elements.json"
#define KEY_FILE "/thekey.txt"
#define USB_POWER 1000 // battery percentage sentinel value to indicate USB power

// variables
String inputs;
String thePin;
String spiffing;
String nosats;
String cntr = "0";
String lnurl;
String currency;
String key;
String preparedURL;
String baseURL;
String baseURLPoS;
String secretPoS;
String currencyPoS;
String baseURLATM;
String baseUrlAtmPage;
String secretATM;
String currencyATM;
String dataIn = "0";
String noSats = "0";
String qrData;
String dataId;
String addressNo;
String pinToShow;
String amountToShow = "0";
String key_val;
String selection;

const char menuItems[5][13] = {"LNPoS", "Offline PoS", "OnChain", "ATM", "Settings"};
const char currencyItems[3][5] = {"sat", "USD", "EUR"};
char fiatDecimalPlacesOutput[20];
int menuItemCheck[5] = {0, 0, 0, 0, 1};
int menuItemNo = 0;
int randomPin;
int calNum = 1;
int sumFlag = 0;
int converted = 0;
int sleepTimer = 30;          // Time in seconds before the device goes to sleep
int qrScreenBrightness = 180; // 0 = min, 255 = max
long timeOfLastInteraction = millis();
bool isSleepEnabled = true;
bool isPretendSleeping = false;
bool onchainCheck = false;
bool lnCheck = false;
bool lnurlCheck = false;
bool unConfirmed = true;
bool selected = false;
bool lnurlCheckPoS = false;
bool lnurlCheckATM = false;
double amountToShowNumber;
enum InvoiceType
{
    LNPOS,
    offlinePoS,
    ONCHAIN,
    offlineATM,
    PORTAL
};

SHA256 h;
TFT_eSPI tft = TFT_eSPI();

uint16_t qrScreenBgColour = tft.color565(qrScreenBrightness, qrScreenBrightness, qrScreenBrightness);

const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[rows] = {21, 27, 26, 22}; // connect to the row pinouts of the keypad
byte colPins[cols] = {33, 32, 25};     // connect to the column pinouts of the keypad

struct KeyValue
{
    String key;
    String value;
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int checker = 0;
char maxdig[20];

#endif