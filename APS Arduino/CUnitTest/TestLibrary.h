#pragma once

/*	Contains various test structures and functions	*/

/*	Contains a state of the Arduino emulation library	*/
typedef struct TestState {
	char pinTable[256];
	char pinValue[256];
} TestState;

/*	Gets the current state of the Arduino emulation library	*/
TestState GetCurrentState();
/*	Sets the current state of the Arduino emulation library	*/
void SetCurrentState(TestState state);