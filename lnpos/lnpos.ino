#include <math.h>
#include <stdio.h>
#include <QRCodeGenerator.h>
#include <WiFiClientSecure.h>
#include <Bitcoin.h>
#include <ArduinoJson.h>

// config
String config_ssid;
String config_password;
String config_offline_pos;
String config_offline_atm;
String config_master_key;
String config_mempool;
String config_lnbits_server;
String config_invoice;
String config_ln_currency;
String config_security_pin;
String config_fiat_decimal_places;
String config_qr_screen_brightness;


// variables
bool isPretendSleeping = false;

String inputs;
String nosats;
String cntr = "0";
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
String selection;

const char menuItems[5][13] = {"Online PoS", "Offline PoS", "OnChain", "ATM", "Settings"};
const char currencyItems[3][5] = {"sat", "USD", "EUR"};
char decimalplacesOutput[20];
int menuItemCheck[5] = {0, 0, 0, 0, 1};
int menuItemNo = 0;
int calNum = 1;
int sumFlag = 0;
int converted = 0;
int qrScreenBrightness = 180; // 0 = min, 255 = max
long timeOfLastInteraction = millis();
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

void setup() {
  Serial.begin(115200);
  setupTFT();
  setupConfig();
  setupWifi();
  displayLogo();
}

void loop()
{
  loopWifi();

  noSats = "0";
  dataIn = "0";
  formatNumber(0, config_fiat_decimal_places.toInt(), decimalplacesOutput);
  unConfirmed = true;

  // check wifi status
  // if (menuItemCheck[0] == 1 && WiFi.status() != WL_CONNECTED)
  // {
  //   menuItemCheck[0] = -1;
  // }
  // else if (menuItemCheck[0] == -1 && WiFi.status() == WL_CONNECTED)
  // {
  //   menuItemCheck[0] = 1;
  // }

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
    // if (selection == "Online PoS")
    // {
    //   lnMain();
    // }
    // else if (selection == "OnChain")
    // {
    //   onchainMain();
    // }
    else if (selection == "Offline PoS")
    {
      lnurlPoSMain();
    }
    // else if (selection == "ATM")
    // {
    //   lnurlATMMain();
    // }
    else if (selection == "Settings")
    {
      accessPoint();
    }
  }
}

void accessPoint()
{
  dataIn = "";
  displayEnterPin(dataIn);

  while (unConfirmed)
  {
    char key = getKeypadInput();
    dataIn += key;

    if (key == '*')
    {
      unConfirmed = false;
    }
    else if (key == '#')
    {
      displayEnterPin(dataIn);
      dataIn = "";
    }

    if (dataIn.length() == config_security_pin.length() && dataIn != config_security_pin)
    {
      error("  WRONG PIN");
      delay(1500);
      dataIn = "";
      displayEnterPin(dataIn);
    }
    else if (dataIn == config_security_pin)
    {
      error("   SETTINGS", "HOLD 1 FOR USB \nHOLD 2 TO RESET", "");
      int count = 0;
      while (count < 10)
      {
        delay(200);
        count++;
        const char key2 = getKeypadInput();
        if (key2 == '1')
        {
          executeConfigForever();
          const char key3 = getKeypadInput();
          if (key3 == '*')
          {
            return;
          }
        }
        if (key == '2')
        {
          ESP.restart();
        }
      }
    }
    else
    {
      delay(100);
    }
  }
}

// // on-chain payment method
// void onchainMain()
// {
//   File file = SPIFFS.open(KEY_FILE);
//   if (file)
//   {
//     addressNo = file.readString();
//     addressNo = String(addressNo.toInt() + 1);
//     file.close();
//     file = SPIFFS.open(KEY_FILE, FILE_WRITE);
//     file.print(addressNo);
//     file.close();
//   }
//   else
//   {
//     file.close();
//     file = SPIFFS.open(KEY_FILE, FILE_WRITE);
//     addressNo = "1";
//     file.print(addressNo);
//     file.close();
//   }

//   Serial.println(addressNo);
//   inputScreenOnChain();

