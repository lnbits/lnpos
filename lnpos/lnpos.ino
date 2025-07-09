#include "defines.h"
#include "screens.h"
#include "helpers.h"
#include "ui.h"

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
