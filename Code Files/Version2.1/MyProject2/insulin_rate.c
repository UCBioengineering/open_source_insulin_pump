/* Module containing functions to set and retreive insulin rate to determine delivery amount
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   15/01/20
 */

#include "stdint.h"
#include "rtc.h"
#include "stdbool.h"
#include "flash.h"

void set_insulin_data(uint8_t insulin_rates[])
{
	// two arrays with 24 elements each, with each element representing hour and the units to be delivered stored
	// in each element
	uint8_t i;
	for(i = 0; i < 48; i++)
	{
		if(i < 24)
		{
			insulin_rates[i] = 20;
			} else if (i == 30) {
			insulin_rates[i] = 7;
			} else if (i == 35) {
			insulin_rates[i] = 7;
			} else if (i == 40){
			insulin_rates[i] = 7;
			} else {
			insulin_rates[i] = 0;	
			}
	}
}

uint8_t get_current_rate(uint8_t insulin_rates[], uint8_t address[])
{
	uint8_t data[48];
	flash_read_operation(address, data, 48);
	uint8_t rate;
	uint8_t current_hour;
	uint8_t date_and_time_bcd[5];
	rtc_read_operation(0x02, date_and_time_bcd, 5);
	current_hour = bcd2bin(date_and_time_bcd[1]);
	rate = (data[current_hour - 1]) + (data[current_hour + 23]);
	return rate;
}


