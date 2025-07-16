#pragma once

#include "defines.h"

// Hardware setup, pin definitions, display, battery, sleep, and related functions

void logo();
void updateBatteryStatus(bool force = false);
unsigned int getBatteryPercentage();
float getInputVoltage();
void maybeSleepDevice();
void sleepAnimation();
void wakeAnimation();
void printSleepAnimationFrame(String text, int wait);
boolean isLilyGoKeyboard();
bool isPoweredExternally();
void callback();
