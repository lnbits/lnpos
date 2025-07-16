#ifndef DEFINES_H
#define DEFINES_H

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
#include <Hash.h>
#include <Bitcoin.h>
#include <Keypad.h>
#include <ArduinoJson.h>

#define VERSION "1.0.0"
#define PARAM_FILE "/elements.json"
#define KEY_FILE "/thekey.txt"
#define USB_POWER 1000 // battery percentage sentinel value to indicate USB power

// variables
extern String inputs;
extern String thePin;
extern String spiffing;
extern String nosats;
extern String cntr;
extern String lnurl;
extern String currency;
extern String key;
extern String preparedURL;
extern String baseURL;
extern String baseURLPoS;
extern String secretPoS;
extern String currencyPoS;
extern String baseURLATM;
extern String baseUrlAtmPage;
extern String secretATM;
extern String currencyATM;
extern String dataIn;
extern String noSats;
extern String qrData;
extern String dataId;
extern String addressNo;
extern String pinToShow;
extern String amountToShow;
extern String key_val;
extern String selection;
extern String offlinePoS;
extern String offlineATM;
extern String masterKey;
extern String lnbitsServer;
extern String invoice;
extern String lnCurrency;
extern String mempool;
extern String securityPin;
extern String fiatDecimalPlaces;
extern String ssid;
extern String password;
extern bool hardcoded;

extern const char menuItems[5][13];
extern const char currencyItems[3][5];
extern char fiatDecimalPlacesOutput[20];
extern int menuItemCheck[5];
extern int menuItemNo;
extern int randomPin;
extern int calNum;
extern int sumFlag;
extern int converted;
extern int sleepTimer;
extern int qrScreenBrightness;
extern long timeOfLastInteraction;
extern bool isSleepEnabled;
extern bool isPretendSleeping;
extern bool onchainCheck;
extern bool lnCheck;
extern bool lnurlCheck;
extern bool unConfirmed;
extern bool selected;
extern bool lnurlCheckPoS;
extern bool lnurlCheckATM;
extern double amountToShowNumber;

enum InvoiceType
{
    LNPOS_TYPE,         // was LNPOS
    OFFLINE_POS_TYPE,   // was offlinePoS
    ONCHAIN_TYPE,       // was ONCHAIN
    OFFLINE_ATM_TYPE,   // was offlineATM
    PORTAL_TYPE         // was PORTAL
};

extern SHA256 h;
extern TFT_eSPI tft;
extern uint16_t qrScreenBgColour;
extern const byte rows;
extern const byte cols;
extern char keys[4][3];
extern byte rowPins[4];
extern byte colPins[3];

// KeyValue struct for key-value parsing
struct KeyValue {
    String key;
    String value;
};

extern Keypad keypad;
extern int checker;
extern char maxdig[20];

#ifndef TFT_BLACK
#define TFT_BLACK   0x0000
#endif
#ifndef TFT_WHITE
#define TFT_WHITE   0xFFFF
#endif
#ifndef TFT_RED
#define TFT_RED     0xF800
#endif
#ifndef TFT_GREEN
#define TFT_GREEN   0x07E0
#endif
#ifndef TFT_ORANGE
#define TFT_ORANGE  0xFD20
#endif
#ifndef TFT_PURPLE
#define TFT_PURPLE  0x780F
#endif

#endif // DEFINES_H