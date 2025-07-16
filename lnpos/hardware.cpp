#include "hardware.h"

// Hardware setup, pin definitions, display, battery, sleep, and related functions implementation

void logo() {
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

void updateBatteryStatus(bool force) {
    static long int lastBatteryCheck = 0;
    if (!force && lastBatteryCheck != 0 && millis() - lastBatteryCheck < 5000) {
        return;
    }
    lastBatteryCheck = millis();
    const int batteryPercentage = getBatteryPercentage();
    String batteryPercentageText = "";
    if (batteryPercentage == USB_POWER) {
        tft.setTextColor(TFT_BLUE, TFT_BLACK);
        batteryPercentageText = " USB";
    } else {
        if (batteryPercentage >= 60) {
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
        } else if (batteryPercentage >= 20) {
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        } else {
            tft.setTextColor(TFT_RED, TFT_BLACK);
        }
        if (batteryPercentage != 100) {
            batteryPercentageText += " ";
            if (batteryPercentage < 10) {
                batteryPercentageText += " ";
            }
        }
        batteryPercentageText += String(batteryPercentage) + "%";
    }
    tft.setCursor(190, 120);
    tft.print(batteryPercentageText);
}

unsigned int getBatteryPercentage() {
    const float batteryMaxVoltage = 4.2;
    const float batteryMinVoltage = 3.73;
    const float batteryAllowedRange = batteryMaxVoltage - batteryMinVoltage;
    const float batteryCurVAboveMin = getInputVoltage() - batteryMinVoltage;
    const int batteryPercentage = (int)(batteryCurVAboveMin / batteryAllowedRange * 100);
    if (batteryPercentage > 150) {
        return USB_POWER;
    }
    return max(min(batteryPercentage, 100), 0);
}

float getInputVoltage() {
    delay(100);
    const uint16_t v1 = analogRead(34);
    return ((float)v1 / 4095.0f) * 2.0f * 3.3f * (1100.0f / 1000.0f);
}

void maybeSleepDevice() {
    if (isSleepEnabled && !isPretendSleeping) {
        long currentTime = millis();
        if (currentTime > (timeOfLastInteraction + sleepTimer * 1000)) {
            sleepAnimation();
            if (isPoweredExternally()) {
                isLilyGoKeyboard();
                Serial.println("Pretend sleep now");
                isPretendSleeping = true;
                tft.fillScreen(TFT_BLACK);
            } else {
                if (isLilyGoKeyboard()) {
                    esp_sleep_enable_ext0_wakeup(GPIO_NUM_32, 1);
                } else {
                    touchAttachInterrupt(T3, callback, 40);
                    esp_sleep_enable_touchpad_wakeup();
                }
                Serial.println("Going to sleep now");
                esp_deep_sleep_start();
            }
        }
    }
}

void sleepAnimation() {
    printSleepAnimationFrame("(o.o)", 500);
    printSleepAnimationFrame("(-.-)", 500);
    printSleepAnimationFrame("(-.-)z", 250);
    printSleepAnimationFrame("(-.-)zz", 250);
    printSleepAnimationFrame("(-.-)zzz", 250);
    tft.fillScreen(TFT_BLACK);
}

void wakeAnimation() {
    printSleepAnimationFrame("(-.-)", 100);
    printSleepAnimationFrame("(o.o)", 200);
    tft.fillScreen(TFT_BLACK);
}

void printSleepAnimationFrame(String text, int wait) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(5, 80);
    tft.setTextSize(4);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println(text);
    delay(wait);
}

boolean isLilyGoKeyboard() {
    if (colPins[0] == 33) {
        return true;
    }
    return false;
}

bool isPoweredExternally() {
    Serial.println("Is powered externally?");
    float inputVoltage = getInputVoltage();
    if (inputVoltage > 4.5) {
        return true;
    } else {
        return false;
    }
}

void callback() {}
