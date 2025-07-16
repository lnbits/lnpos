#include "defines.h"

// All global variable definitions are now in this file only

String lnurlPoS;
String lnurlATM;
String lncurrency;
String lnurlATMMS;
String lnurlATMPin;
String decimalplaces;

bool format = false;

String inputs;
String thePin;
String spiffing;
String nosats;
String cntr;
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
String dataIn;
String noSats;
String qrData;
String dataId;
String addressNo;
String pinToShow;
String amountToShow;
String key_val;
String selection;
String masterKey;
String lnbitsServer;
String invoice;
String ssid;
String password;

const char menuItems[5][13] = {"LNPoS", "Offline PoS", "OnChain", "ATM", "Settings"};
const char currencyItems[3][5] = {"sat", "USD", "EUR"};
char decimalplacesOutput[20] = {0};
int menuItemCheck[5] = {0, 0, 0, 0, 1};
int menuItemNo = 0;
int randomPin = 0;
int calNum = 0;
int sumFlag = 0;
int converted = 0;
int sleepTimer = 30;
int qrScreenBrightness = 180;
long timeOfLastInteraction = 0;
bool isSleepEnabled = true;
bool isPretendSleeping = false;
bool onchainCheck = false;
bool lnCheck = false;
bool lnurlCheck = false;
bool unConfirmed = true;
bool selected = false;
bool lnurlCheckPoS = false;
bool lnurlCheckATM = false;
bool hardcoded = false;
double amountToShowNumber = 0.0;

SHA256 h;
TFT_eSPI tft = TFT_eSPI();
uint16_t qrScreenBgColour = 0;
const byte rows = 4;
const byte cols = 3;
char keys[4][3] = {{'1','2','3'},{'4','5','6'},{'7','8','9'},{'*','0','#'}};
byte rowPins[4] = {21, 27, 26, 22};
byte colPins[3] = {33, 32, 25};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int checker = 0;
char maxdig[20] = {0};

fs::SPIFFSFS &FlashFS = SPIFFS;
const bool FORMAT_ON_FAIL = true;
