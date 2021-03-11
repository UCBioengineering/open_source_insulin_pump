/* Main module for the insulin pump FYP project.

   Code written by Matt Payne for University of Canterbury Final Year Project as part the Insulin pump project,
   Project supervised by Geoff Chase, Jennifer Knopp, Lui Holder Pearson and Jake Campbell.
   23/08/20
 */

#include "atmel_start.h"
#include "hal_delay.h"
#include "hal_gpio.h"
#include "hal_flash.h"
#include "motor.h"
#include "SPI.h"
#include "eink_luts_and_buf.h"
#include "eink.h"
#include "rtc.h"
#include "Buttons.h"
#include "hpl_ext_irq.h"
#include "stdbool.h"
#include "FlashMCU.h"

/**********************************************************************************************
 * Definitions of constants
**********************************************************************************************/
// One unit is actually 128 steps, but may need to deliver in smaller amounts.
// For 298:1 motor one unit is 715 steps. Deliver in 10ths of a unit to make calculations easier
// i.e dont need to use floats

#define ONE_TENTH_UNIT 71

/**********************************************************************************************
 * Declare variables
**********************************************************************************************/

struct io_descriptor* spi_io; // Pointer to SPI_0 module
unsigned char date_and_time[40]; // Array to store a formatted date and time
uint8_t button_array[3] = {0, 0, 0}; // Stores 3 uint8_t variables holding number of button presses
uint8_t basal_rate[1] = {4};
bool basal_insulin_delivered; // Flag to check if basal insulin delivered
bool bolus_insulin_delivered; // Flag to check if bolus insulin delivered
bool cycle_complete;
uint8_t count[1] = {0};
uint16_t flash_address = 0x4000;
uint8_t address_array[2];
uint8_t write_array[2];
uint16_t writes = 0;

/**********************************************************************************************
 * Begin main function
**********************************************************************************************/

int main(void)
{
	basal_insulin_delivered = false;
	bolus_insulin_delivered = false;
	cycle_complete = false;
	
	
	atmel_start_init(); // Initializes MCU, drivers and middleware.
	
	SPI_Begin(); // Start SPI Peripheral.
	
	TIMER_0_setup(); // Setup TIMER_0 Peripheral used for debouncing
	
	init_interrupts(); // Register interrupts and tie them to a callback function.
	
	// Check a general purpose bit in the RTC to find out if the
	// time was set previously. Set it if not.
	if(!check_rtc_initialised()) 
	{
		initialise_rtc();
		rtc_set_current_time(20, 8, 21, 16, 28, 00); // Enter date and time values here
		basal_insulin_delivered = true; // Set true so insulin isn't delivered on first start up
		flash_initialise();
		get_delivery_data();
		check_writes();
	}
	else 
	// If the time had been set previously then the MCU just woke up
	// due to interrupt from RTC. Clear the interrupt so the clock
	// can send another when required.
	{	
		rtc_clear_interrupts();
		get_delivery_data();
		check_writes();
		set_leds(button_array[0]);
	}
	
	//while(1)
	//{
		//turn_x_steps(false, ONE_UNIT);
	//}
	
	while(!cycle_complete)
	
	{
		set_leds(button_array[0]);
		// Deliver insulin when RTC interrupts and resets MCU
		if(!basal_insulin_delivered)
		{
			count[0]++;
			if (count[0] < 6)
			{
			 //do nothing
			} else {
				button_array[0]++;
			}
		}
		
		// Increment array when button 1 is pushed and decrement array when button 2 is pushed
		
		button_array[0]++; //increment for testing purposes only!
		if(button_array[0] > 0)
		{
			turn_x_steps(true, ONE_TENTH_UNIT);
			button_array[0]--;
		}
		
		cycle_complete = true;
	}
	
	flash_erase(&FLASH_0, flash_address, 1);
	store_delivery_data();
	delay_ms(5000);
	leds_off();
	sleep(2);
}

