#include <EEPROM.h>

void set_prog() {
  static uint8_t inputBytes = 0;
	
	char inputbuff[32];
	Serial.println(F("PSOC programmer setup"));
	Serial.println();
  showcurrent();
	
	byte exitmenu1 = 0; // need add command to exit menu
  /*
  1 - setup com speed
  2 - test power pin
  3 - check target voltage  
  0 - exit from setup 
*/
	while ( exitmenu1 == 0 ) {
  Serial.println(F("MENU:  pls choice what you want"));
	Serial.println(F("baudrate <*nnn> -> setup serial interface baudrate"));
    Serial.println(F("target <*n=0,1,2> - check power pin speeds, where"));
    Serial.println(F("*******0 -use hardware setup"));
	Serial.println(F("*******1 -use measured setup, but by software"));
    Serial.println(F("*******2 - use default settings 150uSec"));
    Serial.println(F("*******3- show only"));
    Serial.println(F("supply - test target voltage"));
    Serial.println(F("exit - exit setup"));
    Serial.println(F("Showcurrent - Show current settings"));
    Serial.print(F("CHOICE: "));
	while (!Serial.available()){}
	inputbuff[inputBytes] = Serial.read(); 
	if (inputbuff[inputBytes] != '\n' &&
		inputBytes < 32) inputBytes++;  
	else {
		inputbuff[inputBytes] = 0; 
		for (uint8_t i = 0; i < inputBytes; i++)  //    Convert the whole input buffer
        inputbuff[i] = toupper(inputbuff[i]);  // to uppercase characters
		Serial.print(F("\nCommand \""));
		Serial.write(inputbuff);
		Serial.print(F("\" received.\n"));
      /**********************************************************************************************
      ** Parse the single-line command and perform the appropriate action. The current list of **
      ** commands understood are as follows: **
      ** **
      ** baudrate      - Set the device time **
      ** TARGET     - Calibrate device time **
	  ** SUPPLY  -- test power supply
      ** EXIT - exit from setup
      **********************************************************************************************/
		enum commands { Baudrate, Target, Supply,Exit, Showcurrent, Unknown_Command };  // of commands enumerated type
		commands command;                                     // declare enumerated type
		char     workBuffer[10];                              // Buffer to hold string compare
		sscanf(inputbuff, "%s %*s", workBuffer);            // Parse the string for first word
		if (!strcmp(workBuffer, "BAUDRATE")) command = Baudrate;  // Set command number when found
		else if (!strcmp(workBuffer, "TARGET")) command = Target;  // Set command number when found
		else if (!strcmp(workBuffer, "SUPPLY")) command = Supply;  // Set command number when found	
		else if (!strcmp(workBuffer, "EXIT")) command = Exit; 
    else if (!strcmp(workBuffer, "SHOWCURRENT")) command = Showcurrent; // Set command number when found
		else command = Unknown_Command;                              // Otherwise set to not found
		uint16_t tokens;
		unsigned long bodes;  
		Serial.flush();
		switch(command) {
			case Baudrate:
				 tokens = sscanf(inputbuff, "%*s %lu", &bodes);
				if (tokens != 1)  Serial.println(F("Unable to parse operator \n"));
				else {
					switch (bodes) {
						case 115200 :
							Serial.println("115200 set");
							EEPROM.update(0,0);
						break;
						case 57600 :
							Serial.println("57600 set");
							EEPROM.update(0,1);
						break;
						case 38400 :
							Serial.println("38400 set");
							EEPROM.update(0,2);
						break;
						case 19200 :
							Serial.println("19200 set");
							EEPROM.update(0,3);
						break;
						default:
							Serial.println("baudrate is wrong");
						break;
					}
				}  // of if-then-else the date could be parsed
			break;
			case Target:
				{ 
					// test power pin for delays.
					// 1 - define power pin as output -yes yes
					// 2 - off power pin with delay 1 ms - yes
					// 3 - measure timeHIGH on
					//4 - switch target on and wait for comparator
					// 5 measure timeHIGH of
					// 6 - type time off-> on
					// measure timeLOW on
					// switch target off and wait for comparator
					//measure timeOFF of
					// type time on ->of
					// ask for store to eeprom times if yes - store 2 times, ib now use defaults
				tokens = sscanf(inputbuff, "%*s %lu", &bodes);
				if (tokens != 1)  Serial.println(F("Unable to parse argument \n"));
				else {
					Serial.println(bodes);
					Serial.println("here1");
					unsigned long timeON = micros();
					digitalWrite(TARGET_VDD,HIGH);
					delayMicroseconds(10);
					while (bitRead(PIND, PIND6) == HIGH ){} 
					unsigned long timeOFF2ON = micros()- timeON;
					Serial.print(F("Time off to on condition is: "));
					Serial.println(timeOFF2ON);
					delay(5);
					unsigned long timeOFF = micros();
					digitalWrite(TARGET_VDD,LOW);
					delayMicroseconds(10);
					while (bitRead(PIND, PIND6)==HIGH){}
					unsigned long timeON2OFF = micros()- timeOFF;
					Serial.print(F("Time on to off condition is: "));
					Serial.println(timeON2OFF);
					switch (bodes) {
						case 0 :
						{
							Serial.println("Use hardware setup, but for check print next:");
							if ((timeON2OFF > 2500) || (timeOFF2ON > 2500)) {
								timeOFF2ON = 2300;
								timeON2OFF = 50;
								Serial.println("Something wrong, may be testing");
							}
							if (timeON2OFF>timeOFF2ON) {
								EEPROM.update(10,(timeON2OFF+150));
							} else {
								EEPROM.update(10,(timeOFF2ON+150));
							}
							EEPROM.update(9,0);
							break;
						}
						case 1 :
							Serial.println(F("Save measured value to EEPROM and used as software settings"));
							if ((timeON2OFF > 2500) || (timeOFF2ON > 2500)) {
								timeOFF2ON = 2300;
								timeON2OFF = 50;
								Serial.println("Something wrong, may be testing");
							}
							if (timeON2OFF>timeOFF2ON) {
								EEPROM.update(10,(timeON2OFF+150));
							} else {
								EEPROM.update(10,(timeOFF2ON+150));
							}
							uint16_t softdelay =0;
							EEPROM.get(10,softdelay);
							
							Serial.print(softdelay);
							Serial.println(F(" uSec"));
							EEPROM.update(9,1);
						break;
						case 2 :
							Serial.println(F("not stored values, programm settings 150usec use"));
							EEPROM.update(9,2);
							EEPROM.update(10,150); ///  check, may be replace on 1500
						break;
						case 3 :
							Serial.println(F("not stored values, only show its"));
						break;
						
						default: 
							Serial.println("something wrong");
						break;
					}
					byte value1 = EEPROM.read(9);
					Serial.print("9");
					Serial.print("\t");
					Serial.print(value1, DEC);
					Serial.println();
					Serial.println(F("2 - use default 150uSec, 1 - use programm settings, 0 - use hardware setup"));
					uint16_t value2 = 0;
					EEPROM.get(10,value2);
					Serial.print(value2, DEC);
					Serial.println();
					}
				}
			break;
			case Exit:
        Serial.println("Exit");
			break;
      case Showcurrent:
      Serial.println("Showcurrent");
        showcurrent();
      break;
			case Supply:
			{
				float result = map(analogRead(A0), 0, 1023, 0, 1000);
				Serial.println (result/100);
				if ((result/100)<4.9) {  
					Serial.print(F("Target voltage to low "));
				}else if((result/100)>5.1 ) {
					Serial.print(F("Target voltage to high "));
				} else {
					Serial.print(F("Target voltage = 5.0"));
				}
				Serial.println();
			}
			break;
			default:
				Serial.println("something wrong");
				delay(5);
		} //end switch command
		inputBytes = 0; // reset counter
	}
	} //end while
}

