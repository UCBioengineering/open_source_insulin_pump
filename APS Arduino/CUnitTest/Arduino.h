#pragma once

/*	An emulation library for various functions that Arduinos call	*/

#define	HIGH	1
#define	LOW		2

#define INPUT_a	1
#define	OUTPUT_a	2

/*	Internal Data	*/	
#define ARDUINO_TABLE_LENGTH	256

/*	Internal emulation of pins and their modes	*/
char pinTable[ARDUINO_TABLE_LENGTH];

/*	Internal emulation of pins and their values	*/
char pinValue[ARDUINO_TABLE_LENGTH];

/*	Initialized Arduino Emulation Library	*/
void initArduinoLib();
unsigned char pinMode(unsigned char pin, unsigned char mode);
void delay(unsigned int milliseconds);
unsigned int digitalWrite(unsigned char pin, unsigned int value);