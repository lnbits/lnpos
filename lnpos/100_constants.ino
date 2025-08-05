// do not change the version manually or move the #define it will be replaced during release
#define VERSION "v1.0.0"
#define NAME "LNPoS"

// set to true to wipe the disk
#define FORMAT false

// battery percentage sentinel value to indicate USB power
#define USB_POWER 1000

// time in seconds for configurator to send /delete-file
// so the device boots in configuration mode
#define BOOTUP_TIMEOUT 3
#define CONFIG_FILE "/elements.json"
#define KEY_FILE "/thekey.txt"

// uncomment if you dont want to use the configuration file
//#define HARDCODED

// device specific configuration / defaults
#define CONFIG_SSID "my_wifi_ssid"
#define CONFIG_PASSWORD "my_wifi_password"

#define CONFIG_OFFLINE_POS "https://demo.lnbits.com/lnpos/api/v1/lnurl/WTmei,BzzoY5wbgpym3eMdb9ueXr,USD"
#define CONFIG_OFFLINE_ATM "https://demo.lnbits.com/fossa/api/v1/lnurl/W5xu4,XGg4BJ3xCh36JdMKm2kgDw,USD"

/// for generating onchain addresses
#define CONFIG_MASTER_KEY "xpub6CJFgwcim8tPBJo2A6dS13kZxqbgtWKD3LKj1tyurWADbXbPyWo11exyotTSUY3cvhQy5Mfj8FSURgpXhc4L2UvQyaTMC36S49JnNJMmcWU"
#define CONFIG_MEMPOOL "https://mempool.space"

/// for online pos
#define CONFIG_LNBITS_SERVER "https://demo.lnbits.com"
#define CONFIG_INVOICE ""
#define CONFIG_LN_CURRENCY "GBP"

/// additional settings
#define CONFIG_SECURITY_PIN "878787"  // for settings and atm
#define CONFIG_FIAT_DECIMAL_PLACES "2"  // for our japanese friends
#define CONFIG_QR_SCREEN_BRIGHTNESS "121" // 0 = min, 255 = max
