/* Module to read/write data to/from the NVM of the MCU.

   Code written by Matt Payne for University of Canterbury Insulin pump Final Year Project.
   
   Project supervised by Geoff Chase, Jennifer Knopp, Lui Holder Pearson and Jake Campbell.
   
   23/08/20
 */

#include "atmel_start_pins.h"
#include "hal_flash.h"
#include "stdint.h"
#include "hal_ext_irq.h"
#include "hal_timer.h"
#include "stdbool.h"
#include "driver_init.h"
#include "utils.h"
#include "Buttons.h"
#include "FlashMCU.h"

extern uint8_t basal_rate[1];
extern uint8_t button_array[3];
extern uint8_t count[1];
extern uint16_t flash_address;
extern uint16_t address_array[2];
extern uint8_t write_array[2];
extern uint16_t writes;


void store_delivery_data()
{
	uint32_t page_size;

	/* Init source data */
	page_size = flash_get_page_size(&FLASH_0);
	writes++;
	split_to_8();
	
	uint8_t storage_array[128];
	storage_array[0] = button_array[0];
	storage_array[1] = basal_rate[0];
	storage_array[2] = count[0];
	storage_array[3] = write_array[0];
	storage_array[4] = write_array[1];
	storage_array[5] = address_array[0];
	storage_array[6] = address_array[1];
		
	/* Write data to flash */
	flash_write(&FLASH_0, flash_address, storage_array, page_size);
}

void get_delivery_data()
{
	uint32_t page_size;

	/* Init source data */
	page_size = flash_get_page_size(&FLASH_0);
	
	uint8_t storage_array[128];
	
	/* Read data from flash */
	flash_read(&FLASH_0, flash_address, storage_array, page_size);
	
	button_array[0] = storage_array[0];
	basal_rate[0] = storage_array[1];
	count[0] = storage_array[2];
	write_array[0] = storage_array[3];
	write_array[1] = storage_array[4];
	address_array[0] = storage_array[5];
	address_array[1] = storage_array[6];
	join_to_16();
}

void flash_initialise(void)
{
	flash_address = 0x4000;
	flash_erase(&FLASH_0, flash_address, 1);
	store_delivery_data();
}

void split_to_8(void)
{
	write_array[0] = writes >> 8;
	write_array[1] = writes;
	address_array[0] = flash_address >> 8;
	address_array[1] = flash_address;
}

void join_to_16(void)
{
	writes = 0;
	writes = ((write_array[0] << 8) | write_array[1]);
	flash_address = 0;
	flash_address = ((address_array[0] << 8) | address_array[1]);
	
}

void check_writes(void)
{
	if (writes < 7000)
	{
		flash_address = 0x4000;
	}
	else if (writes >= 7000 && writes < 14000)
	{
		flash_address = 0x4100;
	}
	else if (writes >= 14000 && writes < 21000)
	{
		flash_address = 0x4200;
	}
	else if (writes >= 21000 && writes < 28000)
	{
		flash_address = 0x4300;
	}
	else if (writes >= 28000 && writes < 35000)
	{
		flash_address = 0x4400;
	}
	else
	{
		flash_address = 0x4500;
	}
}