#ifdef TFT
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
void setupTFT() {
  tft.init();
  Serial.println("TFT: " + String(TFT_WIDTH) + "x" + String(TFT_HEIGHT));
  Serial.println("TFT pin MISO: " + String(TFT_MISO));
  Serial.println("TFT pin CS: " + String(TFT_CS));
  Serial.println("TFT pin MOSI: " + String(TFT_MOSI));
  Serial.println("TFT pin SCLK: " + String(TFT_SCLK));
  Serial.println("TFT pin DC: " + String(TFT_DC));
  Serial.println("TFT pin RST: " + String(TFT_RST));
  Serial.println("TFT pin BL: " + String(TFT_BL));
  tft.setRotation(1);
  tft.invertDisplay(true);
}
void _printTFT(String message, int x, int y, int size = 2) {
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.println(message);
}
void printTFT(String message, int x, int y, int size = 2) {
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  _printTFT(message, x, y, size);
}
void printGreenTFT(String message, int x, int y, int size = 2) {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  _printTFT(message, x, y, size);
}
void printRedTFT(String message, int x, int y, int size = 2) {
  tft.setTextColor(TFT_RED, TFT_BLACK);
  _printTFT(message, x, y, size);
}
void printPurpleTFT(String message, int x, int y, int size = 2) {
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  _printTFT(message, x, y, size);
}
void printOrangeTFT(String message, int x, int y, int size = 2) {
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  _printTFT(message, x, y, size);
}
void clearTFT() {
  tft.fillScreen(TFT_BLACK);
}
void printQrCode(String qrData, String title = "") {
  clearTFT();
}
void error(String message) {
  error(message, "", "");
}
void error(String message, String additional, String additional2) {
  clearTFT();
  printRedTFT(message, 0, 30, 3);
  if (additional != "")
  {
    printTFT(additional, 0, 100);
  }
  if (additional2 != "")
  {
    printTFT(additional2, 0, 120);
  }
}
void processing(String message) {
  clearTFT();
  printTFT(message, 20, 60, 2);
}
#else
void printTFT(String message, int x, int y, int size = 2) {}
void printGreenTFT(String message, int x, int y, int size = 2) {}
void printRedTFT(String message, int x, int y, int size = 2) {}
void printPurpleTFT(String message, int x, int y, int size = 2) {}
void clearTFT() {}
void setupTFT() {
  Serial.println("TFT not configured.");
}
void configLaunch(String title) {}
void error(String message) {}
void error(String message, String additional, String additional2) {}
void processing(String message) {}
void printQrCode(String qrData, String title = "") {}
void printOrangeTFT(String message, int x, int y, int size = 2) {
  Serial.println("TFT not configured for printOrangeTFT.");
#endif
