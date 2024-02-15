String ln

void readFiles()
{
    File paramFile = FlashFS.open(PARAM_FILE, "r");
    if (paramFile)
    {
        StaticJsonDocument<2500> doc;
        DeserializationError error = deserializeJson(doc, paramFile.readString());
        if(error){
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return;
        }
        lnurlPoS = getJsonValue(doc, "config_lnurlpos");
        lnurlATM = getJsonValue(doc, "config_lnurlatm");
        apPassword = getJsonValue(doc, "config_password");
        masterKey = getJsonValue(doc, "config_masterkey");
        lnbitsServer = getJsonValue(doc, "config_server");
        invoice = getJsonValue(doc, "config_invoice");
        lncurrency = getJsonValue(doc, "config_lncurrency");
        lnurlATMMS = getJsonValue(doc, "config_lnurlatmms");
        lnurlATMPin = getJsonValue(doc, "config_lnurlatmpin");
        decimalplaces = getJsonValue(doc, "config_decimalplaces");
        ssid = getJsonValue(doc, "config_wifi_ssid");
        password = getJsonValue(doc, "config_wifi_password");
////////LNURL PoS string/////////
        if(lnurlPoS != "null" || lnurlPoS != ""){
            baseURLPoS = getValue(lnurlPoS, ',', 0);
            secretPoS = getValue(lnurlPoS, ',', 1);
            currencyPoS = getValue(lnurlPoS, ',', 2);
            Serial.println("");
            Serial.println("lnurlPoS used");
            Serial.println("lnurlPoS: " + lnurlPoS);
        }
        else{
            Serial.println("lnurlPoS inactive");
        }
////////LNURL ATM string/////////

        if(lnurlATM == "null"){
            lnurlATM = getJsonValue(doc, "config_lnurlatm");
            baseURLATM = getValue(lnurlATM, ',', 0);
            secretATM = getValue(lnurlATM, ',', 1);
            currencyATM = getValue(lnurlATM, ',', 2);
            Serial.println("");
            Serial.println("lnurlPoS: " + lnurlPoS);
        }
        else{
            Serial.println("");
            Serial.println("lnurlATM hardcoded");
            Serial.println("lnurlATM: " + lnurlATM);
        }
////////PoS password/////////
        if(apPassword == "null"){
            apPassword = getJsonValue(doc, "config_password");
            Serial.println("");
            Serial.println("PoS password used from memory");
            Serial.println("PoS password: " + apPassword);
        }
        else{
            Serial.println("");
            Serial.println("SSID password: " + apPassword);
        }
//////////MasterKey/////////
        if(masterKey == "null"){
            switchStr = getJsonValue(doc, "config_masterkey");
            Serial.println("");
            Serial.println("masterKey used from memory");
            Serial.println("masterKey: " + masterKey);
        }
        else{
            Serial.println("");
            Serial.println("masterKey: " + masterKey);
        }
//////////Lnbits Server/////////
        if(lnbitsServer == "null"){
            lnbitsServer = getJsonValue(doc, "config_server");
            Serial.println("");
            Serial.println("lnbitsServer used from memory");
            Serial.println("lnbitsServer: " + lnbitsServer);
        }
        else{
            Serial.println("");
            Serial.println("lnbitsServer: " + lnbitsServer);
        }
/////////LNbits Server///////
        if(invoice == "null"){
            invoice = getJsonValue(doc, "config_invoice");
            Serial.println("");
            Serial.println("invoice key used from memory");
            Serial.println("invoice key: " + invoice);
        }
        else{
            Serial.println("");
            Serial.println("invoice key hardcoded");
            Serial.println("invoice key: " + invoice);
        }
/////////PoS Currency///////
        if(lncurrency == "null"){
            lncurrency = getJsonValue(doc, "config_lncurrency");
            Serial.println("");
            Serial.println("PoS currency used from memory");
            Serial.println("PoS currency: " + lncurrency);
        }
        else{
            Serial.println("");
            Serial.println("PoS currency hardcoded");
            Serial.println("Pos currency: " + lncurrency);
        }
/////////mempool.space server///////
        if(lnurlATMMS == "null"){
            lnurlATMMS = getJsonValue(doc, "config_lnurlatmms");
            Serial.println("");
            Serial.println("mempool.space server used from memory");
            Serial.println("mempool.space server: " + lnurlATMMS);
        }
        else{
            Serial.println("");
            Serial.println("mempool.space server hardcoded");
            Serial.println("mempool.space server: " + lnurlATMMS);
        }
/////////mempool.space server///////
        if(lnurlATMMS == "null"){
            lnurlATMMS = getJsonValue(doc, "config_lnurlatmms");
            Serial.println("");
            Serial.println("mempool.space server used from memory");
            Serial.println("mempool.space server: " + lnurlATMMS);
        }
        else{
            Serial.println("");
            Serial.println("mempool.space server hardcoded");
            Serial.println("mempool.space server: " + lnurlATMMS);
        }
/////////mATM/Settings pin///////
        if(lnurlATMPin == "null"){
            lnurlATMPin = getJsonValue(doc, "config_lnurlatmpin");
            Serial.println("");
            Serial.println("ATM/settings security pin used from memory");
            Serial.println("ATM/settings security pin: " + lnurlATMPin);
        }
        else{
            Serial.println("");
            Serial.println("ATM/settings security pin hardcoded");
            Serial.println("ATM/settings security pin: " + lnurlATMPin);
        }
/////////no. FIAT decimal places///////
        if(decimalplaces == "null"){
            Serial.println("");
            Serial.println("no. fiat decimal places used from memory");
            Serial.println("no. fiat decimal places: " + decimalplaces);
        }
        else{
            Serial.println("");
            Serial.println("no. fiat decimal places hardcoded");
            Serial.println("no. fiat decimal places: " + decimalplaces);
        }
/////////WiFi SSID///////
        if(ssid == "null"){
            Serial.println("");
            Serial.println("WiFi SSID used from memory");
            Serial.println("WiFi SSID: " + ssid);
        }
        else{
            Serial.println("");
            Serial.println("WiFi SSID hardcoded");
            Serial.println("WiFi SSID: " + ssid);
        }
/////////WiFi password///////
        if(decimalplaces == "null"){
            Serial.println("");
            Serial.println("WiFi password used from memory");
            Serial.println("WiFi password: " + password);
        }
        else{
            Serial.println("");
            Serial.println("WiFi password hardcoded");
            Serial.println("WiFi password: " + password);
        }
    }
    paramFile.close();
}