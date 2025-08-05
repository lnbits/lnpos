bool getSats()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  String lnbitsServer = config_lnbits_server;
  lnbitsServer.toLowerCase();
  if (lnbitsServer.substring(0, 8) == "https://")
  {
    lnbitsServer = lnbitsServer.substring(8, lnbitsServer.length());
  }
  const char *lnbitsServerChar = lnbitsServer.c_str();
  const char *invoiceChar = config_invoice.c_str();
  const char *lncurrencyChar = config_ln_currency.c_str();

  Serial.println("connecting to LNbits server " + lnbitsServer);
  if (!client.connect(lnbitsServerChar, 443))
  {
    Serial.println("failed to connect to LNbits server " + lnbitsServer);
    return false;
  }

  const String toPost = "{\"amount\" : 1, \"from_\" :\"" + String(lncurrencyChar) + "\", \"to\" : \"sat\"}";
  const String url = "/api/v1/conversion";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + String(lnbitsServerChar) + "\r\n" + "User-Agent: ESP32\r\n" + "X-Api-Key: " + String(invoiceChar) + " \r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n" + "Content-Length: " + toPost.length() + "\r\n" + "\r\n" + toPost + "\n");

  // Skip response headers
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
      break;
  }

  // Read entire body
  String payload;
  while (client.available())
  {
    char c = client.read();
    payload += c;
  }

  StaticJsonDocument<3000> doc;
  DeserializationError jsonError = deserializeJson(doc, payload);

  if (jsonError)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(jsonError.f_str());
    return false;
  }

  converted = doc["sats"];
  return true;
}

bool getInvoice()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  String lnbitsServer = config_lnbits_server;
  lnbitsServer.toLowerCase();
  if (lnbitsServer.substring(0, 8) == "https://")
  {
    lnbitsServer = lnbitsServer.substring(8, lnbitsServer.length());
  }
  const char *lnbitsServerChar = lnbitsServer.c_str();
  const char *invoiceChar = config_invoice.c_str();

  if (!client.connect(lnbitsServerChar, 443))
  {
    Serial.println("failed");
    error("SERVER DOWN");
    delay(3000);
    return false;
  }

  const String toPost = "{\"out\": false,\"amount\" : " + String(noSats.toInt()) + ", \"memo\" :\"Online PoS-" + String(random(1, 1000)) + "\"}";
  const String url = "/api/v1/payments";
  client.print(String("POST ") + url + " HTTP/1.1\r\n" + "Host: " + lnbitsServerChar + "\r\n" + "User-Agent: ESP32\r\n" + "X-Api-Key: " + invoiceChar + " \r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n" + "Content-Length: " + toPost.length() + "\r\n" + "\r\n" + toPost + "\n");
  // Skip response headers
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
      break;
  }

  // IMPORTANT, to skip "443\r\n
  client.readStringUntil('\n');

  // Read JSON payload
  String payload;
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r" || line.length() == 0)
      break;
    payload += line + "\n";
  }

  Serial.println(payload);
  StaticJsonDocument<3000> doc;
  DeserializationError jsonError = deserializeJson(doc, payload);
  if (jsonError)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(jsonError.f_str());
    return false;
  }

  const char *payment_hash = doc["checking_id"];
  const char *payment_request = doc["bolt11"];
  qrData = payment_request;
  dataId = payment_hash;

  Serial.println(qrData);
  return true;
}

bool checkInvoice()
{
  WiFiClientSecure client;
  client.setInsecure(); // Some versions of WiFiClientSecure need this

  const char *lnbitsServerChar = config_lnbits_server.c_str();
  const char *invoiceChar = config_invoice.c_str();
  if (!client.connect(lnbitsServerChar, 443))
  {
    error("SERVER DOWN");
    delay(3000);
    return false;
  }
  const String url = "/api/v1/payments/";
  client.print(String("GET ") + url + dataId + " HTTP/1.1\r\n" + "Host: " + lnbitsServerChar + "\r\n" + "User-Agent: ESP32\r\n" + "Content-Type: application/json\r\n" + "Connection: close\r\n\r\n");
  // Skip response headers
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
      break;
  }

  // Read entire body
  String payload;
  while (client.available())
  {
    char c = client.read();
    payload += c;
  }

  StaticJsonDocument<3000> doc;
  DeserializationError jsonError = deserializeJson(doc, payload);
  if (jsonError)
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(jsonError.f_str());
    return false;
  }
  if (doc["paid"])
  {
    unConfirmed = false;
  }

  return unConfirmed;
}

