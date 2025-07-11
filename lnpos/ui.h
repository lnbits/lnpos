#ifndef UI_H
#define UI_H

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
    tft.println(String(lnCurrency) + ": ");
    tft.println("SAT: ");
    tft.setCursor(0, 120);
    tft.setTextSize(2);
    tft.println(" *MENU #INVOICE");

    if (!cleared)
    {
        amountToShowNumber = dataIn.toFloat() / pow(10, fiatDecimalPlaces.toInt());
        formatNumber(amountToShowNumber, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = String(fiatDecimalPlacesOutput);
        noSats = String(converted * amountToShowNumber);
    }
    else
    {
        noSats = "0";
        dataIn = "0";
        formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = fiatDecimalPlacesOutput;
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
        amountToShowNumber = dataIn.toFloat() / pow(10, fiatDecimalPlaces.toInt());
        formatNumber(amountToShowNumber, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = String(fiatDecimalPlacesOutput);
    }
    else
    {
        dataIn = "0";
        formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = fiatDecimalPlacesOutput;
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
        amountToShowNumber = dataIn.toFloat() / pow(10, fiatDecimalPlaces.toInt());
        formatNumber(amountToShowNumber, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = String(fiatDecimalPlacesOutput);
    }
    else
    {
        dataIn = "0";
        formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
        amountToShow = fiatDecimalPlacesOutput;
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

    const char *qrDataChar = qrData.c_str();
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
    tft.print(VERSION);
    tft.setCursor(0, 80);
    tft.print("Powered by LNbits");
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
                lnCurrency = currencyItems[i];
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

#endif