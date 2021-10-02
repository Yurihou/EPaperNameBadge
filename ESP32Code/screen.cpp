#include "screen.h"
#include <Arduino.h>
#include <SPI.h>

#include "globalvar.h"
#include "picdata.h"

// char screenData[4736] = {0};

//SPIClass screen(VSPI);

int floor_division(int i, int j)
{
  int k;
  double i1 = (double)i, j1 = (double)j, k1;
  k1 = i1 / j1;
  k = i / j;
  if (k1 - double(k) == 0)return k;
  else return k + 1;
}

eInkScreen::eInkScreen(): screen(VSPI), partialRefreshTime(0)
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
  unsigned int i, j;
  sendByte(CMD, 0x10);        //Transfer old data
  for (i = 0; i < 4736; i++)
    sendByte(DAT, 0xFF);

  sendByte(CMD, 0x13);        //Transfer new data
  for (i = 0; i < 296; i++)
  {
    for (j = 0; j < 16; j++)
      sendByte(DAT, ~screenData[(296 - i) * 16 + j]);
      //For screen, 0 stands for black and 1 for white, so command reversed color here.
  }
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
  partialRefreshTime = 0;
}

//Because of the position of the screen, x=0 on top and y=0 on left
//Thus, x means vertical axis and y means horizontal.
//What's more, because image stores in whole 8-bit bytes for 1/16 column,
//Partial refresh needs a height of 8 pixel or its multiples.
void eInkScreen::screenPartialChange(int y1, int x1, int y2, int x2)
{
  screenInit();
  if (partialRefreshTime < 5)
  {
    unsigned int datas, i, j;

    sendByte(CMD, 0x91);   //This command makes the display enter partial mode
    sendByte(CMD, 0x90);   //resolution setting
    sendByte(DAT, x1);   //x-start
    sendByte(DAT, x2 - 1); //x-end

    sendByte(DAT, y1 / 256);
    sendByte(DAT, y1 % 256); //y-start

    sendByte(DAT, y2 / 256);
    sendByte(DAT, y2 % 256 - 1); //y-end
    sendByte(DAT, 0x28);

    datas = (x2 - x1) * (y2 - y1) / 8;
    sendByte(CMD, 0x10);        //writes Old data to SRAM for programming
    for (i = 0; i < datas; i++)
      sendByte(DAT, 0x00);

    sendByte(CMD, 0x13);        //writes New data to SRAM.
    for (i = y1; i < y2; i++)
    {
      for (j = x1 / 8; j < x1 / 8 + floor_division((x2 - x1), 8); j++)
      {
        sendByte(DAT, ~screenData[(296 - i) * 16 + j]);//Command reversed color.
      }
    }
    partialRefreshTime++;
  }
  else
  {
    // To set image clear, after about 5 times of partial refresh, global refresh is needed.
    partialRefreshTime = 0;
    screenWrite();
  }
  screenRefresh();
  screenSleep();
}
