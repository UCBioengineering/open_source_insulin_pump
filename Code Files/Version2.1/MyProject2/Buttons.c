/* Module containing functions to interface Buttons, and also set up three external interrupts
   linked to those buttons.
   
   Code written by Matt Payne for University of Canterbury Final Year Project as part the Insulin pump project,
   Project supervised by Geoff Chase, Jennifer Knopp, Lui Holder Pearson and Jake Campbell.
   
   22/08/20
 */

#include "atmel_start_pins.h"
#include "stdint.h"
#include "hal_ext_irq.h"
#include "hal_timer.h"
#include "stdbool.h"
#include "driver_init.h"
#include "utils.h"
#include "Buttons.h"

/**********************************************************************************************
 * Declare variables
**********************************************************************************************/
extern uint8_t button_array[3];
extern uint8_t basal_rate[1];
extern uint16_t flash_address;
bool button_1 = false;
bool button_2 = false;
bool button_3 = false;
static struct timer_task TIMER_0_task1;

/**********************************************************************************************
 * Function definitions
**********************************************************************************************/
void init_button_array()
{
	button_array[0] = 0;
	button_array[1] = 0;
	button_array[2] = 0;
}
// cb function called when timer runs down. Checks a set of bools to see which button was
// pressed and increments an array entry linked to that button. Array is checked in main.
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
	if (button_1)
	{
		button_array[0]++;
		button_1 = false;
		timer_stop(&TIMER_0);
		flash_erase(&FLASH_0, flash_address, 1);
		store_delivery_data();
		set_leds(button_array[0]);
	}
	else if (button_2)
	{
		button_array[0]--;
		button_2 = false;
		timer_stop(&TIMER_0);
		flash_erase(&FLASH_0, flash_address, 1);
		store_delivery_data();
		set_leds(button_array[0]);
	}
	else if  (button_3)
	{
		button_array[2]++;
		button_3 = false;
		main();
	}
}

// Initialisation for TIMER_0 peripheral
void TIMER_0_setup(void)
{
	TIMER_0_task1.interval = 10;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	
	timer_add_task(&TIMER_0, &TIMER_0_task1);
}

// cb function which starts TIMER_0 when button 1 pressed
static void button_1_cb(void)
{
	button_1 = true;
	timer_start(&TIMER_0);
}

// cb function which starts TIMER_0 when button 2 pressed
static void button_2_cb(void)
{
	button_2 = true;
	timer_start(&TIMER_0);
}

// cb function which starts TIMER_0 when button 3 pressed
static void button_3_cb(void)
{
	button_3 = true;
	timer_start(&TIMER_0);
}


// Initialise external interrupts, and tie them to callback functions.
// Interrupts are disabled and then enabled, otherwise they are triggered
// on startup.
void init_interrupts(void)
{
	ext_irq_disable(BUTTON1);
	ext_irq_disable(BUTTON2);
	ext_irq_disable(BUTTON3);
	ext_irq_register(BUTTON2, button_2_cb);
	ext_irq_register(BUTTON1, button_1_cb);
	ext_irq_register(BUTTON3, button_3_cb);
	ext_irq_enable(BUTTON1);
	ext_irq_enable(BUTTON2);
	ext_irq_enable(BUTTON3);
}

void set_leds(uint8_t button_count)
{
	if(button_count < 1) 
	{
		gpio_set_pin_level(OUT7, false);
		gpio_set_pin_level(OUT1, false);
		gpio_set_pin_level(OUT6, false);
		gpio_set_pin_level(OUT4, false);
	}
	if (button_count == 1 )
	{
		gpio_set_pin_level(OUT7, true);
		gpio_set_pin_level(OUT1, false);
		gpio_set_pin_level(OUT6, false);
		gpio_set_pin_level(OUT4, false);
		
	}
	if (button_count == 2 )
	{
		gpio_set_pin_level(OUT7, true);
		gpio_set_pin_level(OUT1, true);
		gpio_set_pin_level(OUT6, false);
		gpio_set_pin_level(OUT4, false);
		
	}
	if (button_count == 3 )
	{
		gpio_set_pin_level(OUT7, true);
		gpio_set_pin_level(OUT1, true);
		gpio_set_pin_level(OUT6, true);
		gpio_set_pin_level(OUT4, false);
		
	}
	if (button_count == 4 )
	{
		gpio_set_pin_level(OUT7, true);
		gpio_set_pin_level(OUT1, true);
		gpio_set_pin_level(OUT6, true);
		gpio_set_pin_level(OUT4, true);
	}
}

void leds_off(void)
{
	gpio_set_pin_level(OUT1, false);
	gpio_set_pin_level(OUT6, false);
	gpio_set_pin_level(OUT7, false);
	gpio_set_pin_level(OUT4, false);
}




