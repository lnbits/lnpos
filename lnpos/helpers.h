#ifndef HELPERS_H
#define HELPERS_H

#include "defines.h" // Ensure InvoiceType and global variables are visible
#include <Arduino.h>
#include <ArduinoJson.h>

// General
bool isInteger(const char *str);
void formatNumber(double number, int fiatDecimalPlaces, char *output);
String getJsonValue(JsonDocument &doc, const char *name);
String getValue(String data, char separator, int index);
void to_upper(char *arr);

// Keypad/User Input
void getKeypad(bool isATMPin, bool justKey, bool isLN, bool isATMNum);
void usbAccessPoint();
boolean isLilyGoKeyboard();

// Display/TFT/Brightness
void serialLaunch();
void updateBatteryStatus(bool force = false);
void adjustQrBrightness(bool shouldMakeBrighter, InvoiceType invoiceType);
void handleBrightnessAdjust(String keyVal, InvoiceType invoiceType);

// Power/Battery
float getInputVoltage();
unsigned int getBatteryPercentage();
bool isPoweredExternally();

// Hardcoded & Config
void checkHardcoded();
void loadConfig();

// Validation
bool checkOnlineParams();
bool checkOfflineParams();
bool makeLNURL();
void qrShowCodeLNURL(String message);
void showPin();
void isLNURLMoneyNumber(bool cleared);
void isATMMoneyPin(bool cleared);
void isATMMoneyNumber(bool cleared);

// Encryption
void encrypt(unsigned char *key, unsigned char *iv, int length, const char *plainText, unsigned char *outputBuffer);
void deriveKeyAndIV(const char *secret, unsigned char *salt, unsigned char *outputBuffer);

// Sleep Logic
void maybeSleepDevice();
void callback();
void error(String message);

#endif