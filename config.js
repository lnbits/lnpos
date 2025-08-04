export const addressesAndFiles = [
  {
    address: "0x1000",
    fileName: "lnpos.ino.bootloader.bin",
  },
  {
    address: "0x8000",
    fileName: "lnpos.ino.partitions.bin",
  },
  {
    address: "0xE000",
    fileName: "boot_app0.bin",
  },
  {
    address: "0x10000",
    fileName: "lnpos.ino.bin",
  },
];

export const configPath = "elements.json";
export const elements = [
  {
    type: "heading",
    label: "Offline PoS",
  },
  {
    name: "config_offlinepos",
    value: "",
    label: "String from the LNPoS extension",
    type: "text",
  },
  {
    type: "heading",
    label: "Offline ATM",
  },
  {
    name: "config_offlineatm",
    value: "",
    label: "String from the FOSSA extension",
    type: "text",
  },
  {
    type: "heading",
    label: "Onchain addresses",
  },
  {
    name: "config_masterkey",
    value: "",
    label: "Onchain Extended Public Key",
    type: "text",
  },
  {
    name: "config_mempool",
    value: "https://mempool.space",
    label: "Mempool.space Server, for creating a checking link",
    type: "text",
  },
  {
    type: "heading",
    label: "Online PoS",
  },
  {
    name: "config_lncurrency",
    value: "USD",
    label: "PoS Currency. ie: GBP, EUR",
    type: "text",
  },
  {
    name: "config_server",
    value: "https://demo.lnbits.com",
    label: "LNbits Server",
    type: "text",
  },
  {
    name: "config_invoice",
    value: "",
    label: "Wallet Invoice Key",
    type: "text",
  },
  {
    name: "config_wifi_ssid",
    value: "",
    label: "WiFi SSID",
    type: "text",
  },
  {
    name: "config_wifi_password",
    value: "",
    label: "WiFi Password",
    type: "text",
  },
  {
    type: "heading",
    label: "Additional settings",
  },
  {
    name: "config_securitypin",
    value: "000000",
    label: "Security pin for settings and ATM mode, 4-6 characters",
    type: "text",
  },
  {
    name: "config_fiatdecimalplaces",
    value: 2,
    label: "FIAT Decimal Places ie 2 for USD, 0 for YEN",
    type: "text",
  },
];
