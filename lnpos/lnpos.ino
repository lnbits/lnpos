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

// ArduinoJson, Keypad and uBitcoin should be installed using the Arduino Library manager:
#include <Hash.h>
#include <Bitcoin.h>
#include <Keypad.h>
#include <ArduinoJson.h>

#define PARAM_FILE "/elements.json"
#define KEY_FILE "/thekey.txt"
#define USB_POWER 1000 // battery percentage sentinel value to indicate USB power

//////////SET TO TRUE TO WIPE MEMORY//////////////

bool format = false;

////////////////////////////////////////////////////////
////////////LNPOS WILL LOOK FOR DETAILS SET/////////////
////////OVER THE WEBINSTALLER CONFIG, HOWEVER///////////
///////////OPTIONALLY SET HARDCODED DETAILS/////////////
////////////////////////////////////////////////////////

bool hardcoded = false; /// Set to true to hardcode

String lnurlPoS = "https://legend.lnbits.com/lnurldevice/api/v1/lnurl/WTmei,BzzoY5wbgpym3eMdb9ueXr,USD";
String lnurlATM = "https://legend.lnbits.com/lnurldevice/api/v1/lnurl/W5xu4,XGg4BJ3xCh36JdMKm2kgDw,USD";
String masterKey = "xpub6CJFgwcim8tPBJo2A6dS13kZxqbgtWKD3LKj1tyurWADbXbPyWo11exyotTSUY3cvhQy5Mfj8FSURgpXhc4L2UvQyaTMC36S49JnNJMmcWU";
String lnbitsServer = "https://legend.lnbits.com";
String invoice = "37d45d3e1f0d4572a905bad544588d7d";
String lncurrency = "GBP";
String lnurlATMMS = "https://mempool.space";
String lnurlATMPin = "878787";
String decimalplaces = "2";
String ssid = "AlansBits";
String password = "ithurtswhenip";

//////////////////////////////////////////////////

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
char decimalplacesOutput[20];
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
  LNURLPOS,
  ONCHAIN,
  LNURLATM,
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

bool isInteger(const char *str)
{
  if (*str == '-' || *str == '+')
  {
    str++;
  }
  while (*str)
  {
    if (!isdigit(*str))
    {
      return false;
    }
    str++;
  }
  return true;
}

void formatNumber(float number, int decimalplaces, char *output)
{
  // Create a format string based on the decimalplaces
  char formatString[10];
  sprintf(formatString, "%%.%df", decimalplaces);

  // Use the format string to write the number to the output buffer
  sprintf(output, formatString, number);
}

void setup()
{
  Serial.begin(115200);

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
  formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
  amountToShow = decimalplacesOutput;
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
      accessPoint();
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
      lnurlPoSMain();
    }
    else if (selection == "ATM")
    {
      lnurlATMMain();
    }
    else if (selection == "Settings")
    {
      accessPoint();
    }
  }
}

String getJsonValue(JsonDocument &doc, const char *name)
{
  for (JsonObject elem : doc.as<JsonArray>())
  {
    if (strcmp(elem["name"], name) == 0)
    {
      String value = elem["value"].as<String>();
      return value;
    }
  }
  return "";
}

void checkHardcoded()
{
  if (!hardcoded)
  {
    lnurlPoS = "";
    lnurlATM = "";
    masterKey = "";
    lnbitsServer = "";
    invoice = "";
    lncurrency = "";
    lnurlATMMS = "";
    lnurlATMPin = "";
    decimalplaces = "";
    ssid = "";
    password = "";
  }
}

void accessPoint()
{
  readFiles();
  pinToShow = "";
  dataIn = "";
  isATMMoneyPin(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(true, false, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      isATMMoneyPin(true);
    }

    if (pinToShow.length() == lnurlATMPin.length() && pinToShow != lnurlATMPin)
    {
      error("  WRONG PIN");
      delay(1500);

      pinToShow = "";
      dataIn = "";
      isATMMoneyPin(true);
    }
    else if (pinToShow == lnurlATMPin)
    {
      error("   SETTINGS", "HOLD 1 FOR USB", "");
      // start portal (any key pressed on startup)
      int count = 0;
      while (count < 10)
      {
        key_val = "";
        delay(200);
        count++;
        const char key = keypad.getKey();
        if (key == '1')
        {
          configOverSerialPort();
          key_val = "";
          getKeypad(false, true, false, false);
          if (key_val == "*")
          {
            return;
          }
        }
      }
    }
    else
    {
      delay(100);
    }
  }
}

