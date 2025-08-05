/**
 * Awww. Show the go to sleep animation
 */
void sleepAnimation()
{
  printSleepAnimationFrame("(o.o)", 500);
  printSleepAnimationFrame("(-.-)", 500);
  printSleepAnimationFrame("(-.-)z", 250);
  printSleepAnimationFrame("(-.-)zz", 250);
  printSleepAnimationFrame("(-.-)zzz", 250);
  clearTFT();
}

void wakeAnimation()
{
  printSleepAnimationFrame("(-.-)", 100);
  printSleepAnimationFrame("(o.o)", 200);
  clearTFT();
}

/**
   Print the line of the animation
*/
void printSleepAnimationFrame(String text, int wait)
{
  clearTFT();
  printTFT(text, 5, 80, 4);
  delay(wait);
}
