String makeLNURL(float amount, int randomPin)
{
  int multipler = pow(10, 2);
  if (currencyPoS == "sat")
  {
    multipler = 1;
  }

  float total = amount * multipler;

  String secret;
  char hexbuffer[3];

  if (selection == "Offline PoS")
  {
    preparedURL = baseURLPoS;
    secret = secretPoS;
  }
  else
  {
    // ATM
    preparedURL = baseURLATM;
    secret = secretATM;
  }

  int salt_length = 8;
  unsigned char salt[salt_length];

  for (int i = 0; i < salt_length; i++)
  {
    salt[i] = random(0, 256);
  }

  unsigned char keyIV[32 + 16] = {0};
  deriveKeyAndIV(secret.c_str(), salt, keyIV);

  unsigned char key[32] = {0};
  unsigned char iv[16] = {0};

  memcpy(key, keyIV, 32);
  memcpy(iv, keyIV + 32, 16);

  String payload = String(randomPin) + String(":") + String(total);

  Serial.print("payload: ");
  Serial.println(payload);

  size_t payload_len = payload.length();
  int padding = 16 - (payload_len % 16);
  payload_len += padding;
  for (int i = 0; i < padding; i++)
  {
    payload += String((char)padding);
  }

  unsigned char encrypted[payload_len] = {0};
  encrypt(key, iv, payload_len, payload.c_str(), encrypted);

  const unsigned char *saltedChars = (const unsigned char *)"Salted__";
  unsigned char salted[payload_len + 16];
  memcpy(salted, saltedChars, 8);
  memcpy(salted + 8, salt, salt_length);
  memcpy(salted + 16, encrypted, payload_len);

  preparedURL += "?p=";
  preparedURL += toBase64(salted, payload_len + 16, BASE64_URLSAFE);
  Serial.println(preparedURL);

  char Buf[200];
  preparedURL.toCharArray(Buf, 200);
  char *url = Buf;
  byte *data = (byte *)calloc(strlen(url) * 2, sizeof(byte));
  size_t len = 0;
  int res = convert_bits(data, &len, 5, (byte *)url, strlen(url), 8, 1);
  char *charLnurl = (char *)calloc(strlen(url) * 2, sizeof(byte));
  bech32_encode(charLnurl, "lnurl", data, len);
  to_upper(charLnurl);
  qrData = charLnurl;
  Serial.println(qrData);

  return charLnurl;
}

