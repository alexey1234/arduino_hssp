# arduino_hssp

This is a fork of the original arduino_hssp by miracoli: https://github.com/miracoli/arduino_hssp , which includes the additional commands by trou: https://github.com/trou/arduino_hssp and the bank selection patch by walmis: https://github.com/walmis/arduino_hssp

The goal of the work is to provide a tool for repairing Thrustmaster HOTAS Warthog Throttle, which is notorious for firmware corruption and physical damage of the USB line. See https://forums.eagle.ru/topic/54873-post-motherboard-specs-of-bricked-tm-warthogs-here-please/ for more detail.

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
```


## Project status
Tested and working with:
* Arduino Leonardo (Pro Micro)
* CY8C21434
* CY8C24894

## TODOs
* Implement security configuration read and write.
