# arduino_hssp

This is a fork of the original arduino_hssp https://github.com/acidwise/arduino_hssp decause I want to rebuild it for Arduino nano and give it to work/

As for me no original branch have problem with TARGET VDD pin, it not work now.

I will store scetch into src folder

Also I'll add schematic for my programmer, because my boards needs more then 150 ma power current from 5V 

## Usage

Clone the code into a folder called 'arduino_hssp', open the project in the Arduino IDE and compile and program it. Connect your PSoC 1 device as follows (can be changed in issp_defs.h):
```
Pin   - PCB - Arduino
------------------------
Power -  1  -  RAW
GND   -  2  -  GND
XRES  -  3  -  4
SCLK  -  4  -  8
SDATA -  5  -  9
```
Use psocdude by miracoli https://github.com/miracoli/psocdude (requires linux) to flash the firmware: 
```
psocdude -C psocdude.conf -p CY8C24894 -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:TM_Warthog_Throttle_v23.bin:r