// on-chain payment method
void onchainMain()
{
  File file = SPIFFS.open(KEY_FILE);
  if (file)
  {
    addressNo = file.readString();
    addressNo = String(addressNo.toInt() + 1);
    file.close();
    file = SPIFFS.open(KEY_FILE, FILE_WRITE);
    file.print(addressNo);
    file.close();
  }
  else
  {
    file.close();
    file = SPIFFS.open(KEY_FILE, FILE_WRITE);
    addressNo = "1";
    file.print(addressNo);
    file.close();
  }

  Serial.println(addressNo);
  inputScreenOnChain();

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, true, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      HDPublicKey hd(masterKey);
      qrData = hd.derive(String("m/0/") + addressNo).address();
      qrShowCodeOnchain(true, " *MENU #CHECK");

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, true, false, false);

        if (key_val == "*")
        {
          unConfirmed = false;
        }
        else if (key_val == "#")
        {
          while (unConfirmed)
          {
            qrData = "https://" + lnurlATMMS + "/address/" + qrData;
            qrShowCodeOnchain(false, " *MENU");

            while (unConfirmed)
            {
              key_val = "";
              getKeypad(false, true, false, false);

              if (key_val == "*")
              {
                unConfirmed = false;
              }
            }
          }
        }
        handleBrightnessAdjust(key_val, ONCHAIN);
      }
    }
  }
}

void lnMain()
{
  readFiles();

  if (!checkOnlineParams())
  {
    return;
  }

  if (lncurrency == "" || lncurrency == "default")
  {
    currencyLoop();
  }

  processing("FETCHING FIAT RATE");
  if (!getSats())
  {
    error("FETCHING FIAT RATE FAILED");
    delay(3000);
    return;
  }

  isLNMoneyNumber(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, false, true, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      if (noSats.toInt() == 0)
      {
        error("ZERO SATS");
        delay(3000);
        isLNMoneyNumber(true);
        continue;
      }

      // request invoice
      processing("FETCHING INVOICE");
      if (!getInvoice())
      {
        unConfirmed = false;
        error("ERROR FETCHING INVOICE");
        delay(3000);
        break;
      }

      // show QR
      qrShowCodeln();

      // check invoice
      bool isFirstRun = true;
      while (unConfirmed)
      {
        int timer = 0;

        if (!isFirstRun)
        {
          unConfirmed = checkInvoice();
          if (!unConfirmed)
          {
            paymentSuccess();
            timer = 5000;

            while (key_val != "*")
            {
              key_val = "";
              getKeypad(false, true, false, false);

              if (key_val != "*")
              {
                delay(100);
              }
            }
          }
        }

        // abort on * press
        while (timer < (isFirstRun ? 6000 : 2000))
        {
          getKeypad(false, true, false, false);

          if (key_val == "*")
          {
            noSats = "0";
            dataIn = "0";
            formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
            amountToShow = decimalplacesOutput;
            unConfirmed = false;
            timer = 5000;
            break;
          }
          else
          {
            delay(100);
            handleBrightnessAdjust(key_val, LNPOS);
            key_val = "";
          }
          timer = timer + 100;
        }

        isFirstRun = false;
      }

      noSats = "0";
      dataIn = "0";
      formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
      amountToShow = decimalplacesOutput;
    }
    else
    {
      delay(100);
    }
  }
}

void lnurlPoSMain()
{
  inputs = "";
  pinToShow = "";
  dataIn = "";

  if (!checkOfflineParams())
  {
    return;
  }

  isLNURLMoneyNumber(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, false, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      if (!makeLNURL())
      {
        isLNURLMoneyNumber(true);
        continue;
      }
      qrShowCodeLNURL(" *MENU #SHOW PIN");

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, true, false, false);

        if (key_val == "#")
        {
          showPin();

          while (unConfirmed)
          {
            key_val = "";
            getKeypad(false, true, false, false);

            if (key_val == "*")
            {
              unConfirmed = false;
            }
          }
        }
        else if (key_val == "*")
        {
          unConfirmed = false;
        }
        handleBrightnessAdjust(key_val, LNURLPOS);
      }
    }
    else
    {
      delay(100);
    }
  }
}

