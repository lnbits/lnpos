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

      lnurlPoS = getJsonValue(doc, "config_lnurlpos");
      lnurlATM = getJsonValue(doc, "config_lnurlatm");
      masterKey = getJsonValue(doc, "config_masterkey");
      lnbitsServer = getJsonValue(doc, "config_server");
      invoice = getJsonValue(doc, "config_invoice");
      lncurrency = getJsonValue(doc, "config_lncurrency");
      lnurlATMMS = getJsonValue(doc, "config_lnurlatmms");
      lnurlATMPin = getJsonValue(doc, "config_lnurlatmpin");
      decimalplaces = getJsonValue(doc, "config_decimalplaces");
      ssid = getJsonValue(doc, "config_wifi_ssid");
      password = getJsonValue(doc, "config_wifi_password");
    }

    ////////LNURL PoS string/////////
    if (lnurlPoS != "null" || lnurlPoS != "")
    {
      baseURLPoS = getValue(lnurlPoS, ',', 0);
      secretPoS = getValue(lnurlPoS, ',', 1);
      currencyPoS = getValue(lnurlPoS, ',', 2);
      Serial.println("");
      Serial.println("lnurlPoS used");
      Serial.println("lnurlPoS: " + lnurlPoS);
      if (secretPoS != "")
      {
        menuItemCheck[1] = 1;
      }
    }
    else
    {
      Serial.println("lnurlPoS not set");
    }

    ////////LNURL ATM string/////////
    if (lnurlATM != "null" || lnurlATM != "")
    {
      baseURLATM = getValue(lnurlATM, ',', 0);
      secretATM = getValue(lnurlATM, ',', 1);
      currencyATM = getValue(lnurlATM, ',', 2);
      Serial.println("");
      Serial.println("lnurlATM: " + lnurlATM);
      if (secretATM != "")
      {
        menuItemCheck[3] = 1;
      }
    }
    else
    {
      Serial.println("lnurlATM not set");
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
    if (lncurrency != "null" || lncurrency != "")
    {
      Serial.println("");
      Serial.println("PoS currency used from memory");
      Serial.println("PoS currency: " + lncurrency);
    }
    else
    {
      Serial.println("Pos currency not set");
    }

    /////////mempool.space server///////
    if (lnurlATMMS != "null" || lnurlATMMS != "")
    {
      Serial.println("");
      Serial.println("mempool.space server used from memory");
      Serial.println("mempool.space server: " + lnurlATMMS);
    }
    else
    {
      Serial.println("mempool.space server not set");
    }

    /////////mATM/Settings pin///////
    if (lnurlATMPin != "null" || lnurlATMPin != "")
    {
      Serial.println("");
      Serial.println("ATM/settings security pin used from memory");
      Serial.println("ATM/settings security pin: " + lnurlATMPin);
    }
    else
    {
      lnurlATMPin = "878787";
      Serial.println("ATM/Settings security pin not set using default");
    }

    /////////no. FIAT decimal places///////
    if (decimalplaces != "null" || decimalplaces != "")
    {
      Serial.println("");
      Serial.println("no. fiat decimal places used from memory");
      Serial.println("no. fiat decimal places: " + decimalplaces);
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
    if (decimalplaces != "null" || decimalplaces != "")
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
