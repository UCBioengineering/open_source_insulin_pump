/* Module containing functions to interface with the eink display
   written by Matt Payne for UC summer scholarship as part of Geoff Chases Insulin pump project
   17/12/19
 */

#include "stdbool.h"
#include "eink.h"
#include "atmel_start_pins.h"
#include "eink_luts_and_buf.h"
#include "hal_io.h"
#include "hal_delay.h"
#include "string.h"


extern struct io_descriptor *spi_io;

// drive enable pin low to power up e-ink screen (e-ink power is active low)
void eink_enable()
{
	gpio_set_pin_level(EINK_EN, false); 
}

// drive enable pin high to power down e-ink screen (e-ink power is active low)
void eink_disable()
{
	gpio_set_pin_level(EINK_EN, true); 
}


// reset driver IC by pulling Reset_Eink pin high, low, high
void reset_driver_ic() 
{
	gpio_set_pin_level(RES, true);
	delay_ms(1);
	gpio_set_pin_level(RES, false);
	delay_ms(1);
	gpio_set_pin_level(RES, true);
}

// set slave select for transfer to e-ink
void eink_SS_on()
{
	gpio_set_pin_level(SS_EINK, false); // SS is active low
}

// deactivate ss for e-ink
void eink_SS_off()
{
	gpio_set_pin_level(SS_EINK, true);
}

// set D/C pin low for command transfer
void eink_send_command()
{
	gpio_set_pin_level(DC_EINK, false);
}

// set D/C command high for a data transfer
void eink_send_data()
{
	gpio_set_pin_level(DC_EINK, true);
}

// setup booster softstart as per datasheet p23/24
void eink_booster_settings()
{
	uint8_t command = BOOSTER_SOFT_START;
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_send_data();
	uint8_t data[3] = {0x17, 0x17, 0x17};
	io_write(spi_io, data, 3);
	eink_SS_off();
}

// Setup power on as per datasheet p23/24
void eink_power_on()
{
	uint8_t command = POWER_ON;
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_SS_off();
}

// check if the e-ink display is outputting that its busy on the Busy_Eink pin, returns 1 if busy and 0 if not
bool check_eink_busy()
{
	return gpio_get_pin_level(BUSY);
}

// set panel settings to default using 0x0F as per datasheet p25
void eink_panel_settings()
{
	uint8_t settings = 0x0F;
	uint8_t command = PANEL_SETTINGS;
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_send_data();
	io_write(spi_io, &settings, 1);
	eink_SS_off();	
	
}

// set resolution settings to default as per datasheet p23/37
void eink_resolution_settings()
{
	uint8_t command = RESOLUTION_SETTINGS;
	uint8_t settings[3] = {0x98, 0x00, 0x98};
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_send_data();
	io_write(spi_io, settings, 3);
	eink_SS_off();
}

// Set vcom and data interval settings to default as per datasheet
void eink_vcom_data_interval_settings()
{
	uint8_t command = VCOM_DATA_INTERVAL_SETTINGS;
	uint8_t settings  = 0x87;
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_send_data();
	io_write(spi_io, &settings, 1);
	eink_SS_off();
}

// join initialisation functions together to be called all at once to start the eink
void eink_initialise() 
{
	eink_enable();
	eink_SS_on();
	reset_driver_ic();
	initialise_buffer();
	clear_buffer();
	eink_booster_settings();
	eink_power_on();
	
	delay_ms(1);
	// checks the status of the busy pin and keeps delaying for 1us until the e-ink is ready to proceed
	// with setup
	while(check_eink_busy())
	{
	    delay_ms(1);
	}
	
	eink_panel_settings();
	eink_resolution_settings();
	eink_vcom_data_interval_settings();	
}

void display_text(unsigned char** text, uint8_t x, uint8_t y)
{
	
	uint8_t i;
	uint8_t text_rows = (strlen(text) / 19) + 1;
	uint8_t command = 0x13;
	
	eink_initialise();
	
	char split_string[text_rows][20];
	split_string_to_rows(text, split_string, text_rows);
	for (i = 0; i < text_rows; i++)
	{
		fill_screen_buffer(split_string[i], x, y + (i * CHAR_HEIGHT));
	}
	
	eink_SS_on();
	eink_send_command();
	io_write(spi_io, &command, 1);
	eink_send_data();
	send_screen_buffer();
	command = 0x11;
	eink_send_command();
	io_write(spi_io, &command, 1);
	command = 0x12;
	io_write(spi_io, &command, 1);
	eink_SS_off();
	eink_disable();
	
}




