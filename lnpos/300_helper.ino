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

float getInputVoltage()
{
  delay(100);
  const uint16_t v1 = analogRead(34);
  return ((float)v1 / 4095.0f) * 2.0f * 3.3f * (1100.0f / 1000.0f);
}

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
