/* Module containing functions to interface with the flash memory.
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   08/01/20
 */

// set first block of flash memory to hold current address, and any program data required such as maybe the
// arrays containing basal/bosul data. This block will be continually written and re written every time the flash
// is used, but it can take 100000 write/erase cycles anyway, and the flash memory won't be in the final product, its just
// there to log data for testing so no worries.
// when using mux with no peripherals connected, the mux ctrl is just held high giving normal operation to the flash chip

extern struct io_descriptor *spi_io;

#include "atmel_start_pins.h"
#include "stdint.h"
#include "hal_io.h"
#include "insulin_rate.h"
#include "hal_delay.h"
#include "stdbool.h"
#include "rtc.h"

void convert_number_to_bytes(uint8_t number_formatted[], uint32_t number)
{
	number_formatted[0] = ((number >> 16) & 0xFF);
	number_formatted[1] = ((number >> 8) & 0xFF);
	number_formatted[2] = (number & 0xFF);
}

uint32_t convert_bytes_to_number(uint8_t number_formatted[])
{
	uint32_t number = 0;
	uint32_t temp_number = 0;
	number += number_formatted[0] & (0b11111111);
	temp_number = (number_formatted[1] << 8);
	number += temp_number & (0b1111111100000000);
	temp_number = 0;
	temp_number = (number_formatted[2] << 16);
	number += temp_number & (0b111111110000000000000000);
	
	return number;
}

// used to set the write enable bit, so a write can be made to the flash. write enable Automatically reset after the write is
// complete, hence this function must be used before every write.
void set_write_enable()
{
	uint8_t address = 0x06;
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &address, 1);
	gpio_set_pin_level(SS_FLASH, true);
}

void flash_write_operation(uint8_t address, uint8_t data[], uint8_t data_length)
{
	uint8_t address_and_data[(data_length + 1)]; // create an array one element larger to hold 8bit address and data
	uint8_t i;
	address_and_data[0] = address; // set first element as address
	for(i = 1; i < (data_length + 1); i++)
	{
		address_and_data[i] = data[(i-1)];
	}
	set_write_enable();
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &address_and_data[0], (data_length + 1 ));
	gpio_set_pin_level(SS_FLASH, true);
}

// generic function to read data from the flash, as above can read subsequent registers in one go.
void flash_read_operation(uint8_t address[], uint8_t data[], uint8_t data_length)
{
	uint8_t command_and_address[4];
	command_and_address[0] = 0x03;
	uint8_t i;
	for(i = 1; i < 4; i++)
	{
		command_and_address[i] = address[i-1];
	} 
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &command_and_address[0], 4);
	io_read(spi_io, &data[0], data_length);
	gpio_set_pin_level(SS_FLASH, true);
	delay_ms(500);
}

// writes the non volatile bits in the status register
void flash_set_status_register()
{
	uint8_t address = 0x01;
	uint8_t data[2];
	data[0] = address;
	data[1] = 0b10000000;
	set_write_enable();
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &data[0], 2);
	gpio_set_pin_level(SS_FLASH, true);
}


bool flash_check_write_in_progress()
{
	uint8_t command = 0x05;
	uint8_t data[1];
	bool result;
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &command, 1);
	io_read(spi_io, &data[0], 1);
	gpio_set_pin_level(SS_FLASH, true);
    uint8_t GP0 = data[0] & 0b00000001;
	if (GP0 == 0b00000001)
	{
		result = true;
		} else {
		result = false;
	}
	return result;
}

//void flash_initialise()
//{
	//gpio_set_pin_level(BUSY, true);
	//gpio_set_pin_level(SS_FLASH, false); // flash enable must permanently be set to false as there is an issue with the
	                                     //// circuit design causing the MOSFET to send a noisy 2.8v supply voltage if set
										 //// to true
	//flash_set_status_register();
//}

void flash_page_program_operation(uint8_t data[], uint8_t address[], uint16_t data_length)
{
	uint16_t i;
	// command for page program operation
	uint8_t command = 0x02;
	// Max bytes of data for PP is 256, so check that there is not more data than this.
	if(data_length > 256)
	{
		return;
	}
	// create an array large enough to hold 1 byte command, 3 byte address and up to 256 bytes
	uint8_t command_addr_data[(data_length + 4)];
	command_addr_data[0] = command;
	// loop through the three address bytes and add them to the command_addr_data array
	for(i = 1; i < 4; i++)
	{
		command_addr_data[i] = address[i-1];
	}
	for(i = 4; i < (data_length + 4); i++)
	// loop through data array and add it to command_addr_data array
	{
		command_addr_data[i] = data[i-4];
	}
	// send the command, address and data in one burst
	set_write_enable();
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &command_addr_data[0], (data_length + 4));
	gpio_set_pin_level(SS_FLASH, true);
	delay_ms(500);
}

void set_insulin_and_addr_data()
{
	uint8_t data[48];
	uint8_t memory_address[3] = {0x00};
	set_insulin_data(data);
	flash_page_program_operation(data, memory_address, 48);
}

void flash_chip_erase()
{
	uint8_t command = 0x60;
	set_write_enable();
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &command, 1);
	gpio_set_pin_level(SS_FLASH, true);
	delay_ms(500);
}

void flash_sector_erase(uint16_t sector_number)
{
	uint32_t address = (4096 * sector_number);
	uint8_t address_formatted[4];
	address_formatted[0] = 0x20;
	address_formatted[1] = ((address >> 16) & 0xFF);
	address_formatted[2] = ((address >> 8) & 0xFF);
	address_formatted[3] = (address & 0xFF);
	set_write_enable();
	gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &address_formatted[0], 4);
	gpio_set_pin_level(SS_FLASH, true);
	delay_ms(500);
}

void read_status_register(uint8_t data[1])
{
	uint8_t command = 0x05;
    gpio_set_pin_level(SS_FLASH, false);
	io_write(spi_io, &command, 1);
	io_read(spi_io, &data[0], 1);
	gpio_set_pin_level(SS_FLASH, true);
}

void get_units(unsigned char date_and_time[20], uint8_t units)
{
	
    uint8_t unitsbcd = bin2bcd(units);
	uint8_t ascii[2] = {0};
	bcd2ascii(unitsbcd, ascii);
	date_and_time[51] = ascii[0];
	date_and_time[52] = ascii[1];
}

void log_time_and_insulin(uint8_t current_rate)
{
	uint8_t retreived_address[3];
	uint8_t date_and_time_bcd[5];
	rtc_read_operation(0x02, date_and_time_bcd, 5);
	uint8_t insulin_data[6];
	for(int i = 0; i < 5; i++)
	{
		insulin_data[i+1] = date_and_time_bcd[i];
	}
	insulin_data[0] = current_rate;
    uint32_t address = 4096;
	uint8_t address_formatted[3];
	convert_number_to_bytes(address_formatted, address);
	flash_read_operation(address_formatted, retreived_address, 3);
	flash_page_program_operation(insulin_data, retreived_address, 6);
	flash_sector_erase(1);
	uint32_t new_address = convert_bytes_to_number(retreived_address);
	uint8_t new_address_formatted[3];
	convert_number_to_bytes(new_address_formatted, new_address);
	flash_page_program_operation(new_address_formatted, retreived_address,3);
	
		
}

void store_starting_address()
{
	uint32_t address = 4096;
	uint8_t address_formatted[3];
	convert_number_to_bytes(address_formatted, address);
	
	uint32_t data = 8192;
	uint8_t data_formatted[3];
	convert_number_to_bytes(data_formatted, data);
	
	flash_page_program_operation(data_formatted, address_formatted, 3);
}





