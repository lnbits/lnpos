#include "functions.h"
#include "qrcoded.h"

// Business logic, helpers, networking, encryption, and utility functions implementation

void checkHardcoded() {
    if (!hardcoded) {
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

void readFiles() {
    // Implement as needed for your project
}

void getKeypad(bool isATMPin, bool justKey, bool isLN, bool isATMNum) {
    const char key = keypad.getKey();
    if (key == NO_KEY) {
        return;
    }
    isPretendSleeping = false;
    key_val = String(key);
    if (key_val != "") {
        timeOfLastInteraction = millis();
    }
    if (dataIn.length() < 9) {
        dataIn += key_val;
    }
    if (isLN) {
        isLNMoneyNumber(false);
    } else if (isATMPin) {
        isATMMoneyPin(false);
    } else if (justKey) {
    } else if (isATMNum) {
        isATMMoneyNumber(false);
    } else {
        isLNURLMoneyNumber(false);
    }
}

void isLNMoneyNumber(bool cleared) {
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
    if (!cleared) {
        amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
        formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
        amountToShow = String(decimalplacesOutput);
        noSats = String(converted * amountToShowNumber);
    } else {
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

void isLNURLMoneyNumber(bool cleared) {
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
    if (!cleared) {
        amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
        formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
        amountToShow = String(decimalplacesOutput);
    } else {
        dataIn = "0";
        formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
        amountToShow = decimalplacesOutput;
    }
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(75, 50);
    tft.println(amountToShow);
}

void isATMMoneyNumber(bool cleared) {
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
    if (!cleared) {
        amountToShowNumber = dataIn.toFloat() / pow(10, decimalplaces.toInt());
        formatNumber(amountToShowNumber, decimalplaces.toInt(), decimalplacesOutput);
        amountToShow = String(decimalplacesOutput);
    } else {
        dataIn = "0";
        formatNumber(0, decimalplaces.toInt(), decimalplacesOutput);
        amountToShow = decimalplacesOutput;
    }
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(75, 50);
    tft.println(amountToShow);
}

void isATMMoneyPin(bool cleared) {
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
    for (size_t i = 0; i < pinLength; i++) {
        obscuredPinToShow += "*";
    }
    tft.setTextSize(3);
    if (cleared) {
        pinToShow = "";
        dataIn = "";
    }
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(75, 50);
    tft.println(obscuredPinToShow);
}

void qrShowCodeln() {
    tft.fillScreen(qrScreenBgColour);
    qrData.toUpperCase();
    const char *qrDataChar = qrData.c_str();
    QRCode qrcoded;
    uint8_t qrcodeData[qrcode_getBufferSize(20)];
    qrcode_initText(&qrcoded, qrcodeData, 11, 0, qrDataChar);
    for (uint8_t y = 0; y < qrcoded.size; y++) {
        for (uint8_t x = 0; x < qrcoded.size; x++) {
            if (qrcode_getModule(&qrcoded, x, y)) {
                tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, TFT_BLACK);
            } else {
                tft.fillRect(65 + 2 * x, 5 + 2 * y, 2, 2, qrScreenBgColour);
            }
        }
    }
    tft.setCursor(0, 220);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.print(" *MENU");
}

void qrShowCodeOnchain(bool anAddress, String message) {
    tft.fillScreen(qrScreenBgColour);
    if (anAddress) {
        qrData.toUpperCase();
    }
    const char *qrDataChar = qrData.c_str();
    QRCode qrcoded;
    uint8_t qrcodeData[qrcode_getBufferSize(20)];
    int pixSize = 0;
    tft.setCursor(0, 100);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, qrScreenBgColour);
    if (anAddress) {
        qrcode_initText(&qrcoded, qrcodeData, 2, 0, qrDataChar);
        pixSize = 4;
    } else {
        qrcode_initText(&qrcoded, qrcodeData, 4, 0, qrDataChar);
        pixSize = 3;
    }
    for (uint8_t y = 0; y < qrcoded.size; y++) {
        for (uint8_t x = 0; x < qrcoded.size; x++) {
            if (qrcode_getModule(&qrcoded, x, y)) {
                tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, TFT_BLACK);
            } else {
                tft.fillRect(70 + pixSize * x, 5 + pixSize * y, pixSize, pixSize, qrScreenBgColour);
            }
        }
    }
    tft.setCursor(0, 120);
    tft.println(message);
}

void qrShowCodeLNURL(String message) {
    tft.fillScreen(qrScreenBgColour);
    const char *qrDataChar = qrData.c_str();
    QRCode qrcoded;
    uint8_t qrcodeData[qrcode_getBufferSize(20)];
    qrcode_initText(&qrcoded, qrcodeData, 6, 0, qrDataChar);
    unsigned int pixSize = 3;
    unsigned int offsetTop = 5;
    unsigned int offsetLeft = 65;
    for (uint8_t y = 0; y < qrcoded.size; y++) {
        for (uint8_t x = 0; x < qrcoded.size; x++) {
            if (qrcode_getModule(&qrcoded, x, y)) {
                tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, TFT_BLACK);
            } else {
                tft.fillRect(offsetLeft + pixSize * x, offsetTop + pixSize * y, pixSize, pixSize, qrScreenBgColour);
            }
        }
    }
    tft.setCursor(0, 220);
    tft.setTextSize(2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.println(message);
}

void error(String message) {
    error(message, "", "");
}

void error(String message, String additional, String additional2) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(0, 30);
    tft.println(message);
    if (additional != "") {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(0, 100);
        tft.setTextSize(2);
        tft.println(additional);
    }
    if (additional2 != "") {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setCursor(0, 120);
        tft.setTextSize(2);
        tft.println(additional2);
    }
}

