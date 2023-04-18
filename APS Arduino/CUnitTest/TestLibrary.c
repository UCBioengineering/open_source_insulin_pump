#include <stdio.h>
#include <string.h>

#include "Arduino.h"
#include "TestLibrary.h"

TestState GetCurrentState()
{
	TestState state;
	memcpy(state.pinTable, pinTable, sizeof(char) * ARDUINO_TABLE_LENGTH);
	memcpy(state.pinValue, pinValue, sizeof(char) * ARDUINO_TABLE_LENGTH);
	return state;
}
void SetCurrentState(TestState state)
{
	memcpy(pinTable, state.pinTable, sizeof(char) * ARDUINO_TABLE_LENGTH);
	memcpy(pinValue, state.pinValue, sizeof(char) * ARDUINO_TABLE_LENGTH);
}