//   while (unConfirmed)
//   {
//     getKeypad(false, true, false, false);

//     if (key_val == "*")
//     {
//       unConfirmed = false;
//     }
//     else if (key_val == "#")
//     {
//       HDPublicKey hd(masterKey);
//       qrData = hd.derive(String("m/0/") + addressNo).address();
//       qrShowCodeOnchain(true, " *MENU #CHECK");

//       while (unConfirmed)
//       {
//         key_val = "";
//         getKeypad(false, true, false, false);

//         if (key_val == "*")
//         {
//           unConfirmed = false;
//         }
//         else if (key_val == "#")
//         {
//           while (unConfirmed)
//           {
//             qrData = mempool + "/address/" + qrData;
//             qrShowCodeOnchain(false, " *MENU");

//             while (unConfirmed)
//             {
//               key_val = "";
//               getKeypad(false, true, false, false);

//               if (key_val == "*")
//               {
//                 unConfirmed = false;
//               }
//             }
//           }
//         }
//         handleBrightnessAdjust(key_val, ONCHAIN);
//       }
//     }
//   }
// }

// void lnMain()
// {
//   if (!checkOnlineParams())
//   {
//     return;
//   }

//   if (lnCurrency == "" || lnCurrency == "default")
//   {
//     currencyLoop();
//   }

//   processing("FETCHING FIAT RATE");
//   if (!getSats())
//   {
//     error("FETCHING FIAT RATE FAILED");
//     delay(3000);
//     return;
//   }

//   isLNMoneyNumber(true);

//   while (unConfirmed)
//   {
//     key_val = "";
//     getKeypad(false, false, true, false);

//     if (key_val == "*")
//     {
//       unConfirmed = false;
//     }
//     else if (key_val == "#")
//     {
//       if (noSats.toInt() == 0)
//       {
//         error("ZERO SATS");
//         delay(3000);
//         isLNMoneyNumber(true);
//         continue;
//       }

//       // request invoice
//       processing("FETCHING INVOICE");
//       if (!getInvoice())
//       {
//         unConfirmed = false;
//         error("ERROR FETCHING INVOICE");
//         delay(3000);
//         break;
//       }

//       // show QR
//       qrShowCodeln();

//       // check invoice
//       bool isFirstRun = true;
//       while (unConfirmed)
//       {
//         int timer = 0;

//         if (!isFirstRun)
//         {
//           unConfirmed = checkInvoice();
//           if (!unConfirmed)
//           {
//             paymentSuccess();
//             timer = 5000;

//             while (key_val != "*")
//             {
//               key_val = "";
//               getKeypad(false, true, false, false);

//               if (key_val != "*")
//               {
//                 delay(100);
//               }
//             }
//           }
//         }

//         // abort on * press
//         while (timer < (isFirstRun ? 6000 : 2000))
//         {
//           getKeypad(false, true, false, false);

//           if (key_val == "*")
//           {
//             noSats = "0";
//             dataIn = "0";
//             formatNumber(0, fiatDecimalPlaces.toInt(), decimalplacesOutput);
//             amountToShow = decimalplacesOutput;
//             unConfirmed = false;
//             timer = 5000;
//             break;
//           }
//           else
//           {
//             delay(100);
//             handleBrightnessAdjust(key_val, LNPOS);
//             key_val = "";
//           }
//           timer = timer + 100;
//         }

//         isFirstRun = false;
//       }

//       noSats = "0";
//       dataIn = "0";
//       formatNumber(0, fiatDecimalPlaces.toInt(), decimalplacesOutput);
//       amountToShow = decimalplacesOutput;
//     }
//     else
//     {
//       delay(100);
//     }
//   }
// }

