#include "atcommand.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Ds1302.h>

#include "globalvar.h"

int getATCommand()
{
  char cmdBuff[100] = "";
  char cmdBuff1[100] = "";
  unsigned char i = 0, n = 0;

  cmdBuff[0] = SerialBT.read();
  if (cmdBuff[0] == 'A')
  {
    if (!SerialBT.available())return -1;
    cmdBuff[1] = SerialBT.read();
    if (cmdBuff[1] == 'T')
    {
      if (!SerialBT.available())return -1;
      cmdBuff[2] = SerialBT.read();
      if (cmdBuff[2] == '+')
      {
        for (i = 3; i < 100; i++)
        {
          if (!SerialBT.available())break;
          cmdBuff[i] = SerialBT.read();
        }
        //Command without parameters
        if (!strcmp(cmdBuff + 3, "TIME?"))
        {
          return readTime;
        }
        else if (!strcmp(cmdBuff + 3, "CWLAP"))
        {
          return scanWifiAP;
        }
        else if (!strcmp(cmdBuff + 3, "CJWAP?"))
        {
          return checkWiFi;
        }
        else if (!strcmp(cmdBuff + 3, "CWQAP"))
        {
          return disconnectWiFi;
        }
        //3Byte Command with parameters
        //4Byte Command with parameters
        strncpy(cmdBuff1, cmdBuff + 3, 5);
        if (!strcmp(cmdBuff1, "TIME "))
        {
          n = sscanf(cmdBuff + 8, "20%d/%d/%d,%d:%d,%d", &dt.year, &dt.month, &dt.day, &dt.hour, &dt.minute, &dt.dow);
          Serial.println(n);
          if (n < 6)return notCorrectAT;
          else
          {
            dt.second = 0;
            return setTime;
          }
        }
        //5Byte Command with parameters
        strncpy(cmdBuff1, cmdBuff + 3, 6);
        if (!strcmp(cmdBuff1, "CJWAP "))
        {
          n = sscanf(cmdBuff + 9, "%s%s", wifiSSID,wifiPswd);
          if (n == 0)return notCorrectAT;
          else if(n == 1)return connWiFi;
          else if(n == 2)return connWiFiWithPswd;
          else return notCorrectAT;
        }
        else return notCorrectAT;
      }
    }
  }
  else return -1;
  return 0;
}
