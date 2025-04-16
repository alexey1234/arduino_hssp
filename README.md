# arduino_hssp

This is a fork of the original arduino_hssp https://github.com/acidwise/arduino_hssp because I want to rebuild it for Arduino nano and give it to work.

As for me no original branch have problem with TARGET VDD pin.

Also board work with tty speed 56800 bod maximum, but maybe because I have usb cable 3 meters (?)

I will store scetch into src folder

Also I'll add schematic for my programmer, because my boards needs more then 150 ma power current from 5V - pin of Arduino must give no more 40ma


# Attention
```diff
-Software under work now at 16 Apr 2025. I will remove this message when it will ready for use
```

## Usage

Clone the code from src into a folder called 'arduino_hssp', open the project in the Arduino IDE and compile and program it. Connect your PSoC 1 device as follows (can be changed in issp_defs.h):
```
Pin   - PCB - Arduino
------------------------
GND   -  1  -  GND
Power -  2  -  TARGET VDD
XRES  -  3  -  4
SCLK  -  4  -  8
SDATA -  5  -  9
```
Use psocdude by miracoli https://github.com/miracoli/psocdude (requires linux) to flash the firmware: 
```
psocdude -C psocdude.conf -p CY8C24894 -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:TM_Warthog_Throttle_v23.bin:r
```

In my case  as Arduino nano it read device from command :

```
psocdude -C /usr/local/etc/psocdude.conf -p CY8C24894 -c arduino -P /dev/ttyUSB0 -b 58600 -U flash:r:flash.bin:r
...

