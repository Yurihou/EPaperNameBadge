#include <SPI.h>
#include "screen.h"

// extern char screenData[];

void setup() {
  // put your setup code here, to run once:

  SPIInit();
  Serial.begin(115200);
  screenChange();
}

void loop() {
  // put your main code here, to run repeatedly:
}
