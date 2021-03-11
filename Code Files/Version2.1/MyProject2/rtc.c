/* Module containing functions to interface with the real time clock.
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   08/01/20
   
   Modified 26/08/2020
 */

#include "stdint.h"
#include "stdbool.h"
#include "atmel_start_pins.h"
#include "hal_io.h"
#include "rtc.h"
#include "motor.h"
#include "SPI.h"

extern struct io_descriptor *spi_io;

// bcd2bin and bin2bcd written by max
uint8_t bcd2bin(uint8_t value){
	return (value & 0x0F) + ((value >> 4) * 10);
}

uint8_t bin2bcd(uint8_t value){
	return ((value / 10) << 4) + value % 10;
}

// generic function to write address and any data to the rtc, complies the data and address into a
// single array and writes in one go, can be used to write multiple subsequent registers.
void rtc_write_operation(uint8_t address, uint8_t data[], uint8_t data_length)
{
	uint8_t address_and_data[(data_length + 1)]; // create an array one element larger to hold 8bit address and data
	uint8_t i;
	address_and_data[0] = address; // set first element as address
	for(i = 1; i < (data_length + 1); i++)
	{
		address_and_data[i] = data[(i-1)];
	}
	gpio_set_pin_level(SS_RTC, false);
	io_write(spi_io, &address_and_data[0], (data_length + 1 ));
	gpio_set_pin_level(SS_RTC, true);
}

// generic function to read data from the rtc, as above can read subsequent registers in one go.
void rtc_read_operation(uint8_t address, uint8_t data[], uint8_t data_length)
{
	gpio_set_pin_level(SS_RTC, false);
	io_write(spi_io, &address, 1);
	io_read(spi_io, &data[0], data_length);
	gpio_set_pin_level(SS_RTC, true);
}

// following functions set registers on the rtc


void set_control_1_register()
{
	uint8_t address = 0x90; // 0x10 with a 1 bit shifted to bit 7 to denote a write = 0x90 = 10010000
	uint8_t data[1] = {0b00010101};
	rtc_write_operation(address, data, 1);
}

void set_control_2_register()
{
	uint8_t address = 0x91; // 0x11 with a 1 bit shifted to bit 7 to allow write = 0x91 = 10010001
	uint8_t data[1] = {0b00011011};
	rtc_write_operation(address, data, 1);
}

void set_status_register()
{
	uint8_t address = 0x9D; // 0x1D |= (1 << 7)
	uint8_t data[1] = {0b00000000};
	rtc_write_operation(address, data, 1);
}

void set_interrupt_mask_register()
{
	uint8_t address = 0x92;// 0x12 |= (1 << 7)
	uint8_t data[1] = {0b01101100}; //0b01101100
	rtc_write_operation(address, data, 1);
}

void set_squarewave_register()
{
	uint8_t address = 0x93; // 0x13 |= (1 << 7)
	uint8_t data[1] = {0b00000000};
	rtc_write_operation(address, data, 1);	
}

void set_osc_control()
{
	// need to write config key with value 0xA1 to change osc control
	uint8_t address = 0x9F; // 0x1F |= (1 << 7)
	uint8_t data[1] = {0xA1};
	rtc_write_operation(address, data, 1);
	address = 0x9C; // 0x1C |= (1 << 7)
	data[1] = 0b01001000;
	rtc_write_operation(address, data, 1);
}

void set_alarm_register()
{
	uint8_t address = 0x98; // 0x18 |= (1 << 7)
	uint8_t data[1] = {0b00010100};
	rtc_write_operation(address, data, 1);
	address = 0x8A; //0x0A |= (1 << 7)
	data[0] = 0b00110000;
	rtc_write_operation(address, data, 1);
}

// combine all initialisation functions and send at once
void initialise_rtc()
{
	set_control_1_register();
	set_control_2_register();
	set_status_register();
	set_interrupt_mask_register();
	set_squarewave_register();
	set_osc_control();
	set_alarm_register();
	//rtc_clear_interrupts();
}

// converts the input time to bcd format and sends to the rtc
void rtc_set_current_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
	uint8_t date_and_time[5] = {0};  // year values must range from 0 to 99
	date_and_time[5] = bin2bcd(year);
	date_and_time[4] = bin2bcd(month);
	date_and_time[3] = bin2bcd(day);
	date_and_time[2] = bin2bcd(hour);
	date_and_time[1] = bin2bcd(minute);
	date_and_time[0] = bin2bcd(second);
	// writes the GP0 bit of the seconds register so this can be checked later to see if the rtc has been initialised
	// and the time has been set. 
	date_and_time[0] |= (1 << 7); 
	rtc_write_operation(0x81, date_and_time, 6);
}

// reads current time on the rtc and compiles into a single array, with all time and date values nicely formatted
void rtc_get_time(unsigned char date_and_time[])
{
	uint8_t date_and_time_bcd[5] = {0};
	char ascii[2] = {0};
	
	rtc_read_operation(0x02, date_and_time_bcd, 5);
	bcd2ascii(date_and_time_bcd[1], ascii);
	date_and_time[1] = ascii[0];
	date_and_time[2] = ascii[1];
	bcd2ascii(date_and_time_bcd[0], ascii);
	date_and_time[4] = ascii[0];
	date_and_time[5] = ascii[1];
	bcd2ascii(date_and_time_bcd[2], ascii);
	date_and_time[7] = ascii[0];
	date_and_time[8] = ascii[1];
	date_and_time[9] = '-';
	bcd2ascii(date_and_time_bcd[3], ascii);
	date_and_time[10] = ascii[0];
	date_and_time[11] = ascii[1];
    bcd2ascii(date_and_time_bcd[4], ascii);
	date_and_time[15] = ascii[0];
	date_and_time[16] = ascii[1];
}

// converts bcd format to two ascii charachters stored in an array
void bcd2ascii(uint8_t byte, uint8_t ascii[2])
{
	// bitshift the bcd tens column along four places and then 
	ascii[0] = (byte >> 4) + 48;
	//convert ones to ascii by converting tens column to zero
	ascii[1] = (byte & 0x0F) + 48;
}

// reads the status register in the rtc to clear all interrupts
void rtc_clear_interrupts()
{
	uint8_t data[1];
	rtc_read_operation(0x0F, data, 1);
}

// this function is unused, it was only written for testing.
void rtc_write_second()
{
	uint8_t data[2] = {0};
	data[0] = 0x82;
	data[1] = 0b00010111;
	gpio_set_pin_level(SS_RTC, false);
	io_write(spi_io, &data, 2);
	gpio_set_pin_level(SS_RTC, true);
}

// reads the seconds register to check if GP0 has been set. does an & comparison and if it is set will output true.
bool check_rtc_initialised()
{
	uint8_t data[1];
	bool result;
	rtc_read_operation(0x01, data, 1);
	uint8_t GP0 = data[0] & 0b10000000;
	if (GP0 == 0b10000000)
	{
		result = true;
	} else {
		result = false;
	}
	return result;
}

void rtc_get_hour(uint8_t current_hour[1])
{
	uint8_t hour_bcd[1];
	rtc_read_operation(0x03, hour_bcd, 1);
	current_hour[0] = bcd2bin(hour_bcd);
}

