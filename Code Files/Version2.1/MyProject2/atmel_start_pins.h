/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define SS_RTC GPIO(GPIO_PORTA, 0)
#define SS_FLASH GPIO(GPIO_PORTA, 1)
#define ADC_BATT_TEST GPIO(GPIO_PORTA, 2)
#define BOOST_EN GPIO(GPIO_PORTA, 3)
#define EINK_EN GPIO(GPIO_PORTA, 4)
#define BUTTON2 GPIO(GPIO_PORTA, 5)
#define BUTTON1 GPIO(GPIO_PORTA, 6)
#define SCK GPIO(GPIO_PORTA, 7)
#define MISO GPIO(GPIO_PORTA, 8)
#define BUTTON3 GPIO(GPIO_PORTA, 9)
#define MOSI GPIO(GPIO_PORTA, 10)
#define OUT1 GPIO(GPIO_PORTA, 11)
#define OUT2 GPIO(GPIO_PORTA, 12)
#define nFAULT GPIO(GPIO_PORTA, 13)
#define STEP GPIO(GPIO_PORTA, 14)
#define DIR GPIO(GPIO_PORTA, 15)
#define OUT3 GPIO(GPIO_PORTA, 16)
#define OUT4 GPIO(GPIO_PORTA, 17)
#define OUT5 GPIO(GPIO_PORTA, 18)
#define SCK_EXT GPIO(GPIO_PORTA, 19)
#define OUT6 GPIO(GPIO_PORTA, 20)
#define MISO_EXT GPIO(GPIO_PORTA, 22)
#define OUT7 GPIO(GPIO_PORTA, 23)
#define MOSI_EXT GPIO(GPIO_PORTA, 24)
#define APS_BUTTON1 GPIO(GPIO_PORTA, 25)
#define APS_BUTTON2 GPIO(GPIO_PORTA, 27)
#define APS_BUTTON3 GPIO(GPIO_PORTA, 28)
#define SWDCLK GPIO(GPIO_PORTA, 30)
#define SWDIO GPIO(GPIO_PORTA, 31)
#define BUSY GPIO(GPIO_PORTB, 2)
#define RES GPIO(GPIO_PORTB, 3)
#define DC_EINK GPIO(GPIO_PORTB, 8)
#define SS_EINK GPIO(GPIO_PORTB, 9)
#define OUT8 GPIO(GPIO_PORTB, 10)
#define OUT9 GPIO(GPIO_PORTB, 11)
#define USB_DETECT GPIO(GPIO_PORTB, 22)

#endif // ATMEL_START_PINS_H_INCLUDED