void showcurrent(){
  uint16_t timedelay =0;
    Serial.println(F("Current interface speed settings"));
	comspeed = EEPROM.read(0);
	switch (comspeed) {
		case 0:
			Serial.println(F("Serial speed set as 115200"));
		break;
		case 1:
			Serial.println(F("Serial speed set as 57600"));
		break;
		case 2:
			Serial.println(F("Serial speed set as 38400"));
		break;
		case 3:
			Serial.println(F("Serial speed set as 19200"));
		break;
		default:
			Serial.println(F("Something wrong or speed not defined/ Please define it"));
		break;
	}
	Serial.println(F("Current power target time settings"));
	comspeed = EEPROM.read(9);
	switch (comspeed) {
		case 0:
			Serial.println(F("Hardware waiting use"));	
//remove from
			 EEPROM.get(10, timedelay);
			Serial.print(timedelay);
			Serial.println(F(" usec - for check"));	
// to here
		break;
		case 1:
			Serial.println(F("Software waiting, but by hand measurement"));
			 EEPROM.get(10, timedelay);
			Serial.print(timedelay);
			Serial.println(F(" usec"));
		break;
		case 2:
			Serial.println(F("Software default 150uSec"));
		break;
		default:
			Serial.println(F("Something wrong or speed not defined. Will use default value"));
		break;
	}

  Serial.flush();
}