#ifndef CONFIG_H
#define CONFIG_H

void configOverSerialPort()
{
  serialLaunch();
  executeConfig();
}

void executeConfig()
{

  while (true)
  {
    key_val = "";
    getKeypad(false, true, false, false);
    if (key_val == "*")
    {
      readFiles();
      unConfirmed = false;
      return;
    }
    if (Serial.available() == 0)
      continue;
    String data = Serial.readStringUntil('\n');
    Serial.println("received: " + data);
    KeyValue kv = extractKeyValue(data);
    String commandName = kv.key;
    if (commandName == "/config-done")
    {
      Serial.println("/config-done");
      return;
    }
    executeCommand(commandName, kv.value);
  }
}

void executeCommand(String commandName, String commandData)
{
  Serial.println("executeCommand: " + commandName + " > " + commandData);
  KeyValue kv = extractKeyValue(commandData);
  String path = kv.key;
  String data = kv.value;

  if (commandName == "/file-remove")
  {
    return removeFile(path);
  }
  if (commandName == "/file-append")
  {
    return appendToFile(path, data);
  }

  if (commandName == "/file-read")
  {
    Serial.println("prepare to read");
    readFile(path);
    Serial.println("readFile done");
    return;
  }

  Serial.println("command unknown");
}

void removeFile(String path)
{
  Serial.println("removeFile: " + path);
  SPIFFS.remove("/" + path);
}

void appendToFile(String path, String data)
{
  Serial.println("appendToFile: " + path);
  File file = SPIFFS.open("/" + path, FILE_APPEND);
  if (!file)
  {
    file = SPIFFS.open("/" + path, FILE_WRITE);
  }
  if (file)
  {
    file.println(data);
    file.close();
  }
}

void readFile(String path)
{
  Serial.println("readFile: " + path);
  File file = SPIFFS.open("/" + path);
  if (file)
  {
    while (file.available())
    {
      String line = file.readStringUntil('\n');
      Serial.println("/file-read " + line);
    }
    file.close();
  }
  Serial.println("");
  Serial.println("/file-done");
}

KeyValue extractKeyValue(String s)
{
  int spacePos = s.indexOf(" ");
  String key = s.substring(0, spacePos);
  if (spacePos == -1)
  {
    return {key, ""};
  }
  String value = s.substring(spacePos + 1, s.length());
  return {key, value};
}