void lnurlATMMain()
{
  pinToShow = "";
  dataIn = "";
  isATMMoneyPin(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(true, false, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      isATMMoneyPin(true);
    }

    if (pinToShow.length() == lnurlATMPin.length() && pinToShow != lnurlATMPin)
    {
      error("  WRONG PIN");
      delay(1500);

      pinToShow = "";
      dataIn = "";
      isATMMoneyPin(true);
    }
    else if (pinToShow == lnurlATMPin)
    {
      isATMMoneyNumber(true);
      inputs = "";
      dataIn = "";

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, false, false, true);

        if (key_val == "*")
        {
          unConfirmed = false;
        }
        else if (key_val == "#")
        {
          if (!makeLNURL())
          {
            isATMMoneyNumber(true);
            continue;
          }
          qrShowCodeLNURL(" *MENU");

          while (unConfirmed)
          {
            key_val = "";
            getKeypad(false, true, false, false);
            handleBrightnessAdjust(key_val, LNURLATM);

            if (key_val == "*")
            {
              unConfirmed = false;
            }
          }
        }
      }
    }
    else
    {
      delay(100);
    }
  }
}

void getKeypad(bool isATMPin, bool justKey, bool isLN, bool isATMNum)
{
  const char key = keypad.getKey();

  if (key == NO_KEY)
  {
    return;
  }

  isPretendSleeping = false;

  key_val = String(key);

  if (key_val != "")
  {
    timeOfLastInteraction = millis();
  }

  if (dataIn.length() < 9)
  {
    dataIn += key_val;
  }

  if (isLN)
  {
    isLNMoneyNumber(false);
  }
  else if (isATMPin)
  {
    isATMMoneyPin(false);
  }
  else if (justKey)
  {
  }
  else if (isATMNum)
  {
    isATMMoneyNumber(false);
  }
  else
  {
    isLNURLMoneyNumber(false);
  }
}

///////////DISPLAY///////////////
void serialLaunch()
{
  configLaunch("USB Config");
}

void configLaunch(String title)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(20, 30);
  tft.println(title);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 65);
  tft.setTextSize(2);
  tft.println(" WHEN FINISHED *");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(30, 83);
  tft.setTextSize(2);
  // tft.println(config.apid);
}

void isLNMoneyNumber(bool cleared)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 20);
  tft.print("  - ENTER AMOUNT -");
  tft.setTextSize(3);
  tft.setCursor(0, 50);
  tft.println(String(lncurrency) + ": ");
  tft.println("SAT: ");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.println(" *MENU #INVOICE");

  if (!cleared)
  {
    amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
    formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = String(decimalplacesOutput);
    noSats = String(converted * amountToShowNumber);
  }
  else
  {
    noSats = "0";
    dataIn = "0";
    formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = decimalplacesOutput;
  }

  tft.setTextSize(3);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setCursor(75, 50);
  tft.println(amountToShow);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(75, 75);
  tft.println(noSats.toInt());
}

void isLNURLMoneyNumber(bool cleared)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 20);
  tft.print("  - ENTER AMOUNT -");
  tft.setTextSize(3);
  tft.setCursor(0, 50);
  tft.println(String(currencyPoS) + ": ");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.println(" *MENU #INVOICE");
  tft.setTextSize(3);

  if (!cleared)
  {
    amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
    formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = String(decimalplacesOutput);
  }
  else
  {
    dataIn = "0";
    formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = decimalplacesOutput;
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(75, 50);
  tft.println(amountToShow);
}

void isATMMoneyNumber(bool cleared)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 20);
  tft.print("  - ENTER AMOUNT -");
  tft.setTextSize(3);
  tft.setCursor(0, 50);
  tft.println(String(currencyATM) + ": ");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.println(" *MENU #WITHDRAW");
  tft.setTextSize(3);

  if (!cleared)
  {
    amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
    formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = String(decimalplacesOutput);
  }
  else
  {
    dataIn = "0";
    formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
    amountToShow = decimalplacesOutput;
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(75, 50);
  tft.println(amountToShow);
}

void isATMMoneyPin(bool cleared)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 20);
  tft.print(" ENTER SECRET PIN");
  tft.setTextSize(3);
  tft.setCursor(0, 50);
  tft.println("PIN:");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.println(" *MENU #CLEAR");

  pinToShow = dataIn;
  String obscuredPinToShow = "";

  int pinLength = dataIn.length();
  for (size_t i = 0; i < pinLength; i++)
  {
    obscuredPinToShow += "*";
  }

  tft.setTextSize(3);
  if (cleared)
  {
    pinToShow = "";
    dataIn = "";
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(75, 50);
  tft.println(obscuredPinToShow);
}