void processing(String message) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 60);
    tft.println(message);
}

void paymentSuccess() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(3);
    tft.setCursor(70, 50);
    tft.println("PAID");
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("  PRESS * FOR MENU");
}

String getJsonValue(JsonDocument &doc, const char *name) {
    for (JsonObject elem : doc.as<JsonArray>()) {
        if (strcmp(elem["name"], name) == 0) {
            String value = elem["value"].as<String>();
            return value;
        }
    }
    return "";
}

bool checkOnlineParams() {
    if (invoice != "" && invoice.length() != 32) {
        error("WRONG INVOICE");
        delay(3000);
        return false;
    }
    const char *decimal = decimalplaces.c_str();
    if (!isInteger(decimal)) {
        error("WRONG DECIMAL");
        delay(3000);
        return false;
    }
    lnbitsServer.toLowerCase();
    if (lnbitsServer != "") {
        const char *lnServer = lnbitsServer.c_str();
        char lastChar = lnServer[strlen(lnServer) - 1];
        if (lastChar == '/') {
            error("WRONG LNBITS");
            delay(3000);
            return false;
        }
    }
    return true;
}

bool checkOfflineParams() {
    if (baseURLPoS != "" && baseURLPoS.substring(0, 4) != "http") {
        error("WRONG LNURLPoS");
        delay(3000);
        return false;
    }
    if (baseURLATM != "" && baseURLATM.substring(0, 4) != "http") {
        error("WRONG LNURLATM");
        delay(3000);
        return false;
    }
    if (!isInteger(decimalplaces.c_str())) {
        error("WRONG DECIMAL");
        delay(3000);
        return false;
    }
    return true;
}

// Add missing stub for isInteger
bool isInteger(const char *str) {
    if (*str == '-' || *str == '+') str++;
    while (*str) {
        if (!isdigit(*str)) return false;
        str++;
    }
    return true;
}

// Add stubs for main logic functions
void lnMain() {}
void onchainMain() {}
void lnurlPoSMain() {}
void lnurlATMMain() {}
void serialLaunch() {}

// Stub implementation for formatNumber to resolve linker errors
void formatNumber(float value, int decimals, char* output) {
    // Simple stub: just print value as integer
    sprintf(output, "%.*f", decimals, value);
}
