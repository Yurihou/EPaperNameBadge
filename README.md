# EPaperNameBadge
This is a name badge using E-Paper screen which shows various information including name, status, time, weather and so on. 

![522e120fe108bd40b3b348502633d6c](https://user-images.githubusercontent.com/46369018/135572402-bca31bfd-30ce-4aa4-ac79-32095521ecb7.jpg)

This project uses ESP32 as controller, which is able to connect Bluetooth and Wi-Fi. Related function is developing.

## Hardware
EPaperNameBadge uses YWRobot 2.9 inch E-ink screen as main screen, which displays 296x128 monochrome(maybe with red pixel?) bitmap on itself.

![image](https://user-images.githubusercontent.com/46369018/134794718-e993ee51-138a-4f9d-b159-ea6dcb419fdb.png)

The screen use GDEW029T5 as driver. The datasheet can be found in path /datasheet.

The screen uses SPI interface connected to an ESP32. There is 8 wires to connect:

|EPaper|ESP32|
|---|---|
|BUSY|17|
|RST|22|
|DC|21|
|CS|5|
|MOSI(DIN)|23|
|SCLK|18|
|VCC|3V3 or 5V|
|GND|GND|

We also added 3 LED to show Power, Bluetooth and Wi-Fi (preserved) connection status:
|LED|Anode|Cathode|
|---|-----|-------|
|Power|5V/3V3|GND|
|Bluetooth|12|GND|
|Wi-Fi|14|GND|

## Software
In fact, we can create a library about EPaper screen. However, in order to test the screen, we use just basic SPI library to control the screen.

The programe refers to the example code from YWRobot.

## Update Log
2021.9.26 V0.1.0 

- First Version Uploaded.

2021.10.1 V0.1.1

- Bluetooth Serial added.
- Three status LED added.
- Rewrite EPapae control as a class.
- Back shell (temporary version with handmade circuit board) uploaded.