void lnurlPoSMain()
{
  // if (!checkOfflineParams())
  // {
  //   return;
  // }
  String amount = "0";
  displayEnterAmount(amount);

  while (unConfirmed)
  {
    const char key = getKeypadInput();
    if (key == NO_KEY)
    {
      continue;
    }
    if (key == '*')
    {
      unConfirmed = false;
    }
    else if (key == '#')
    {
      if (amount.toFloat() == 0)
      {
        error("ZERO AMOUNT");
        delay(3000);
        amount = "0";
        displayEnterAmount(amount);
        continue;
      }
      int randomPin = random(1000, 9999);
      String lnurl = makeLNURL(amount.toFloat(), randomPin);
      displayQrCode(lnurl);
      while (unConfirmed)
      {
        const char key2 = getKeypadInput();
        if (key2 == '#')
        {
          showPin(randomPin);
          while (unConfirmed)
          {
            const char key3 = getKeypadInput();
            if (key == '*')
            {
              unConfirmed = false;
            }
          }
        }
        else if (key == '*')
        {
          unConfirmed = false;
        }
        handleBrightnessAdjust(key, LNURLPOS);
      }
    }
    else
    {
      delay(100);
    }
  }
}

// void lnurlATMMain()
// {
//   pinToShow = "";
//   dataIn = "";
//   isATMMoneyPin(true);

//   while (unConfirmed)
//   {
//     key_val = "";
//     getKeypad(true, false, false, false);

//     if (key_val == "*")
//     {
//       unConfirmed = false;
//     }
//     else if (key_val == "#")
//     {
//       isATMMoneyPin(true);
//     }

//     if (pinToShow.length() == securityPin.length() && pinToShow != securityPin)
//     {
//       error("  WRONG PIN");
//       delay(1500);

//       pinToShow = "";
//       dataIn = "";
//       isATMMoneyPin(true);
//     }
//     else if (pinToShow == securityPin)
//     {
//       isATMMoneyNumber(true);
//       inputs = "";
//       dataIn = "";

//       while (unConfirmed)
//       {
//         key_val = "";
//         getKeypad(false, false, false, true);

//         if (key_val == "*")
//         {
//           unConfirmed = false;
//         }
//         else if (key_val == "#")
//         {
//           if (!makeLNURL())
//           {
//             isATMMoneyNumber(true);
//             continue;
//           }
//           printTFT(" *MENU");
//           qrShowCodeLNURL();

//           while (unConfirmed)
//           {
//             key_val = "";
//             getKeypad(false, true, false, false);
//             handleBrightnessAdjust(key_val, LNURLATM);

//             if (key_val == "*")
//             {
//               unConfirmed = false;
//             }
//           }
//         }
//       }
//     }
//     else
//     {
//       delay(100);
//     }
//   }
// }

// ///////////DISPLAY///////////////
// void isATMMoneyNumber(bool cleared)
// {
//   clearTFT();
//   printTFT("  - ENTER AMOUNT -", 0, 20);
//   printTFT(String(currencyATM) + ": ", 0, 50, 3);
//   printTFT(" *MENU #WITHDRAW", 0, 120);

//   if (!cleared)
//   {
//     amountToShowNumber = dataIn.toFloat() / pow(10, fiatDecimalPlaces.toInt());
//     formatNumber(amountToShowNumber, fiatDecimalPlaces.toInt(), decimalplacesOutput);
//     amountToShow = String(decimalplacesOutput);
//   }
//   else
//   {
//     dataIn = "0";
//     formatNumber(0, fiatDecimalPlaces.toInt(), decimalplacesOutput);
//     amountToShow = decimalplacesOutput;
//   }
//   printGreenTFT(amountToShow, 75, 50, 3);
// }

// void inputScreenOnChain()
// {
//   clearTFT();
//   printTFT("XPUB ENDING " + masterKey.substring(masterKey.length() - 5), 0, 40, 2);
//   printTFT("  *MENU #ADDRESS", 0, 120);
// }

// void configLaunch(String title) {
//   clearTFT();
//   printPurpleTFT(title, 20, 30, 3);
//   printTFT(" WHEN FINISHED *", 0, 65);
//   // printTFT(config.apid, 30, 83, 2);
// }

// void qrShowCodeln()
// {
//   clearTFT();

//   qrData.toUpperCase();
//   const char *qrDataChar = qrData.c_str();
//   QRCode qrcoded;
//   uint8_t qrcodeData[qrcode_getBufferSize(20)];

