# EPaperNameBadge
This is a name badge using E-Paper screen which shows various information including name, status, time, weather and so on. 

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