void inputScreenOnChain()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(0, 40);
  tft.println("XPUB ENDING " + masterKey.substring(masterKey.length() - 5));
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 120);
  tft.println(" *MENU #ADDRESS");
}

void qrShowCodeln()
{
  tft.fillScreen(qrScreenBgColour);

  qrData.toUpperCase();
  const char *qrDataChar = qrData.c_str();
  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];

  qrcode_initText(&qrcoded, qrcodeData, 11, 0, qrDataChar);

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, TFT_BLACK);
      }
      else
      {
        tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, qrScreenBgColour);
      }
    }
  }

  tft.setCursor(0, 220);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.print(" *MENU");
}

void qrShowCodeOnchain(bool anAddress, String message)
{
  tft.fillScreen(qrScreenBgColour);

  if (anAddress)
  {
    qrData.toUpperCase();
  }

  const char *qrDataChar = qrData.c_str();
  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];
  int pixSize = 0;

  tft.setCursor(0, 100);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, qrScreenBgColour);

  if (anAddress)
  {
    qrcode_initText(&qrcoded, qrcodeData, 2, 0, qrDataChar);
    pixSize = 4;
  }
  else
  {
    qrcode_initText(&qrcoded, qrcodeData, 4, 0, qrDataChar);
    pixSize = 3;
  }

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, TFT_BLACK);
      }
      else
      {
        tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, qrScreenBgColour);
      }
    }
  }

  tft.setCursor(0, 120);
  tft.println(message);
}

void qrShowCodeLNURL(String message)
{
  tft.fillScreen(qrScreenBgColour);

  qrData.toUpperCase();
  const char *qrDataChar = qrData.c_str();
  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];
  qrcode_initText(&qrcoded, qrcodeData, 6, 0, qrDataChar);

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(65 + 3 * x, 5 + 3 * y, 3, 3, TFT_BLACK);
      }
      else
      {
        tft.fillRect(65 + 3 * x, 5 + 3 * y, 3, 3, qrScreenBgColour);
      }
    }
  }

  tft.setCursor(0, 220);
  tft.setTextSize(2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.println(message);
}

void error(String message)
{
  error(message, "", "");
}

void error(String message, String additional, String additional2)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 30);
  tft.println(message);
  if (additional != "")
  {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 100);
    tft.setTextSize(2);
    tft.println(additional);
  }
  if (additional2 != "")
  {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 120);
    tft.setTextSize(2);
    tft.println(additional2);
  }
}

void processing(String message)
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 60);
  tft.println(message);
}

void paymentSuccess()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(70, 50);
  tft.println("PAID");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.println("  PRESS * FOR MENU");
}

void showPin()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(40, 5);
  tft.println("PROOF PIN");
  tft.setCursor(70, 60);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(4);
  tft.println(randomPin);
}

void lnurlInputScreen()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
  tft.setCursor(0, 0);
  tft.println("AMOUNT THEN #");
  tft.setCursor(50, 110);
  tft.setTextSize(2);
  tft.println("TO RESET PRESS *");
  tft.setTextSize(3);
  tft.setCursor(0, 30);
  tft.print(String(currency) + ":");
}

void logo()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 30);
  tft.print("LN");
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.print("PoS");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 80);
  tft.print("Powered by LNbits");
}

long int lastBatteryCheck = 0;
void updateBatteryStatus(bool force = false)
{
  // throttle
  if (!force && lastBatteryCheck != 0 && millis() - lastBatteryCheck < 5000)
  {
    return;
  }

  lastBatteryCheck = millis();

  // update
  const int batteryPercentage = getBatteryPercentage();

  String batteryPercentageText = "";
  if (batteryPercentage == USB_POWER)
  {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    batteryPercentageText = " USB";
  }
  else
  {
    if (batteryPercentage >= 60)
    {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    else if (batteryPercentage >= 20)
    {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    }
    else
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
    }

    if (batteryPercentage != 100)
    {
      batteryPercentageText += " ";

      if (batteryPercentage < 10)
      {
        batteryPercentageText += " ";
      }
    }

    batteryPercentageText += String(batteryPercentage) + "%";
  }

  tft.setCursor(190, 120);
  tft.print(batteryPercentageText);
}