void readFiles()
{
  File paramFile = FlashFS.open(PARAM_FILE, "r");
  if (paramFile)
  {
    if (!hardcoded)
    {
      StaticJsonDocument<6000> doc;
      DeserializationError error = deserializeJson(doc, paramFile.readString());
      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      offlinePoS = getJsonValue(doc, "config_offlinePoS");
      offlineATM = getJsonValue(doc, "config_offlineATM");
      masterKey = getJsonValue(doc, "config_masterkey");
      lnbitsServer = getJsonValue(doc, "config_server");
      invoice = getJsonValue(doc, "config_invoice");
      lnCurrency = getJsonValue(doc, "config_lnCurrency");
      mempool = getJsonValue(doc, "config_mempool");
      securityPin = getJsonValue(doc, "config_securityPin");
      fiatDecimalPlaces = getJsonValue(doc, "config_decimalplaces");
      ssid = getJsonValue(doc, "config_wifi_ssid");
      password = getJsonValue(doc, "config_wifi_password");
    }

    ////////LNURL PoS string/////////
    if (offlinePoS != "null" || offlinePoS != "")
    {
      baseURLPoS = getValue(offlinePoS, ',', 0);
      secretPoS = getValue(offlinePoS, ',', 1);
      currencyPoS = getValue(offlinePoS, ',', 2);
      Serial.println("");
      Serial.println("offlinePoS: " + offlinePoS);
      Serial.println("baseURLPoS: " + baseURLPoS);
      Serial.println("secretPoS: " + secretPoS);
      Serial.println("currencyPoS: " + currencyPoS);
      if (secretPoS != "")
      {
        menuItemCheck[1] = 1;
      }
    }
    else
    {
      Serial.println("offlinePoS not set");
    }

    ////////LNURL ATM string/////////
    if (offlineATM != "null" || offlineATM != "")
    {
      Serial.println("");
      Serial.println("offlineATM: " + offlineATM);
      baseURLATM = getValue(offlineATM, ',', 0);
      // remove /api/v1.... and add /atm?lightning=
      int apiPos = baseURLATM.indexOf("api");
      baseUrlAtmPage = baseURLATM.substring(0, apiPos);
      baseUrlAtmPage += "atm?lightning=";
      secretATM = getValue(offlineATM, ',', 1);
      currencyATM = getValue(offlineATM, ',', 2);
      Serial.println("baseUrlAtmPage: " + baseUrlAtmPage);
      Serial.println("baseURLATM: " + baseURLATM);
      Serial.println("secretATM: " + secretATM);
      Serial.println("currencyATM: " + currencyATM);
      if (secretATM != "")
      {
        menuItemCheck[3] = 1;
      }
    }
    else
    {
      Serial.println("baseURLATM not set");
    }

    //////////MasterKey/////////
    if (masterKey != "null" || masterKey != "")
    {
      Serial.println("");
      Serial.println("masterKey used from memory");
      Serial.println("masterKey: " + masterKey);
      if (masterKey != "")
      {
        menuItemCheck[2] = 1;
      }
    }
    else
    {
      Serial.println("MasterKey not set");
    }

    //////////Lnbits Server/////////
    if (lnbitsServer != "null" || lnbitsServer != "")
    {
      Serial.println("");
      Serial.println("lnbitsServer used from memory");
      Serial.println("lnbitsServer: " + lnbitsServer);
    }
    else
    {
      Serial.println("lnbitsServer not set");
    }

    /////////LNbits Server///////
    if (invoice != "null" || invoice != "")
    {
      Serial.println("");
      Serial.println("invoice key used from memory");
      Serial.println("invoice key: " + invoice);
      if (invoice != "")
      {
        menuItemCheck[0] = 1;
      }
    }
    else
    {
      Serial.println("invoice key not set");
    }

    /////////PoS Currency///////
    if (lnCurrency != "null" || lnCurrency != "")
    {
      Serial.println("");
      Serial.println("PoS currency used from memory");
      Serial.println("PoS currency: " + lnCurrency);
    }
    else
    {
      Serial.println("Pos currency not set");
    }

    /////////mempool.space server///////
    if (mempool != "null" || mempool != "")
    {
      Serial.println("");
      Serial.println("mempool.space server used from memory");
      Serial.println("mempool.space server: " + mempool);
    }
    else
    {
      Serial.println("mempool.space server not set");
    }

    /////////mATM/Settings pin///////
    if (securityPin != "null" || securityPin != "")
    {
      Serial.println("");
      Serial.println("ATM/settings security pin used from memory");
      Serial.println("ATM/settings security pin: " + securityPin);
    }
    else
    {
      securityPin = "878787";
      Serial.println("ATM/Settings security pin not set using default");
    }

    /////////no. FIAT decimal places///////
    if (fiatDecimalPlaces != "null" || fiatDecimalPlaces != "")
    {
      Serial.println("");
      Serial.println("no. fiat decimal places used from memory");
      Serial.println("no. fiat decimal places: " + fiatDecimalPlaces);
    }
    else
    {
      Serial.println("no. fiat decimal places not set");
    }

    /////////WiFi SSID///////
    if (ssid != "null" || ssid != "")
    {
      Serial.println("");
      Serial.println("WiFi SSID used from memory");
      Serial.println("WiFi SSID: " + ssid);
    }
    else
    {
      Serial.println("WiFi SSID not set");
    }

    /////////WiFi password///////
    if (password != "null" || password != "")
    {
      Serial.println("");
      Serial.println("WiFi password used from memory");
      Serial.println("WiFi password: " + password);
    }
    else
    {
      Serial.println("WiFi password not set");
    }
  }
  paramFile.close();
}

#endif