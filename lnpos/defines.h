#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <Keypad.h>
#include <Hash.h>
#include <Bitcoin.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <mbedtls/aes.h>
#include <mbedtls/md5.h>
#include <SPI.h>
#include "qrcoded.h"

// All global variables, constants, enums, and struct definitions

#define VERSION "1.0.0"
#define PARAM_FILE "/elements.json"
#define KEY_FILE "/thekey.txt"
#define USB_POWER 1000

extern bool format;
extern bool hardcoded;
extern String lnurlPoS, lnurlATM, masterKey, lnbitsServer, invoice, lncurrency, lnurlATMMS, lnurlATMPin, decimalplaces, ssid, password;
extern String inputs, thePin, spiffing, nosats, cntr, lnurl, currency, key, preparedURL, baseURL, baseURLPoS, secretPoS, currencyPoS, baseURLATM, baseUrlAtmPage, secretATM, currencyATM, dataIn, noSats, qrData, dataId, addressNo, pinToShow, amountToShow, key_val, selection;
extern const char menuItems[5][13];
extern const char currencyItems[3][5];
extern char decimalplacesOutput[20];
extern int menuItemCheck[5], menuItemNo, randomPin, calNum, sumFlag, converted, sleepTimer, qrScreenBrightness;
extern long timeOfLastInteraction;
extern bool isSleepEnabled, isPretendSleeping, onchainCheck, lnCheck, lnurlCheck, unConfirmed, selected, lnurlCheckPoS, lnurlCheckATM;
extern double amountToShowNumber;
extern SHA256 h;
extern TFT_eSPI tft;
extern uint16_t qrScreenBgColour;
extern const byte rows, cols;
extern char keys[4][3];
extern byte rowPins[4], colPins[3];
extern Keypad keypad;
extern int checker;
extern char maxdig[20];

// Enums

enum InvoiceType { LNPOS, LNURLPOS, ONCHAIN, LNURLATM, PORTAL };

// Structs
struct KeyValue { String key; String value; };

// Add externs for all main logic functions
extern void lnMain();
extern void onchainMain();
extern void lnurlPoSMain();
extern void lnurlATMMain();
extern void serialLaunch();
extern bool isInteger(const char *str);

extern fs::SPIFFSFS &FlashFS;
extern const bool FORMAT_ON_FAIL;
