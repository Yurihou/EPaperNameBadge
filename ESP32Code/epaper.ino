#include <SPI.h>
#include "screen.h"
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

eInkScreen screen;
BluetoothSerial SerialBT;

#define BluetoothLED 12

// extern char screenData[];

void setup() {
  // put your setup code here, to run once:

  pinMode(BluetoothLED,OUTPUT);
  Serial.begin(115200);
  SerialBT.begin("EPapaer_Screen"); //Bluetooth device name
  delay(1);
  screen.screenGlobalChange();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  //Detect whether Bluetooth is connected and turn on or off GPIO12
  digitalWrite(BluetoothLED,SerialBT.connected());
  delay(50);
}
