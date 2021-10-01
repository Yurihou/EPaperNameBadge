#include "screen.h"
#include <Arduino.h>
#include <SPI.h>
#include "picdata.h"

// char screenData[4736] = {0};

//SPIClass screen(VSPI);

eInkScreen::eInkScreen():screen(VSPI)
{
  //SCLK = 18, MISO = 19, MOSI = 23, CS = 5
  pinMode(DC, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(SS, OUTPUT);
  pinMode(ISBUSY, INPUT);
  screen.begin();
}

void eInkScreen::sendByte(char dc, char data)
{
  //while (digitalRead(ISBUSY));
  digitalWrite(DC, dc);
  screen.beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
  screen.transfer(data);
  digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
  screen.endTransaction();
}

void eInkScreen::isScreenBusy()
{
  char busy = 0;
  do
  {
    sendByte(CMD, 0x71);
    busy = digitalRead(ISBUSY);
    busy = !(busy & 0x01);
  }
  while (busy);
}

void eInkScreen::screenInit()
{
  //Resolution
  char HRES = 0x80;          //128
  char VRES_byte1 = 0x01;      //296
  char VRES_byte2 = 0x28;

  digitalWrite(RST, LOW);   // Module reset
  delay(100); //At least 10ms
  digitalWrite(RST, HIGH);
  delay(100);

  sendByte(CMD, 0x06);        //boost soft start
  sendByte(DAT, 0x17);  //A
  sendByte(DAT, 0x17);  //B
  sendByte(DAT, 0x17);  //C

  sendByte(CMD, 0x04); //Power on
  //waiting for the electronic paper IC to release the idle signal
  isScreenBusy();
  delay(200);

  sendByte(CMD, 0x00);    //panel setting
  sendByte(DAT, 0x1f);   //LUT from OTP
  sendByte(DAT, 0x0d);   //VCOM to 0V

  sendByte(CMD, 0x61);    //resolution setting
  sendByte(DAT, HRES);
  sendByte(DAT, VRES_byte1);
  sendByte(DAT, VRES_byte2);

  sendByte(CMD, 0X50);    //VCOM AND DATA INTERVAL SETTING
  sendByte(DAT, 0x97);   //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
}

void eInkScreen::screenWrite()
{
  //Display Test
  unsigned int i;
  sendByte(CMD, 0x10);        //Transfer old data
  for (i = 0; i < 4736; i++)
    sendByte(DAT, 0xFF);

  sendByte(CMD, 0x13);        //Transfer new data
  for (i = 0; i < 4736; i++)
    sendByte(DAT, screenData[i]);
}

void eInkScreen::screenRefresh()
{
  sendByte(CMD, 0x12);     //DISPLAY REFRESH
  delay(100);          //!!!The delay here is necessary, 200uS at least!!!
  isScreenBusy();
}

void eInkScreen::screenSleep()
{
  sendByte(CMD, 0X50);  //VCOM AND DATA INTERVAL SETTING
  sendByte(DAT, 0xf7);

  sendByte(CMD, 0X02);   //power off
  isScreenBusy();
  sendByte(CMD, 0X07);   //deep sleep
  sendByte(DAT, 0xA5);
}

void eInkScreen::screenGlobalChange()
{
  screenInit();
  screenWrite();
  screenRefresh();
  screenSleep();
}
