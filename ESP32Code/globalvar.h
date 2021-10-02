#ifndef _GLOBALVAR_H_
#define _GLOBALVAR_H_

#include <ETH.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <Arduino.h>
#include <SPI.h>
#include <BluetoothSerial.h>
#include <Ds1302.h>

extern BluetoothSerial SerialBT;
extern Ds1302::DateTime dt;

extern char wifiSSID[20];
extern char wifiPswd[20];

#endif
