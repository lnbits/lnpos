#include "menu.h"
#include "hardware.h"
#include "functions.h"

// Menu logic, user interaction, and display/menu-related functions implementation

void menuLoop() {
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
    selection = "";
    selected = true;
    while (selected) {
        maybeSleepDevice();
        if (menuItemCheck[0] <= 0 && menuItemNo == 0) {
            menuItemNo++;
        }
        tft.setCursor(0, 40);
        tft.setTextSize(2);
        int current = 0;
        int menuItemCount = 0;
        for (int i = 0; i < sizeof(menuItems) / sizeof(menuItems[0]); i++) {
            if (menuItemCheck[i] == 1) {
                if (menuItems[i] == menuItems[menuItemNo]) {
                    tft.setTextColor(TFT_GREEN, TFT_BLACK);
                    selection = menuItems[i];
                    tft.print("-> ");
                } else {
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                    tft.print("   ");
                }
                tft.println(menuItems[i]);
                menuItemCount++;
            }
        }
        bool btnloop = true;
        while (btnloop) {
            maybeSleepDevice();
            key_val = "";
            getKeypad(false, true, false, false);
            if (key_val == "*") {
                do {
                    menuItemNo++;
                    menuItemNo %= sizeof(menuItems) / sizeof(menuItems[0]);
                } while (menuItemCheck[menuItemNo] == 0);
                btnloop = false;
            } else if (key_val == "#") {
                selected = false;
                btnloop = false;
            } else {
                updateBatteryStatus();
                delay(100);
            }
        }
    }
}

void currencyLoop() {
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
    while (currencySelected) {
        maybeSleepDevice();
        tft.setCursor(0, 40);
        tft.setTextSize(2);
        int menuItemCount = 0;
        for (int i = 0; i < sizeof(currencyItems) / sizeof(currencyItems[0]); i++) {
            if (currencyItems[i] == currencyItems[currencyItemNo]) {
                tft.setTextColor(TFT_GREEN, TFT_BLACK);
                lncurrency = currencyItems[i];
            } else {
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
            }
            tft.print("  ");
            tft.println(currencyItems[i]);
            menuItemCount++;
        }
        bool btnloop = true;
        while (btnloop) {
            maybeSleepDevice();
            key_val = "";
            getKeypad(false, true, false, false);
            if (key_val == "*") {
                currencyItemNo++;
                currencyItemNo %= sizeof(currencyItems) / sizeof(currencyItems[0]);
                btnloop = false;
            } else if (key_val == "#") {
                currencySelected = false;
                btnloop = false;
            } else {
                updateBatteryStatus();
                delay(100);
            }
        }
    }
}

void accessPoint() {
    readFiles();
    pinToShow = "";
    dataIn = "";
    isATMMoneyPin(true);
    while (unConfirmed) {
        key_val = "";
        getKeypad(true, false, false, false);
        if (key_val == "*") {
            unConfirmed = false;
        } else if (key_val == "#") {
            isATMMoneyPin(true);
        }
        if (pinToShow.length() == lnurlATMPin.length() && pinToShow != lnurlATMPin) {
            error("  WRONG PIN");
            delay(1500);
            pinToShow = "";
            dataIn = "";
            isATMMoneyPin(true);
        } else if (pinToShow == lnurlATMPin) {
            error("   SETTINGS", "HOLD 1 FOR USB", "");
            int count = 0;
            while (count < 10) {
                key_val = "";
                delay(200);
                count++;
                const char key = keypad.getKey();
                if (key == '1') {
                    // configOverSerialPort();
                    key_val = "";
                    getKeypad(false, true, false, false);
                    if (key_val == "*") {
                        return;
                    }
                }
            }
        } else {
            delay(100);
        }
    }
}

void inputScreenOnChain() {
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

void showPin() {
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
