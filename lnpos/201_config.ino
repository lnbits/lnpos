#ifdef HARDCODED
void setupConfig(){
    Serial.println("Setting hardcoded values...");
    config_ssid = CONFIG_SSID;
    Serial.println("SSID: " + config_ssid);
    config_password = CONFIG_PASSWORD;
    Serial.println("SSID password: " + config_password);
    config_offline_pos = CONFIG_OFFLINE_POS;
    Serial.println("Offline POS URL: " + config_offline_pos);
    config_offline_atm = CONFIG_OFFLINE_ATM;
    Serial.println("Offline ATM URL: " + config_offline_atm);
    config_master_key = CONFIG_MASTER_KEY;
    Serial.println("Master key: " + config_master_key);
    config_mempool = CONFIG_MEMPOOL;
    Serial.println("Mempool URL: " + config_mempool);
    config_lnbits_server = CONFIG_LNBITS_SERVER;
    Serial.println("LNbits server: " + config_lnbits_server);
    config_invoice = CONFIG_INVOICE;
    Serial.println("Invoice: " + config_invoice);
    config_ln_currency = CONFIG_LN_CURRENCY;
    Serial.println("LN currency: " + config_ln_currency);
    config_security_pin = CONFIG_SECURITY_PIN;
    Serial.println("Security PIN: " + config_security_pin);
    config_fiat_decimal_places = CONFIG_FIAT_DECIMAL_PLACES;
    Serial.println("Fiat decimal places: " + config_fiat_decimal_places);
    config_qr_screen_brightness = CONFIG_QR_SCREEN_BRIGHTNESS;
    Serial.println("QR screen brightness: " + String(config_qr_screen_brightness));
}
#else
#include <FS.h>
#include <SPIFFS.h>
void setupConfig(){
    SPIFFS.begin(true);
    // first give the installer a chance to delete configuration file
    executeConfigBoot();
    String fileContent = readConfig();
    // file does not exist, so we will enter endless config mode
    if (fileContent == "") {
        Serial.println("Config file does not exist.");
        executeConfigForever();
    }
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, fileContent);
    if(error){
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
    }

    config_ssid = getJsonValue(doc, "config_ssid", CONFIG_SSID);
    config_password = getJsonValue(doc, "config_password", CONFIG_PASSWORD);
    config_offline_pos = getJsonValue(doc, "config_offline_pos", CONFIG_OFFLINE_POS);
    config_offline_atm = getJsonValue(doc, "config_offline_atm", CONFIG_OFFLINE_ATM);
    config_master_key = getJsonValue(doc, "config_master_key", CONFIG_MASTER_KEY);
    config_mempool = getJsonValue(doc, "config_mempool", CONFIG_MEMPOOL);
    config_lnbits_server = getJsonValue(doc, "config_lnbits_server", CONFIG_LNBITS_SERVER);
    config_invoice = getJsonValue(doc, "config_invoice", CONFIG_INVOICE);
    config_ln_currency = getJsonValue(doc, "config_ln_currency", CONFIG_LN_CURRENCY);
    config_security_pin = getJsonValue(doc, "config_security_pin", CONFIG_SECURITY_PIN);
    config_fiat_decimal_places = getJsonValue(doc, "config_fiat_decimal_places", CONFIG_FIAT_DECIMAL_PLACES);
    config_qr_screen_brightness = getJsonValue(doc, "config_qr_screen_brightness", "121");
}

String readConfig() {
  fs::File paramFile = SPIFFS.open(CONFIG_FILE, FILE_READ);
    if (!paramFile) {
        return "";
    }
    String fileContent = paramFile.readString();
    if (fileContent == "") {
        return "";
    }
    paramFile.close();
    return fileContent;
}

String getJsonValue(JsonDocument &doc, const char* name, String defaultValue)
{
    String value = defaultValue;
    for (JsonObject elem : doc.as<JsonArray>()) {
        if (strcmp(elem["name"], name) == 0) {
            value = elem["value"].as<String>();
            Serial.println(String(name) + ": " + value);
            return value;
        }
    }
    Serial.println(String(name) + " (using default): " + value);
    return defaultValue;
}

void executeConfigBoot() {
    Serial.println("Entering boot mode. Waiting for " + String(BOOTUP_TIMEOUT) + " seconds.");
    clearTFT();
    printTFT("BOOT MODE", 21, 21);
    int counter = BOOTUP_TIMEOUT + 1;
    while (counter-- > 0) {
        if (Serial.available() == 0) {
            delay(1000);
            continue;
        }
        Serial.println();
        // if we get serial data in the first 5 seconds, we will enter config mode
        counter = 0;
        executeConfigForever();
    }
    Serial.println("Exiting boot mode.");
    Serial.print("Welcome to the LNbits " + String(NAME) + "!");
    Serial.println(" (" + String(VERSION) + ")");
    clearTFT();
    printTFT(String(NAME), 21, 21);
    printTFT(String(VERSION), 21, 42);
}

void executeConfigForever() {
    Serial.println("Entering config mode. until we receive /config-done.");
    clearTFT();
    printTFT("CONFIG", 21, 21);
    bool done = false;
    while (true) {
        done = executeConfig();
        if (done) {
            Serial.println("Exiting config mode.");
            return;
        }
    }
}

bool executeConfig() {
  if (Serial.available() == 0) return false;
  String data = Serial.readStringUntil('\n');
  Serial.println("received serial data: " + data);
  if (data == "/config-done") {
    delay(1000);
    return true;
  }
  if (data == "/file-remove") {
    SPIFFS.remove(CONFIG_FILE);
  }
  if (data.startsWith("/file-append")) {
    fs::File file = SPIFFS.open(CONFIG_FILE, FILE_APPEND);
    if (!file) {
      file = SPIFFS.open(CONFIG_FILE, FILE_WRITE);
    }
    if (!file) {
      Serial.println("Failed to open file for writing.");
    }
    if (file) {
      int pos = data.indexOf(" ");
      String jsondata = data.substring(pos + 1);
      file.println(jsondata);
      file.close();
    }
  }
  if (data.startsWith("/file-read")) {
    fs::File file = SPIFFS.open(CONFIG_FILE, "r");
    if (file) {
      while (file.available()) {
        String line = file.readStringUntil('\n');
        Serial.println("/file-send " + line);
      }
      file.close();
      Serial.println("/file-done");
    }
    return false;
  }
  return false;
}
#endif
