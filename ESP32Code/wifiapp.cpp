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
#include <BluetoothSerial.h>

#include "globalvar.h"
#include "wifiapp.h"



void wifiScan()
{
  WiFi.mode(WIFI_STA); //设置为STA模式
  WiFi.disconnect();   //断开当前可能的连接
  delay(100);

  SerialBT.println("scan start");
  int n = WiFi.scanNetworks(); //扫描并返回搜索到的网络数量，该方法默认会阻塞
  SerialBT.println("scan done");
  if (n != 0)
  {
    SerialBT.print(n);
    SerialBT.println(" networks found: ");
    for (int i = 0; i < n; ++i)
    {
      SerialBT.println();
      SerialBT.print(i + 1);
      SerialBT.print(":       ");
      SerialBT.print(WiFi.SSID(i)); //打印网络名称
      SerialBT.print("        ");
      SerialBT.print(WiFi.RSSI(i)); //打印信号强度
      SerialBT.print("        ");
      SerialBT.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Encrypted"); //打印是否加密
      delay(10);
    }
    SerialBT.println();
  }
}

int checkWiFi1()
{
  if (WiFi.isConnected())
  {
    SerialBT.println("Wi-Fi is connected: ");
    SerialBT.print("SSID: ");
    SerialBT.print("Local IP: ");
    SerialBT.println(WiFi.localIP());
    SerialBT.print("Local IPv6: ");
    SerialBT.println(WiFi.localIPv6());
    SerialBT.print("MAC Address: ");
    SerialBT.println(WiFi.macAddress());
    SerialBT.print("Network ID: ");
    SerialBT.println(WiFi.networkID());
    SerialBT.print("Transmission Gain: ");
    SerialBT.println(WiFi.getTxPower());
    SerialBT.print("Host Name: ");
    SerialBT.println(WiFi.getHostname());
    SerialBT.print("Gateway IP: ");
    SerialBT.println(WiFi.gatewayIP());
    SerialBT.print("DNS IP: ");
    SerialBT.println(WiFi.dnsIP());
    SerialBT.print("Status: ");
    SerialBT.println(WiFi.status());
    return 1;
  }
  else SerialBT.println("Wi-Fi is not connected.");
  return 0;
}

int connWiFi0()
{
  int i = 0;
  SerialBT.print("Connecting");
  for (i = 0; i < 20; i++)
  {
    if (WiFi.isConnected())
    {
      SerialBT.println();
      SerialBT.println("Wi-Fi Connection Succeed!");
      return 1;
    }
    SerialBT.print(".");
    delay(500);
  }
  SerialBT.println();
  SerialBT.println("Wi-Fi Connection Failed.");
  return 0;
}

int connWiFi1()
{
  WiFi.mode(WIFI_STA); //设置为STA模式
  WiFi.disconnect();   //断开当前可能的连接
  WiFi.begin(wifiSSID);
  return connWiFi0();
}

int connWiFi2()
{
  WiFi.mode(WIFI_STA); //设置为STA模式
  WiFi.disconnect();   //断开当前可能的连接
  WiFi.begin(wifiSSID, wifiPswd);
  return connWiFi0();
}

int disconnWiFi0()
{
  WiFi.disconnect();
  SerialBT.print("Disconnecting");
  int i = 0;
  for (i = 0; i < 20; i++)
  {
    if (!WiFi.isConnected())
    {
      SerialBT.println();
      SerialBT.println("Wi-Fi Disonnection Succeed!");
      return 1;
    }
    SerialBT.print(".");
    delay(500);
  }
  SerialBT.println();
  SerialBT.println("Wi-Fi Disonnection Failed.");
  return 0;
}
