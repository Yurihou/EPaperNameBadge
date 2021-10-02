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

#include <SPI.h>
#include <BluetoothSerial.h>
#include <Ds1302.h>

#include "globalvar.h"
#include "screen.h"
#include "atcommand.h"
#include "wifiapp.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define PIN_ENA 33
#define PIN_CLK 32
#define PIN_DAT 25

eInkScreen screen;
BluetoothSerial SerialBT;
Ds1302 rtc(PIN_ENA, PIN_CLK, PIN_DAT);

Ds1302::DateTime dt = {
  .year = 21,
  .month = 10,
  .day = 1,
  .hour = 20,
  .minute = 0,
  .second = 0,
  .dow = Ds1302::DOW_FRI
};

char wifiSSID[20] = "";
char wifiPswd[20] = "";

#define BluetoothLED 12
#define WiFiLED 14

// extern char screenData[];

void setup() {
  // put your setup code here, to run once:
  int i;
  Serial.begin(115200);
  rtc.init();
  if (rtc.isHalted())
  {
    Serial.println("RTC is halted. Setting time...");
    rtc.setDateTime(&dt);
  }
  pinMode(BluetoothLED, OUTPUT);
  pinMode(WiFiLED, OUTPUT);
  SerialBT.begin("EPapaer_Screen"); //Bluetooth device name
  delay(1);
  screen.screenGlobalChange();
}

void loop() {
  // put your main code here, to run repeatedly:
  int buff = 0;
  if (SerialBT.available())//Deal with AT command
  {
    buff = getATCommand();
    switch (buff)
    {
      case notCorrectCommand:
        SerialBT.printf("This is not a correct command.\n"); break;
      case notCorrectAT:
        SerialBT.printf("This is not a correct AT command.\n"); break;
      case readTime:
        Ds1302::DateTime now;
        rtc.getDateTime(&now);
        SerialBT.printf("Current Time: 20%02d/%02d/%02d %02d:%02d:%02d\n", now.year, now.month, now.day, now.hour, now.minute, now.second);
        break;
      case setTime:
        rtc.setDateTime(&dt);
        rtc.getDateTime(&now);
        SerialBT.printf("Current Time: 20%02d/%02d/%02d %02d:%02d:%02d\n", now.year, now.month, now.day, now.hour, now.minute, now.second);
        break;
      case scanWifiAP:
        wifiScan();
        break;
      case checkWiFi:
        checkWiFi1();
        break;
      case connWiFi:
        connWiFi1();
        break;
      case connWiFiWithPswd:
        connWiFi2();
        break;
      case disconnectWiFi:
        disconnWiFi0();
        break;
    }
  }

  //Detect whether Bluetooth is connected and turn on or off GPIO12
  digitalWrite(BluetoothLED, SerialBT.connected());
  digitalWrite(WiFiLED, WiFi.isConnected());

  delay(50);
}
