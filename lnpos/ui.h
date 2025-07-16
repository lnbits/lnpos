#ifndef UI_H
#define UI_H

#include <Arduino.h>

// UI Screens and Drawing
void configLaunch(String title);
void isLNMoneyNumber(bool cleared);
void isLNURLMoneyNumber(bool cleared);
void isATMMoneyNumber(bool cleared);
void isATMMoneyPin(bool cleared);
void inputScreenOnChain();
void qrShowCodeln();
void qrShowCodeOnchain(bool anAddress, String message);
void qrShowCodeLNURL(String message);
void error(String message, String additional, String additional2);
void error(String message);
void processing(String message);
void paymentSuccess();
void showPin();
void lnurlInputScreen();
void logo();
void currencyLoop();
void menuLoop();
void printSleepAnimationFrame(String text, int wait);
void sleepAnimation();
void wakeAnimation();

#endif