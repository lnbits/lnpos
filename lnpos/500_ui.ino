void displayLogo()
{
  clearTFT();
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(0, 30);
  tft.print("LN");
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.print("PoS");
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.print(VERSION);
  tft.setCursor(0, 80);
  tft.print("Powered by LNbits");
}

void displayEnterPin(String pin)
{
  clearTFT();
  printTFT(" ENTER SECRET PIN", 0, 20);
  printTFT("PIN:", 0, 50, 3);
  printTFT(" *MENU #CLEAR", 0, 120);
  String obscuredPinToShow = "";
  int pinLength = pin.length();
  for (size_t i = 0; i < pinLength; i++)
  {
    obscuredPinToShow += "*";
  }
  printGreenTFT(obscuredPinToShow, 75, 50, 3);
}

void displayEnterAmount(String dataIn)
{
  clearTFT();
  printTFT("  - ENTER AMOUNT -", 0, 20);
  printTFT(String(currencyPoS) + ": ", 0, 50, 3);
  printTFT(" *MENU #INVOICE", 0, 120);
  float num = dataIn.toFloat() / pow(10, config_fiat_decimal_places.toInt());
  char amountToShow[20];
  formatNumber(num, config_fiat_decimal_places.toInt(), amountToShow);
  printGreenTFT(String(amountToShow), 75, 50, 3);
}

void displayMenuFooterHeader()
{
  clearTFT();
  printOrangeTFT("      - MENU -", 0, 10);
  printTFT("  *NEXT #SELECT", 0, 120);
}

void displayMenuItems()
{
  int menuItemCount = 0;
  for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++)
  {
    if (menuItemCheck[i] == 1)
    {
      if (menuItems[i] == menuItems[menuItemNo])
      {
        selection = menuItems[i];
        printGreenTFT("-> " + String(menuItems[i]), 0, 40);
      }
      else
      {
        printTFT("   " + String(menuItems[i]), 0, 40);
      }
      menuItemCount++;
    }
  }
}

void displayBatteryStatus()
{
  unsigned int batteryPercentage = getBatteryPercentage();
  String batteryPercentageText = "";
  if (batteryPercentage == USB_POWER)
  {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
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

void displayQrCode(String data)
{
  printTFT(" *MENU #SHOW PIN", 0, 120);
  const int br = 200; // 0-255
  uint16_t qrScreenBgColour = tft.color565(br, br, br);
  tft.fillScreen(qrScreenBgColour);

  const char *qrDataChar = data.c_str();
  QRCode qrcoded;
  uint8_t qrcodeData[qrcode_getBufferSize(20)];
  qrcode_initText(&qrcoded, qrcodeData, 6, 0, qrDataChar);

  unsigned int pixSize = 3;
  unsigned int offsetTop = 5;
  unsigned int offsetLeft = 65;

  for (uint8_t y = 0; y < qrcoded.size; y++)
  {
    for (uint8_t x = 0; x < qrcoded.size; x++)
    {
      if (qrcode_getModule(&qrcoded, x, y))
      {
        tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, TFT_BLACK);
      }
      else
      {
        tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, qrScreenBgColour);
      }
    }
  }
}

void showPin(int randomPin)
{
  clearTFT();
  printTFT("PROOF PIN", 40, 5, 3);
  printGreenTFT(String(randomPin), 70, 60, 4);
}
