#ifndef SCREENS_H
#define SCREENS_H

#include "defines.h"
#include "helpers.h"
#include "ui.h"

// on-chain payment method
void onchainMain()
{
  File file = SPIFFS.open(KEY_FILE);
  if (file)
  {
    addressNo = file.readString();
    addressNo = String(addressNo.toInt() + 1);
    file.close();
    file = SPIFFS.open(KEY_FILE, FILE_WRITE);
    file.print(addressNo);
    file.close();
  }
  else
  {
    file.close();
    file = SPIFFS.open(KEY_FILE, FILE_WRITE);
    addressNo = "1";
    file.print(addressNo);
    file.close();
  }

  Serial.println(addressNo);
  inputScreenOnChain();

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, true, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      HDPublicKey hd(masterKey);
      qrData = hd.derive(String("m/0/") + addressNo).address();
      qrShowCodeOnchain(true, " *MENU #CHECK");

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, true, false, false);

        if (key_val == "*")
        {
          unConfirmed = false;
        }
        else if (key_val == "#")
        {
          while (unConfirmed)
          {
            qrData = "https://" + mempool + "/address/" + qrData;
            qrShowCodeOnchain(false, " *MENU");

            while (unConfirmed)
            {
              key_val = "";
              getKeypad(false, true, false, false);

              if (key_val == "*")
              {
                unConfirmed = false;
              }
            }
          }
        }
        handleBrightnessAdjust(key_val, ONCHAIN);
      }
    }
  }
}

void lnMain()
{
  readFiles();

  if (!checkOnlineParams())
  {
    return;
  }

  if (lnCurrency == "" || lnCurrency == "default")
  {
    currencyLoop();
  }

  processing("FETCHING FIAT RATE");
  if (!getSats())
  {
    error("FETCHING FIAT RATE FAILED");
    delay(3000);
    return;
  }

  isLNMoneyNumber(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, false, true, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      if (noSats.toInt() == 0)
      {
        error("ZERO SATS");
        delay(3000);
        isLNMoneyNumber(true);
        continue;
      }

      // request invoice
      processing("FETCHING INVOICE");
      if (!getInvoice())
      {
        unConfirmed = false;
        error("ERROR FETCHING INVOICE");
        delay(3000);
        break;
      }

      // show QR
      qrShowCodeln();

      // check invoice
      bool isFirstRun = true;
      while (unConfirmed)
      {
        int timer = 0;

        if (!isFirstRun)
        {
          unConfirmed = checkInvoice();
          if (!unConfirmed)
          {
            paymentSuccess();
            timer = 5000;

            while (key_val != "*")
            {
              key_val = "";
              getKeypad(false, true, false, false);

              if (key_val != "*")
              {
                delay(100);
              }
            }
          }
        }

        // abort on * press
        while (timer < (isFirstRun ? 6000 : 2000))
        {
          getKeypad(false, true, false, false);

          if (key_val == "*")
          {
            noSats = "0";
            dataIn = "0";
            formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
            amountToShow = fiatDecimalPlacesOutput;
            unConfirmed = false;
            timer = 5000;
            break;
          }
          else
          {
            delay(100);
            handleBrightnessAdjust(key_val, LNPOS);
            key_val = "";
          }
          timer = timer + 100;
        }

        isFirstRun = false;
      }

      noSats = "0";
      dataIn = "0";
      formatNumber(0, fiatDecimalPlaces.toInt(), fiatDecimalPlacesOutput);
      amountToShow = fiatDecimalPlacesOutput;
    }
    else
    {
      delay(100);
    }
  }
}

void offlinePoSMain()
{
  inputs = "";
  pinToShow = "";
  dataIn = "";

  if (!checkOfflineParams())
  {
    return;
  }

  isLNURLMoneyNumber(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(false, false, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      if (!makeLNURL())
      {
        isLNURLMoneyNumber(true);
        continue;
      }
      qrShowCodeLNURL(" *MENU #SHOW PIN");

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, true, false, false);

        if (key_val == "#")
        {
          showPin();

          while (unConfirmed)
          {
            key_val = "";
            getKeypad(false, true, false, false);

            if (key_val == "*")
            {
              unConfirmed = false;
            }
          }
        }
        else if (key_val == "*")
        {
          unConfirmed = false;
        }
        handleBrightnessAdjust(key_val, offlinePoS);
      }
    }
    else
    {
      delay(100);
    }
  }
}

void offlineATMMain()
{
  pinToShow = "";
  dataIn = "";
  isATMMoneyPin(true);

  while (unConfirmed)
  {
    key_val = "";
    getKeypad(true, false, false, false);

    if (key_val == "*")
    {
      unConfirmed = false;
    }
    else if (key_val == "#")
    {
      isATMMoneyPin(true);
    }

    if (pinToShow.length() == securityPin.length() && pinToShow != securityPin)
    {
      error("  WRONG PIN");
      delay(1500);

      pinToShow = "";
      dataIn = "";
      isATMMoneyPin(true);
    }
    else if (pinToShow == securityPin)
    {
      isATMMoneyNumber(true);
      inputs = "";
      dataIn = "";

      while (unConfirmed)
      {
        key_val = "";
        getKeypad(false, false, false, true);

        if (key_val == "*")
        {
          unConfirmed = false;
        }
        else if (key_val == "#")
        {
          if (!makeLNURL())
          {
            isATMMoneyNumber(true);
            continue;
          }
          qrShowCodeLNURL(" *MENU");

          while (unConfirmed)
          {
            key_val = "";
            getKeypad(false, true, false, false);
            handleBrightnessAdjust(key_val, offlineATM);

            if (key_val == "*")
            {
              unConfirmed = false;
            }
          }
        }
      }
    }
    else
    {
      delay(100);
    }
  }
}

#endif