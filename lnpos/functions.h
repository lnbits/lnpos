#pragma once

#include "defines.h"

// Business logic, helpers, networking, encryption, and utility functions

void checkHardcoded();
void readFiles();
void getKeypad(bool isATMPin, bool justKey, bool isLN, bool isATMNum);
void isLNMoneyNumber(bool cleared);
void isLNURLMoneyNumber(bool cleared);
void isATMMoneyNumber(bool cleared);
void isATMMoneyPin(bool cleared);
void qrShowCodeln();
void qrShowCodeOnchain(bool anAddress, String message);
void qrShowCodeLNURL(String message);
void error(String message);
void error(String message, String additional, String additional2);
void processing(String message);
void paymentSuccess();
String getJsonValue(JsonDocument &doc, const char *name);
bool checkOnlineParams();
bool checkOfflineParams();
bool getSats();
bool getInvoice();
bool checkInvoice();
String getValue(String data, char separator, int index);
void to_upper(char *arr);
bool makeLNURL();
void formatNumber(float value, int decimals, char* output);
void encrypt(unsigned char* key, unsigned char* iv, int length, const char* plainText, unsigned char* outputBuffer);
void deriveKeyAndIV(const char* secret, unsigned char* salt, unsigned char* outputBuffer);
void handleBrightnessAdjust(String keyVal, InvoiceType invoiceType);