void currencyLoop()
{
  // footer/header
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.print("    - CURRENCY -");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(" *NEXT #SELECT");

  updateBatteryStatus(true);

  bool currencySelected = true;
  int currencyItemNo = 0;

  while (currencySelected)
  {
    maybeSleepDevice();
    tft.setCursor(0, 40);
    tft.setTextSize(2);

    int menuItemCount = 0;

    for (int i = 0; i < sizeof(currencyItems) / sizeof(currencyItems[0]); i++)
    {
      if (currencyItems[i] == currencyItems[currencyItemNo])
      {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        lncurrency = currencyItems[i];
      }
      else
      {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }

      tft.print("  ");
      tft.println(currencyItems[i]);
      menuItemCount++;
    }

    bool btnloop = true;
    while (btnloop)
    {
      maybeSleepDevice();
      key_val = "";
      getKeypad(false, true, false, false);

      if (key_val == "*")
      {
        currencyItemNo++;
        currencyItemNo %= sizeof(currencyItems) / sizeof(currencyItems[0]);

        btnloop = false;
      }
      else if (key_val == "#")
      {
        currencySelected = false;
        btnloop = false;
      }
      else
      {
        updateBatteryStatus();
        delay(100);
      }
    }
  }
}

void menuLoop()
{
  // footer/header
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 10);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.print("      - MENU -");
  tft.setCursor(0, 120);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(" *NEXT #SELECT");

  updateBatteryStatus(true);

  // menu items
  selection = "";
  selected = true;

  while (selected)
  {
    maybeSleepDevice();
    if (menuItemCheck[0] <= 0 && menuItemNo == 0)
    {
      menuItemNo++;
    }

    tft.setCursor(0, 40);
    tft.setTextSize(2);

    int current = 0;
    int menuItemCount = 0;

    for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++)
    {
      if (menuItemCheck[i] == 1)
      {
        if (menuItems[i] == menuItems[menuItemNo])
        {
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          selection = menuItems[i];
          tft.print("-> ");
        }
        else
        {
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.print("   ");
        }

        tft.println(menuItems[i]);
        menuItemCount++;
      }
    }

    bool btnloop = true;
    while (btnloop)
    {
      maybeSleepDevice();
      key_val = "";
      getKeypad(false, true, false, false);

      if (key_val == "*")
      {
        do
        {
          menuItemNo++;
          menuItemNo %= sizeof(menuItems) / sizeof(menuItems[0]);
        } while (menuItemCheck[menuItemNo] == 0);

        btnloop = false;
      }
      else if (key_val == "#")
      {
        selected = false;
        btnloop = false;
      }
      else
      {
        updateBatteryStatus();
        delay(100);
      }
    }
  }
}

bool checkOnlineParams()
{
  if (invoice != "" && invoice.length() != 32)
  {
    error("WRONG INVOICE");
    delay(3000);
    return false;
  }

  const char *decimal = decimalplaces.c_str();
  if (!isInteger(decimal))
  {
    error("WRONG DECIMAL");
    delay(3000);
    return false;
  }

  lnbitsServer.toLowerCase();

  if (lnbitsServer != "")
  {
    const char *lnServer = lnbitsServer.c_str();
    char lastChar = lnServer[strlen(lnServer) - 1];

    if (lastChar == '/')
    {
      error("WRONG LNBITS");
      delay(3000);

      return false;
    }
  }

  return true;
}

bool checkOfflineParams()
{
  if (baseURLPoS != "" && baseURLPoS.substring(0, 4) != "http")
  {
    error("WRONG LNURLPoS");
    delay(3000);
    return false;
  }

  if (baseURLATM != "" && baseURLATM.substring(0, 4) != "http")
  {
    error("WRONG LNURLATM");
    delay(3000);
    return false;
  }

  if (!isInteger(decimalplaces.c_str()))
  {
    error("WRONG DECIMAL");
    delay(3000);
    return false;
  }

  return true;
}

//////////LIGHTNING//////////////////////

