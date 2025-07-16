#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Config and file management
void configOverSerialPort();
void executeConfig();
void executeCommand(String commandName, String commandData);
void removeFile(String path);
void appendToFile(String path, String data);
void readFile(String path);
KeyValue extractKeyValue(String s);
void readFiles();

#endif // CONFIG_H