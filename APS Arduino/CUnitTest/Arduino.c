#include "Arduino.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

#include <stdio.h>

extern char pinTable[ARDUINO_TABLE_LENGTH];

extern char pinValue[ARDUINO_TABLE_LENGTH];

void initArduinoLib()
{
	for (int i = 0; i < ARDUINO_TABLE_LENGTH; i++)
	{
		pinTable[i] = 0;
		pinValue[i] = 0;
	}
}
unsigned char pinMode(unsigned char pin, unsigned char mode)
{
	pinTable[pin] = mode;
	if (mode == INPUT_a)
		printf("Setting pin #%d to INPUT\n", pin);
	else if (mode == OUTPUT_a)
		printf("Setting pin #%d to OUTPUT\n", pin);
	else
		printf("Setting pin #%d to UNKNOWN:%d \n", pin, mode);
	return mode;
}
void delay(unsigned int milliseconds)
{
	Sleep(milliseconds);
}
unsigned int digitalWrite(unsigned char pin, unsigned int value)
{
	switch (value)
	{
	case HIGH:
		printf("Writing pin #%d to HIGH\n", pin);
		pinValue[pin] = value;
		break;
	case LOW:
		printf("Writing pin #%d to LOW\n", pin);
		pinValue[pin] = value;
		break;
	default:
		printf("Writing pin #%d to UNKNOWN: %d\n", pin, value);
		pinValue[pin] = value;
		break;
	}
	return value;
}