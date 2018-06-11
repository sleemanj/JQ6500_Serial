JQ6500_Serial
=======================

Simple to use Arduino library to interface to JQ6500 (JQ6500-28P, JQ6500-16P) Mp3 Player Modules

For complete documentation about the JQ6500 Mp3 Player Module, see: 
   http://sparks.gogo.co.nz/jq6500/index.html
   
For a library methods reference see:
   http://sparks.gogo.co.nz/jq6500/doxygen/class_j_q6500___serial.html

For Linux Upload and Windows Upload Repair Tool (JQ6500-16) see:
   https://github.com/NikolaiRadke/JQ6500-rescue-tool

Download, Install and Example
-----------------------------

* Download: http://sparks.gogo.co.nz/JQ6500_Serial.zip
* Open the Arduino IDE (1.0.5)
* Select the menu item Sketch > Import Library > Add Library
* Choose to install the JQ6500_Serial.zip file you downloaded
* Now you can choose File > Examples > JQ6500_Serial > HelloWorld
  
Connecting To Your Arduino
--------------------------

<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/kq6500-16p.jpeg" align="right" title="JQ6500-16p" alt="Pinout image of JQ6500-16p MP3 Player Module For Arduino"/>
<img src="http://sparks.gogo.co.nz/assets/_site_/images/jq6500/jq6500-28.jpeg" align="right" title="JQ6500-28p" alt="Pinout image of JQ6500-28p MP3 Player Module For Arduino"/>

There are two varients of the JQ6500 module as shown.

To use this library with a *5v Arduino*, connect as follows.

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | through a 1K Resistor then to pin 9 |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

To use this library with a *3v3 Arduino*, connect as follows...

| JQ6500 Module | Arduino |
| ------------- | ------- |
| RX            | pin 9   |
| TX            | pin 8   |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

You can use pins other than 9 and 8 if you wish, simply set them in your code.



ESP32
--------------------------

To use this library with an ESP32, connect as follows...

The ESP brings real HardwareSerial so it will not use SoftwareSerial for this.

| JQ6500 Module | ESP32   |
| ------------- | ------- |
| RX            | pin 17  |
| TX            | pin 16  |
| GND (any of)  | GND     |
| VCC (any of)  | VCC     |

You can use pins other than 16 and 17 if you wish, simply set them in your code.

You also have to include the JQ6500_Serial_ESP32.h instead of the JQ6500_Serial.h

The constuctor looks like this: JQ6500_Serial_ESP32 mp3(uartNr);

The uartNr must be 0,1 or 2.

Default mapping of the pins according to: https://github.com/espressif/arduino-esp32/blob/master/cores/esp32/HardwareSerial.cpp is

| UART Nr | RX | TX |
| ------- | -- | -- |
| 0       | 3  | 1  | 
| 1       | 9  | 10 |
| 2       | 16 | 17 |

To use the default pins just call:

  mp3.begin(9600)

To use other pins than the default ones call begin like this:

  mp3.begin(9600,SERIAL_8N1,rxPin,txPin)





Power Demands
--------------------------

If using the on-board speaker driver, then naturally the power
demands are significant, and your USB power may not be sufficient
at more 1/3rd level of volume or so, the symptom is the audo 
breaking up and potentially resetting when volume increases.

You should use either an external power source, an external amp, or a lower
volume if you experience this problem.

Usage
--------------------------

Open the HelloWorld example.
