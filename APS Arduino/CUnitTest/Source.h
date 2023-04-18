#pragma once

/*
This is the source Arduino code converted into C for testing.
*/
#include "Arduino.h"

#define APS_BUTTON1  6
#define APS_BUTTON2  7
#define APS_BUTTON3  18

void setup() {
    /*  Setup output direction for pins */
    pinMode(APS_BUTTON1, OUTPUT_a);
    pinMode(APS_BUTTON2, OUTPUT_a);
    pinMode(APS_BUTTON3, OUTPUT_a);
}

/*  Take in a button ID and send a signal for .1s to Insulin Pump System (IPS)
    Does nothing if buttonID does not match existing pins.
*/
void PressButton(unsigned char buttonID)
{
    switch (buttonID)
    {
    case APS_BUTTON1:
    case APS_BUTTON2:
    case APS_BUTTON3:
        digitalWrite(buttonID, HIGH);
        delay(100);
        digitalWrite(buttonID, LOW);
    default:
        break;
    }
}

void loop() {

}

