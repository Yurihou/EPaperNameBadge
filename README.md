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

For offline time display, we added a DS1302 to store time data:
|DS1302|ESP32|
|------|-----|
|DAT(I/O)|25|
|CLK|32|
|RST(EN)|33|

## Software
In fact, we can create a library about EPaper screen. However, in order to test the screen, we use just basic SPI library to control the screen.

The programe refers to the example code from YWRobot.

### Bluetooth AT command
To control the EPaper, time chip, Wi-Fi and so on, we use AT command.

To send command, we use Bluetooth serial APP (such as "Bluetooth Serial" on Android or Harmony OS, "BluTerm" on iOS, any serial tool on Windows, Linux or MacOS), power on the EPaper, connect the Bluetooth "EPapaer_Screen", then open Bluetooth serial APP and connect the bluetooth serial. Then we just send AT command thru Bluetooth.

![394275306ac3181d10a8feeccb368d2](https://user-images.githubusercontent.com/46369018/135706860-7fe9b973-b9be-4860-98a2-db6d133e78a5.jpg)
![image](https://user-images.githubusercontent.com/46369018/135706869-36279f6b-2a94-4c16-a10e-473acbd59e16.png)

Detailed AT command list will be upload later. Here comes basic examples:

|AT Command|Function|
|--------|--------|
|AT+TIME?|Check local time.|
|AT+TIME 2021/10/02,10:00,7|Set local time to 10:00, Oct.2, 2021, Sunday.|
|AT+CJWAP?|Check current Wi-Fi status.|
|AT+CJWAP Tsinghua-2G|Connect to Wi-Fi "Tsinghua-2G" without password.|
|AT+CJWAP Tsinghua-2G 1145141919810|Connect to Wi-Fi "Tsinghua-2G" with password "1145141919810".|
|AT+CWLAP|Scan and display all available Wi-Fi.|
|AT+CWQAP|Disconnect Wi-Fi.|

## Update Log
2021.9.26 V0.1.0 

- First Version Uploaded.

2021.10.1 V0.1.1

- Bluetooth Serial added.
- Three status LED added.
- Rewrite EPapae control as a class.
- Back shell (temporary version with handmade circuit board) uploaded.
- EInk Screen datasheet uploaded.


2021.10.02 V0.2.0

- DS1302 Added.
- Simple AT command added.
- Able to connect to Wi-Fi.
