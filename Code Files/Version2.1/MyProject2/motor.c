/*  **** Module written by Matt Payne as part of UC Summer Scholarship 2019 - 2020 for the insulin pump project.
    **** Started on 22/11/2019
    **** This module contains a collection of functions to be used to drive the motor in the pump to deliver insulin.
*/

// define forward and backward directions and a variable to track how many steps the motor has turned.

#include <stdint.h>
#include <stdbool.h>
#include <atmel_start_pins.h>
#include "hal_delay.h"

#define FORWARD 0
#define BACKWARD 1
#define DELAY_TIME 500

uint32_t steps_turned = 0;

bool read_motor_fault_pin()
// read the status of the motor fault pin. Return false if there is a fault with the motor and a true if all is good.
{
	bool fault_pin = gpio_get_pin_level(nFAULT);
	return fault_pin;
}

void set_motor_direction(bool direction)
// call this function to change the value of the DIR pin on the IC which controls the direction the motor turns
{
	gpio_set_pin_level(DIR, direction);
}

void step_motor(bool direction)
/* this function will call set_motor_direction() to set the motor function to turn one step in the direction of the passed variable. If there is a falult with the motor nothing will happen.
   One step will then be recorded in the steps_turned variable to keep track of how much the motor has turned */
{
	
		set_motor_direction(direction);
		gpio_set_pin_level(STEP, 1);
		delay_us(DELAY_TIME);
		gpio_set_pin_level(STEP, 0);
		delay_us(DELAY_TIME);
		steps_turned += 1;
	
	
}

void turn_x_steps(bool direction, uint16_t steps_to_turn)
/* this function will turn the motor a x number of steps in the specified direction. x will have to be worked out from the basal rate, bolus rate, and motor testing to determine
   how much insulin is actually required */ 
{
	gpio_set_pin_level(BOOST_EN, true);
	delay_ms(20);
	while(steps_to_turn > 0)
	{
		step_motor(direction);
		steps_to_turn -=1;
	}
	gpio_set_pin_level(BOOST_EN, false);
}

void motor_return_to_zero()
// return motor back to zero position, ie if the motor has turned forward 25 steps then run motor backwards 25 steps.
{
	if(steps_turned > 0)
	{
		step_motor(BACKWARD);
		steps_turned -= 1;
	}
	
}