//   qrcode_initText(&qrcoded, qrcodeData, 11, 0, qrDataChar);

//   for (uint8_t y = 0; y < qrcoded.size; y++)
//   {
//     for (uint8_t x = 0; x < qrcoded.size; x++)
//     {
//       if (qrcode_getModule(&qrcoded, x, y))
//       {
//         tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, TFT_BLACK);
//       }
//       else
//       {
//         tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, qrScreenBgColour);
//       }
//     }
//   }
//   printTFT(" *MENU", 0, 220);
// }

// void qrShowCodeOnchain(bool anAddress, String message)
// {
//   // tft.fillScreen(qrScreenBgColour);

//   if (anAddress)
//   {
//     qrData.toUpperCase();
//   }

//   const char *qrDataChar = qrData.c_str();
//   QRCode qrcoded;
//   uint8_t qrcodeData[qrcode_getBufferSize(20)];
//   int pixSize = 0;

//   // tft.setCursor(0, 100);
//   // tft.setTextSize(2);
//   // tft.setTextColor(TFT_BLACK, qrScreenBgColour);

//   if (anAddress)
//   {
//     qrcode_initText(&qrcoded, qrcodeData, 2, 0, qrDataChar);
//     pixSize = 4;
//   }
//   else
//   {
//     qrcode_initText(&qrcoded, qrcodeData, 4, 0, qrDataChar);
//     pixSize = 3;
//   }

//   for (uint8_t y = 0; y < qrcoded.size; y++)
//   {
//     for (uint8_t x = 0; x < qrcoded.size; x++)
//     {
//       if (qrcode_getModule(&qrcoded, x, y))
//       {
//         tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, TFT_BLACK);
//       }
//       else
//       {
//         tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, qrScreenBgColour);
//       }
//     }
//   }
//   printTFT(message, 0, 120);
// }

// void paymentSuccess()
// {
//   clearTFT();
//   printGreenTFT("PAID", 70, 50, 3);
//   printTFT("  PRESS * FOR MENU", 0, 120);
// }

// void lnurlInputScreen()
// {
//   clearTFT();
//   printTFT("AMOUNT THEN #", 0, 0, 3);
//   printTFT("TO RESET PRESS *", 50, 110);
//   printTFT(String(currency), 0, 30, 3);
// }

long int lastBatteryCheck = 0;
void updateBatteryStatus(bool force = false)
{
  // throttle
  if (!force && lastBatteryCheck != 0 && millis() - lastBatteryCheck < 5000)
  {
    return;
  }
  lastBatteryCheck = millis();
  displayBatteryStatus();
}

// void currencyLoop()
// {
//   // footer/header
//   clearTFT();
//   printOrangeTFT("    - CURRENCY -", 0, 10, 2);
//   printTFT("  *NEXT #SELECT", 0, 120);

//   updateBatteryStatus(true);

//   bool currencySelected = true;
//   int currencyItemNo = 0;

//   while (currencySelected)
//   {
//     maybeSleepDevice();

//     int menuItemCount = 0;

//     for (int i = 0; i < sizeof(currencyItems) / sizeof(currencyItems[0]); i++)
//     {
//       if (currencyItems[i] == currencyItems[currencyItemNo])
//       {
//         lnCurrency = currencyItems[i];
//         printGreenTFT("  " + currencyItems[i], 0, 40);
//       }
//       else
//       {
//         printTFT("  " + currencyItems[i], 0, 40);
//       }
//       menuItemCount++;
//     }

//     bool btnloop = true;
//     while (btnloop)
//     {
//       maybeSleepDevice();
//       key_val = "";
//       getKeypad(false, true, false, false);

//       if (key_val == "*")
//       {
//         currencyItemNo++;
//         currencyItemNo %= sizeof(currencyItems) / sizeof(currencyItems[0]);

//         btnloop = false;
//       }
//       else if (key_val == "#")
//       {
//         currencySelected = false;
//         btnloop = false;
//       }
//       else
//       {
//         updateBatteryStatus();
//         delay(100);
//       }
//     }
//   }
// }

