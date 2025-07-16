
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

#include "settings.h"
#include "config.h"
#include "ui.h"
#include "helpers.h"
#include "screens.h"

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting...");

  // load screen
  tft.init();
  tft.setRotation(1);
  tft.invertDisplay(true);

  // load buttons
  h.begin();
  FlashFS.begin(FORMAT_ON_FAIL);
  SPIFFS.begin(true);
  if (format == true)
  {
    SPIFFS.format();
  }
  logo();
  checkHardcoded();
  readFiles();

  // connect to configured WiFi
  int wifiCounter = 0;
  if (menuItemCheck[0])
  {
    delay(1000);
    processing("Connecting to wifi");
    WiFi.mode(WIFI_STA); // Optional
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED && wifiCounter < 30)
    {
      Serial.print(".");
      delay(100);
      wifiCounter++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      processing("Connected to wifi");
      delay(3000);
    }
    else
    {
      error("Error", "WiFi failed", "");
      delay(3000);
    }
  }

  delay(2000);
}

void loop()
{
  noSats = "0";
  dataIn = "0";
  formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
  amountToShow = fiatDecimalPlacesOutput;
  unConfirmed = true;
  key_val = "";

  // check wifi status
  if (menuItemCheck[0] == 1 && WiFi.status() != WL_CONNECTED)
  {
    menuItemCheck[0] = -1;
  }
  else if (menuItemCheck[0] == -1 && WiFi.status() == WL_CONNECTED)
  {
    menuItemCheck[0] = 1;
  }

  // count menu items
  int menuItemsAmount = 0;

  for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++)
  {
    if (menuItemCheck[i] == 1)
    {
      menuItemsAmount++;
      selection = menuItems[i];
    }
  }

  // select menu item
  while (unConfirmed)
  {
    if (menuItemsAmount == 1)
    {
      usbAccessPoint();
    }
    if (menuItemsAmount > 1)
    {
      menuLoop();
    }

    if (selection == "LNPoS")
    {
      lnMain();
    }
    else if (selection == "OnChain")
    {
      onchainMain();
    }
    else if (selection == "Offline PoS")
    {
      offlinePoSMain();
    }
    else if (selection == "ATM")
    {
      offlineATMMain();
    }
    else if (selection == "Settings")
    {
      usbAccessPoint();
    }
  }
}
