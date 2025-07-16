#include "defines.h"

// Global variable definitions for lnpos project

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

const char menuItems[5][13] = {"", "", "", "", ""}; // Fill with your actual menu items
const char currencyItems[3][5] = {"", "", ""}; // Fill with your actual currency items
char fiatDecimalPlacesOutput[20] = {0};
int menuItemCheck[5] = {0};
int menuItemNo = 0;
int randomPin = 0;
int calNum = 0;
int sumFlag = 0;
int converted = 0;
int sleepTimer = 0;
int qrScreenBrightness = 0;
long timeOfLastInteraction = 0;
bool isSleepEnabled = false;
bool isPretendSleeping = false;
bool onchainCheck = false;
bool lnCheck = false;
bool lnurlCheck = false;
bool unConfirmed = false;
bool selected = false;
bool lnurlCheckPoS = false;
bool lnurlCheckATM = false;
double amountToShowNumber = 0.0;

SHA256 h;
TFT_eSPI tft = TFT_eSPI();
uint16_t qrScreenBgColour = 0;
const byte rows = 4;
const byte cols = 3;
char keys[4][3] = {{0}};
byte rowPins[4] = {0};
byte colPins[3] = {0};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int checker = 0;
char maxdig[20] = {0};
