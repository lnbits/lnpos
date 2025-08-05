#include <Keypad.h>
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[rows] = {21, 27, 26, 22}; // connect to the row pinouts of the keypad
byte colPins[cols] = {33, 32, 25};     // connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

char getKeypadInput()
{
  char key = keypad.getKey();
  if (key == NO_KEY)
  {
    return NO_KEY;
  }
  isPretendSleeping = false;
  timeOfLastInteraction = millis();
  return key;
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

// void getKeypad(bool isATMPin, bool justKey, bool isLN, bool isATMNum)
// {
//   const char key = keypad.getKey();

//   if (key == NO_KEY)
//   {
//     return;
//   }

//   isPretendSleeping = false;

//   key_val = String(key);

//   if (key_val != "")
//   {
//     timeOfLastInteraction = millis();
//   }

//   if (dataIn.length() < 9)
//   {
//     dataIn += key_val;
//   }

//   if (isLN)
//   {
//     isLNMoneyNumber(false);
//   }
//   else if (isATMPin)
//   {
//     isATMMoneyPin(false);
//   }
//   else if (justKey)
//   {
//   }
//   else if (isATMNum)
//   {
//     isATMMoneyNumber(false);
//   }
//   else
//   {
//     isLNURLMoneyNumber(false);
//   }
// }
