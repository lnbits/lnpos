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
    name: "config_ssid",
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
