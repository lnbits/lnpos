#include "helpers.h"
#include "ui.h"
#include "defines.h"
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

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

void formatNumber(float number, int fiatDecimalPlaces, char *output)
{
    // Create a format string based on the fiatDecimalPlaces
    char formatString[10];
    sprintf(formatString, "%%.%df", fiatDecimalPlaces);

    // Use the format string to write the number to the output buffer
    sprintf(output, formatString, number);
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
        offlinePoS = "";
        offlineATM = "";
        masterKey = "";
        lnbitsServer = "";
        invoice = "";
        lnCurrency = "";
        mempool = "";
        securityPin = "";
        fiatDecimalPlaces = "";
        ssid = "";
        password = "";
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

void usbAccessPoint()
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

        if (pinToShow.length() == securityPin.length() && pinToShow != securityPin)
        {
            error("  WRONG PIN");
            delay(1500);

            pinToShow = "";
            dataIn = "";
            isATMMoneyPin(true);
        }
        else if (pinToShow == securityPin)
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

float getInputVoltage()
{
    delay(100);
    const uint16_t v1 = analogRead(34);
    return ((float)v1 / 4095.0f) * 2.0f * 3.3f * (1100.0f / 1000.0f);
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

///////////DISPLAY///////////////
void serialLaunch()
{
    configLaunch("USB Config");
}

long int lastBatteryCheck = 0;
void updateBatteryStatus(bool force)
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

bool checkOnlineParams()
{
    if (invoice != "" && invoice.length() != 32)
    {
        error("WRONG INVOICE");
        delay(3000);
        return false;
    }

    const char *decimal = fiatDecimalPlaces.c_str();
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
        error("WRONG offlinePoS");
        delay(3000);
        return false;
    }

    if (baseURLATM != "" && baseURLATM.substring(0, 4) != "http")
    {
        error("WRONG offlineATM");
        delay(3000);
        return false;
    }

    if (!isInteger(fiatDecimalPlaces.c_str()))
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
    const char *lnCurrencyChar = lnCurrency.c_str();

    Serial.println("connecting to LNbits server " + lnbitsServer);
    if (!client.connect(lnbitsServerChar, 443))
    {
        Serial.println("failed to connect to LNbits server " + lnbitsServer);
        return false;
    }

    const String toPost = "{\"amount\" : 1, \"from\" :\"" + String(lnCurrencyChar) + "\"}";
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



//////////ENCRYPTION///////////////
void encrypt(unsigned char *key, unsigned char *iv, int length, const char *plainText, unsigned char *outputBuffer)
{
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, key, 256); // AES-256 requires a 32-byte key
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, length, iv, (const unsigned char *)plainText, outputBuffer);
    mbedtls_aes_free(&aes);
}

void deriveKeyAndIV(const char *secret, unsigned char *salt, unsigned char *outputBuffer)
{
    mbedtls_md5_context md5_ctx;
    unsigned char data[24];      // 16 bytes key + 8 bytes salt
    unsigned char md5Output[16]; // 16 bytes for MD5 output

    memcpy(data, secret, 16);
    memcpy(data + 16, salt, 8);

    // first iteration
    mbedtls_md5_init(&md5_ctx);
    mbedtls_md5_update(&md5_ctx, data, sizeof(data));
    mbedtls_md5_finish(&md5_ctx, md5Output);

    // Copy the first 16 bytes to the output buffer for the key
    memcpy(outputBuffer, md5Output, 16);

    unsigned char data_md5[16 + 16 + 8]; // 16 bytes md5 output + 16 bytes key + 8 bytes salt

    for (int i = 16; i <= 48; i += 16)
    {
        memcpy(data_md5, md5Output, 16);
        memcpy(data_md5 + 16, data, 24);
        mbedtls_md5_init(&md5_ctx);
        mbedtls_md5_update(&md5_ctx, data_md5, sizeof(data_md5));
        mbedtls_md5_finish(&md5_ctx, md5Output);
        // Copy the next 16 bytes to the output buffer
        memcpy(outputBuffer + i, md5Output, 16);
    }

    mbedtls_md5_free(&md5_ctx);
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
    if (amountToShow.toFloat() <= 0)
    {
        error("ZERO VALUE");
        delay(3000);
        return false;
    }

    int multipler = pow(10, 2);
    if (currencyPoS == "sat")
    {
        multipler = 1;
    }

    float total = amountToShow.toFloat() * multipler;

    String secret;
    char hexbuffer[3];

    if (selection == "Offline PoS")
    {
        preparedURL = baseURLPoS;
        secret = secretPoS;
    }
    else
    {
        // ATM
        preparedURL = baseURLATM;
        secret = secretATM;
    }

    int salt_length = 8;
    unsigned char salt[salt_length];

    for (int i = 0; i < salt_length; i++)
    {
        salt[i] = random(0, 256);
    }

    unsigned char keyIV[32 + 16] = {0};
    deriveKeyAndIV(secret.c_str(), salt, keyIV);

    unsigned char key[32] = {0};
    unsigned char iv[16] = {0};

    memcpy(key, keyIV, 32);
    memcpy(iv, keyIV + 32, 16);

    randomPin = random(1000, 9999);
    String payload = String(randomPin) + String(":") + String(total);

    Serial.print("payload: ");
    Serial.println(payload);

    size_t payload_len = payload.length();
    int padding = 16 - (payload_len % 16);
    payload_len += padding;
    for (int i = 0; i < padding; i++)
    {
        payload += String((char)padding);
    }

    unsigned char encrypted[payload_len] = {0};
    encrypt(key, iv, payload_len, payload.c_str(), encrypted);

    const unsigned char *saltedChars = (const unsigned char *)"Salted__";
    unsigned char salted[payload_len + 16];
    memcpy(salted, saltedChars, 8);
    memcpy(salted + 8, salt, salt_length);
    memcpy(salted + 16, encrypted, payload_len);

    preparedURL += "?p=";
    preparedURL += toBase64(salted, payload_len + 16, BASE64_URLSAFE);
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

void callback() {}

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
    case LNPOS_TYPE:
        qrShowCodeln();
        break;
    case OFFLINE_POS_TYPE:
        qrShowCodeLNURL(" *MENU #SHOW PIN");
        break;
    case ONCHAIN_TYPE:
        qrShowCodeOnchain(true, " *MENU #CHECK");
        break;
    case OFFLINE_ATM_TYPE:
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
        Serial.println("Adjust bnrightness " + String((int)invoiceType));
        adjustQrBrightness(true, invoiceType);
    }
    // Handle screen dim on QR screen
    else if (keyVal == "4")
    {
        Serial.println("Adjust bnrightness " + String((int)invoiceType));
        adjustQrBrightness(false, invoiceType);
    }
}