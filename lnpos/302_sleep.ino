bool isSleepEnabled = true;
int sleepTimer = 30; // Time in seconds before the device goes to sleep

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

/**
 * Check whether the device should be put to sleep and put it to sleep
 * if it should
 */
void callback() {}
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
        clearTFT();
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