bool getSats()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  lnbitsServer.toLowerCase();
  if (lnbitsServer.substring(0, 8) == "https://")
  {
    lnbitsServer = lnbitsServer.substring(8, lnbitsServer.length());
  }
  const char *lnbitsServerChar = lnbitsServer.c_str();
  const char *invoiceChar = invoice.c_str();
  const char *lncurrencyChar = lncurrency.c_str();

  Serial.println("connecting to LNbits server " + lnbitsServer);
  if (!client.connect(lnbitsServerChar, 443))
  {
    Serial.println("failed to connect to LNbits server " + lnbitsServer);
    return false;
  }

  const String toPost = "{\"amount\" : 1, \"from\" :\"" + String(lncurrencyChar) + "\"}";
  const String url = "/api/v1/conversion";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + String(lnbitsServerChar) + "\r\n" + "User-Agent: ESP32\r\n" + "X-Api-Key: " + String(invoiceChar) + " \r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n" + "Content-Length: " + toPost.length() + "\r\n" + "\r\n" + toPost + "\n");

  while (client.connected())
  {
    const String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }

  const String line = client.readString();
  StaticJsonDocument<150> doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return false;
  }

  converted = doc["sats"];
  return true;
}

bool getInvoice()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  lnbitsServer.toLowerCase();
  if (lnbitsServer.substring(0, 8) == "https://")
  {
    lnbitsServer = lnbitsServer.substring(8, lnbitsServer.length());
  }
  const char *lnbitsServerChar = lnbitsServer.c_str();
  const char *invoiceChar = invoice.c_str();

  if (!client.connect(lnbitsServerChar, 443))
  {
    Serial.println("failed");
    error("SERVER DOWN");
    delay(3000);
    return false;
  }

  const String toPost = "{\"out\": false,\"amount\" : " + String(noSats.toInt()) + ", \"memo\" :\"LNPoS-" + String(random(1, 1000)) + "\"}";
  const String url = "/api/v1/payments";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + lnbitsServerChar + "\r\n" + "User-Agent: ESP32\r\n" + "X-Api-Key: " + invoiceChar + " \r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n" + "Content-Length: " + toPost.length() + "\r\n" + "\r\n" + toPost + "\n");

  while (client.connected())
  {
    const String line = client.readStringUntil('\n');

    if (line == "\r")
    {
      break;
    }
  }
  const String line = client.readString();

  StaticJsonDocument<1000> doc;
  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return false;
  }

  const char *payment_hash = doc["checking_id"];
  const char *payment_request = doc["payment_request"];
  qrData = payment_request;
  dataId = payment_hash;

  Serial.println(qrData);
  return true;
}

bool checkInvoice()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  const char *lnbitsServerChar = lnbitsServer.c_str();
  const char *invoiceChar = invoice.c_str();
  if (!client.connect(lnbitsServerChar, 443))
  {
    error("SERVER DOWN");
    delay(3000);
    return false;
  }

  const String url = "/api/v1/payments/";
  client.print(String("GET ") + url + dataId + " HTTP/1.1\r\n" + "Host: " + lnbitsServerChar + "\r\n" + "User-Agent: ESP32\r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n\r\n");
  while (client.connected())
  {
    const String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      break;
    }
  }

  const String line = client.readString();
  Serial.println(line);
  StaticJsonDocument<2000> doc;

  DeserializationError error = deserializeJson(doc, line);
  if (error)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return false;
  }
  if (doc["paid"])
  {
    unConfirmed = false;
  }

  return unConfirmed;
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  const int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//////////UTILS///////////////
void to_upper(char *arr)
{
  for (size_t i = 0; i < strlen(arr); i++)
  {
    if (arr[i] >= 'a' && arr[i] <= 'z')
    {
      arr[i] = arr[i] - 'a' + 'A';
    }
  }
}

