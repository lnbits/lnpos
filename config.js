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
    name: "config_lnurlpos",
    value: "",
    label: "LNURLPoS string from LNURLDevices extension",
    type: "text",
  },
  {
    name: "config_lnurlatm",
    value: "",
    label: "LNURL ATM",
    type: "text",
  },
  {
    name: "config_password",
    value: "",
    label: "Device Password",
    type: "text",
  },
  {
    name: "config_masterkey",
    value: "",
    label: "Onchain Master Public Key",
    type: "text",
  },
  {
    name: "config_server",
    value: "",
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
    name: "config_lncurrency",
    value: "",
    label: "PoS Currency. ie: EUR",
    type: "text",
  },
  {
    name: "config_lnurlatmms",
    value: "",
    label: "Mempool.space Server",
    type: "text",
  },
  {
    name: "config_lnurlatmpin",
    value: "",
    label: "LNURLATM pin String",
    type: "text",
  },
  {
    name: "config_decimalplaces",
    value: "",
    label: "FIAT Decimal Places",
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
];