void menuLoop()
{
  displayMenuFooterHeader();
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

    displayMenuItems();

    bool btnloop = true;
    while (btnloop)
    {
      maybeSleepDevice();
      char key = getKeypadInput();
      if (key == '*')
      {
        do
        {
          menuItemNo++;
          menuItemNo %= sizeof(menuItems) / sizeof(menuItems[0]);
        } while (menuItemCheck[menuItemNo] == 0);
        btnloop = false;
      }
      else if (key == '#')
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

// bool checkOnlineParams()
// {
//   if (invoice != "" && invoice.length() != 32)
//   {
//     error("WRONG INVOICE");
//     delay(3000);
//     return false;
//   }

//   const char *decimal = fiatDecimalPlaces.c_str();
//   if (!isInteger(decimal))
//   {
//     error("WRONG DECIMAL");
//     delay(3000);
//     return false;
//   }

//   lnbitsServer.toLowerCase();

//   if (lnbitsServer != "")
//   {
//     const char *lnServer = lnbitsServer.c_str();
//     char lastChar = lnServer[strlen(lnServer) - 1];

//     if (lastChar == '/')
//     {
//       error("WRONG LNBITS");
//       delay(3000);

//       return false;
//     }
//   }

//   return true;
// }

// bool checkOfflineParams()
// {
//   if (baseURLPoS != "" && baseURLPoS.substring(0, 4) != "http")
//   {
//     error("WRONG LNURLPoS");
//     delay(3000);
//     return false;
//   }

//   if (baseURLATM != "" && baseURLATM.substring(0, 4) != "http")
//   {
//     error("WRONG LNURLATM");
//     delay(3000);
//     return false;
//   }

//   if (!isInteger(fiatDecimalPlaces.c_str()))
//   {
//     error("WRONG DECIMAL");
//     delay(3000);
//     return false;
//   }

//   return true;
// }


// void adjustQrBrightness(bool shouldMakeBrighter, InvoiceType invoiceType)
// {
//   if (shouldMakeBrighter && qrScreenBrightness >= 0)
//   {
//     qrScreenBrightness = qrScreenBrightness + 25;
//     if (qrScreenBrightness > 255)
//     {
//       qrScreenBrightness = 255;
//     }
//   }
//   else if (!shouldMakeBrighter && qrScreenBrightness <= 30)
//   {
//     qrScreenBrightness = qrScreenBrightness - 5;
//   }
//   else if (!shouldMakeBrighter && qrScreenBrightness <= 255)
//   {
//     qrScreenBrightness = qrScreenBrightness - 25;
//   }

//   if (qrScreenBrightness < 4)
//   {
//     qrScreenBrightness = 4;
//   }

//   // qrScreenBgColour = tft.color565(qrScreenBrightness, qrScreenBrightness, qrScreenBrightness);

//   switch (invoiceType)
//   {
//   case LNPOS:
//     qrShowCodeln();
//     break;
//   case LNURLPOS:
//     printTFT(" *MENU #SHOW PIN", 0, 220);
//     qrShowCodeLNURL();
//     break;
//   case ONCHAIN:
//     qrShowCodeOnchain(true, " *MENU #CHECK");
//     break;
//   case LNURLATM:
//     printTFT(" *MENU", 0, 220);
//     qrShowCodeLNURL();
//     break;
//   default:
//     break;
//   }

//   File configFile = SPIFFS.open("/config.txt", "w");
//   configFile.print(String(qrScreenBrightness));
//   configFile.close();
// }

// /**
//  * Handle user inputs for adjusting the screen brightness
//  */
// void handleBrightnessAdjust(String keyVal, InvoiceType invoiceType)
// {
//   // Handle screen brighten on QR screen
//   if (keyVal == "1")
//   {
//     Serial.println("Adjust brightness " + invoiceType);
//     adjustQrBrightness(true, invoiceType);
//   }
//   // Handle screen dim on QR screen
//   else if (keyVal == "4")
//   {
//     Serial.println("Adjust brightness " + invoiceType);
//     adjustQrBrightness(false, invoiceType);
//   }
// }