bool makeLNURL()
{
  if (amountToShow.toFloat() == 0)
  {
    error("ZERO VALUE");
    delay(3000);
    return false;
  }

  randomPin = random(1000, 9999);
  byte nonce[8];
  for (int i = 0; i < 8; i++)
  {
    nonce[i] = random(256);
  }

  int multipler = pow(10, 2);

  if (currencyPoS == "sat")
  {
    multipler = 1;
  }

  float total = amountToShow.toFloat() * multipler;

  byte payload[51]; // 51 bytes is max one can get with xor-encryption
  if (selection == "Offline PoS")
  {
    size_t payload_len = xor_encrypt(payload, sizeof(payload), (uint8_t *)secretPoS.c_str(), secretPoS.length(), nonce, sizeof(nonce), randomPin, total);
    preparedURL = baseURLPoS + "?p=";
    preparedURL += toBase64(payload, payload_len, BASE64_URLSAFE | BASE64_NOPADDING);
  }
  else // ATM
  {
    size_t payload_len = xor_encrypt(payload, sizeof(payload), (uint8_t *)secretATM.c_str(), secretATM.length(), nonce, sizeof(nonce), randomPin, total);
    preparedURL = baseURLATM + "?atm=1&p=";
    preparedURL += toBase64(payload, payload_len, BASE64_URLSAFE | BASE64_NOPADDING);
  }

  Serial.println(preparedURL);
  char Buf[200];
  preparedURL.toCharArray(Buf, 200);
  char *url = Buf;
  byte *data = (byte *)calloc(strlen(url) * 2, sizeof(byte));
  size_t len = 0;
  int res = convert_bits(data, &len, 5, (byte *)url, strlen(url), 8, 1);
  char *charLnurl = (char *)calloc(strlen(url) * 2, sizeof(byte));
  bech32_encode(charLnurl, "lnurl", data, len);
  to_upper(charLnurl);
  qrData = charLnurl;
  Serial.println(qrData);

  return true;
}

int xor_encrypt(uint8_t *output, size_t outlen, uint8_t *key, size_t keylen, uint8_t *nonce, size_t nonce_len, uint64_t pin, uint64_t amount_in_cents)
{
  // check we have space for all the data:
  // <variant_byte><len|nonce><len|payload:{pin}{amount}><hmac>
  if (outlen < 2 + nonce_len + 1 + lenVarInt(pin) + 1 + lenVarInt(amount_in_cents) + 8)
  {
    return 0;
  }

  int cur = 0;
  output[cur] = 1; // variant: XOR encryption
  cur++;

  // nonce_len | nonce
  output[cur] = nonce_len;
  cur++;
  memcpy(output + cur, nonce, nonce_len);
  cur += nonce_len;

  // payload, unxored first - <pin><currency byte><amount>
  int payload_len = lenVarInt(pin) + 1 + lenVarInt(amount_in_cents);
  output[cur] = (uint8_t)payload_len;
  cur++;
  uint8_t *payload = output + cur;                                 // pointer to the start of the payload
  cur += writeVarInt(pin, output + cur, outlen - cur);             // pin code
  cur += writeVarInt(amount_in_cents, output + cur, outlen - cur); // amount
  cur++;

  // xor it with round key
  uint8_t hmacresult[32];
  SHA256 h;
  h.beginHMAC(key, keylen);
  h.write((uint8_t *)"Round secret:", 13);
  h.write(nonce, nonce_len);
  h.endHMAC(hmacresult);
  for (int i = 0; i < payload_len; i++)
  {
    payload[i] = payload[i] ^ hmacresult[i];
  }

  // add hmac to authenticate
  h.beginHMAC(key, keylen);
  h.write((uint8_t *)"Data:", 5);
  h.write(output, cur);
  h.endHMAC(hmacresult);
  memcpy(output + cur, hmacresult, 8);
  cur += 8;

  // return number of bytes written to the output
  return cur;
}

unsigned int getBatteryPercentage()
{
  const float batteryMaxVoltage = 4.2;
  const float batteryMinVoltage = 3.73;

  const float batteryAllowedRange = batteryMaxVoltage - batteryMinVoltage;
  const float batteryCurVAboveMin = getInputVoltage() - batteryMinVoltage;

  const int batteryPercentage = (int)(batteryCurVAboveMin / batteryAllowedRange * 100);
  if (batteryPercentage > 150)
  {
    return USB_POWER;
  }

  return max(min(batteryPercentage, 100), 0);
}

float getInputVoltage()
{
  delay(100);
  const uint16_t v1 = analogRead(34);
  return ((float)v1 / 4095.0f) * 2.0f * 3.3f * (1100.0f / 1000.0f);
}

/**
 * Check whether the device should be put to sleep and put it to sleep
 * if it should
 */
void maybeSleepDevice()
{
  if (isSleepEnabled && !isPretendSleeping)
  {
    long currentTime = millis();

    if (currentTime > (timeOfLastInteraction + sleepTimer * 1000))
    {
      sleepAnimation();
      // The device wont charge if it is sleeping, so when charging, do a pretend sleep
      if (isPoweredExternally())
      {
        isLilyGoKeyboard();
        Serial.println("Pretend sleep now");
        isPretendSleeping = true;
        tft.fillScreen(TFT_BLACK);
      }
      else
      {
        if (isLilyGoKeyboard())
        {
          esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1); // 1 = High, 0 = Low
        }
        else
        {
          // Configure Touchpad as wakeup source
          touchAttachInterrupt(T3, callback, 40);
          esp_sleep_enable_touchpad_wakeup();
        }
        Serial.println("Going to sleep now");
        esp_deep_sleep_start();
      }
    }
  }
}

