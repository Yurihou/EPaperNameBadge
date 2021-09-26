#ifndef _SCREEN_H_
#define _SCREEN_H_

#define VSPI_MISO   MISO
#define VSPI_MOSI   MOSI
#define VSPI_SCLK   SCK
#define VSPI_SS     SS

#define DAT HIGH
#define CMD LOW

#define DC 21
#define RST 22
#define ISBUSY 17

static const int spiClk = 1000000; // 1 MHz

void SPIInit();

void screenChange();

#endif
