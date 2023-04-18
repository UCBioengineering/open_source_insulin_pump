#include "Arduino.h"
#include "Source.h"
#include "TestLibrary.h"

/*	Insulin Pump Communication Test
	
	This checks whether the script's communication function works.
*/
int TestPressButton(int buttonID, int expected)
{
	TestState state = GetCurrentState();

    /*  PressButton */
    int initial = 0, final = 0, result = 1;
    switch (buttonID)
    {
    case APS_BUTTON1:
    case APS_BUTTON2:
    case APS_BUTTON3:
        initial = digitalWrite(buttonID, HIGH);
        result &= pinValue[buttonID] == HIGH;
        delay(100);
        final = digitalWrite(buttonID, LOW);
    default:
        break;
    }

    /*  Expecting initial == HIGH && final == low && pinValue[buttonID] == low */
    result &= initial == HIGH && final == LOW && pinValue[buttonID] == LOW;
	SetCurrentState(state);
    if (result != expected)
        printf("Failed for %d\n", buttonID);
    return result == expected;
}


/*  This is the entry point to the testing code */
int main()
{
	initArduinoLib();
	setup();

    int total = 6;
    int accuracy = 0;
    printf("Running Communication Tests\n");
    accuracy += TestPressButton(APS_BUTTON1, 1);
    accuracy += TestPressButton(APS_BUTTON2, 1);
    accuracy += TestPressButton(APS_BUTTON3, 1);
    accuracy += TestPressButton(-1, 0);
    accuracy += TestPressButton(0, 0);
    accuracy += TestPressButton(255, 0);
    printf("Results: %d/6", accuracy);
    while (1);
}