void callback() {}

void adjustQrBrightness(bool shouldMakeBrighter, InvoiceType invoiceType)
{
  if (shouldMakeBrighter && qrScreenBrightness >= 0)
  {
    qrScreenBrightness = qrScreenBrightness + 25;
    if (qrScreenBrightness > 255)
    {
      qrScreenBrightness = 255;
    }
  }
  else if (!shouldMakeBrighter && qrScreenBrightness <= 30)
  {
    qrScreenBrightness = qrScreenBrightness - 5;
  }
  else if (!shouldMakeBrighter && qrScreenBrightness <= 255)
  {
    qrScreenBrightness = qrScreenBrightness - 25;
  }

  if (qrScreenBrightness < 4)
  {
    qrScreenBrightness = 4;
  }

  qrScreenBgColour = tft.color565(qrScreenBrightness, qrScreenBrightness, qrScreenBrightness);

  switch (invoiceType)
  {
  case LNPOS:
    qrShowCodeln();
    break;
  case LNURLPOS:
    qrShowCodeLNURL(" *MENU #SHOW PIN");
    break;
  case ONCHAIN:
    qrShowCodeOnchain(true, " *MENU #CHECK");
    break;
  case LNURLATM:
    qrShowCodeLNURL(" *MENU");
    break;
  default:
    break;
  }

  File configFile = SPIFFS.open("/config.txt", "w");
  configFile.print(String(qrScreenBrightness));
  configFile.close();
}

/**
 * Load stored config values
 */
void loadConfig()
{
  File file = SPIFFS.open("/config.txt");
  spiffing = file.readStringUntil('\n');
  String tempQrScreenBrightness = spiffing.c_str();
  int tempQrScreenBrightnessInt = tempQrScreenBrightness.toInt();
  Serial.println("spiffcontent " + String(tempQrScreenBrightnessInt));
  file.close();

  if (tempQrScreenBrightnessInt && tempQrScreenBrightnessInt > 3)
  {
    qrScreenBrightness = tempQrScreenBrightnessInt;
  }
  Serial.println("qrScreenBrightness from config " + String(qrScreenBrightness));
  qrScreenBgColour = tft.color565(qrScreenBrightness, qrScreenBrightness, qrScreenBrightness);
}

/**
 * Handle user inputs for adjusting the screen brightness
 */
void handleBrightnessAdjust(String keyVal, InvoiceType invoiceType)
{
  // Handle screen brighten on QR screen
  if (keyVal == "1")
  {
    Serial.println("Adjust bnrightness " + invoiceType);
    adjustQrBrightness(true, invoiceType);
  }
  // Handle screen dim on QR screen
  else if (keyVal == "4")
  {
    Serial.println("Adjust bnrightness " + invoiceType);
    adjustQrBrightness(false, invoiceType);
  }
}

/*
 * Get the keypad type
 */
boolean isLilyGoKeyboard()
{
  if (colPins[0] == 33)
  {
    return true;
  }
  return false;
}

/**
 * Does the device have external or internal power?
 */
bool isPoweredExternally()
{
  Serial.println("Is powered externally?");
  float inputVoltage = getInputVoltage();
  if (inputVoltage > 4.5)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Awww. Show the go to sleep animation
 */
void sleepAnimation()
{
  printSleepAnimationFrame("(o.o)", 500);
  printSleepAnimationFrame("(-.-)", 500);
  printSleepAnimationFrame("(-.-)z", 250);
  printSleepAnimationFrame("(-.-)zz", 250);
  printSleepAnimationFrame("(-.-)zzz", 250);
  tft.fillScreen(TFT_BLACK);
}

void wakeAnimation()
{
  printSleepAnimationFrame("(-.-)", 100);
  printSleepAnimationFrame("(o.o)", 200);
  tft.fillScreen(TFT_BLACK);
}

/**
   Print the line of the animation
*/
void printSleepAnimationFrame(String text, int wait)
{
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 80);
  tft.setTextSize(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  // tft.setFreeFont(BIGFONT);
  tft.println(text);
  delay(wait);
}
