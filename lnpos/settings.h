#ifndef SETTINGS_H
#define SETTINGS_H

///////////// SET TO TRUE TO WIPE MEMORY ///////////////
bool format = false;

///////// OPTIONALLY SET HARDCODED SETTINGS ////////////

bool hardcoded = false; /// Set to true to hardcode

/// FOR OFFLINE POS
String offlinePoS = "https://demo.lnbits.com/lnpos/api/v1/lnurl/WTmei,BzzoY5wbgpym3eMdb9ueXr,USD";

/// FOR OFFLINE ATM
String offlineATM = "https://demo.lnbits.com/fossa/api/v1/lnurl/W5xu4,XGg4BJ3xCh36JdMKm2kgDw,USD";

/// FOR GENERATING ONCHAIN ADDRESSES
String masterKey = "xpub6CJFgwcim8tPBJo2A6dS13kZxqbgtWKD3LKj1tyurWADbXbPyWo11exyotTSUY3cvhQy5Mfj8FSURgpXhc4L2UvQyaTMC36S49JnNJMmcWU";
String mempool = "https://mempool.space";

/// FOR ONLINE POS
String lnbitsServer = "https://demo.lnbits.com";
String invoice = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
String lnCurrency = "GBP";
String ssid = "AlansBits";
String password = "ithurtswhenip";

/// ADDITIONAL SETTINGS
String securityPin = "878787";  // FOR SETTINGS AND ATM
String fiatDecimalPlaces = "2"; // FOR OUR JAPANESE FREINDS

#endif