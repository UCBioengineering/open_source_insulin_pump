/* Module containing .h file for eink.c
   written by Matt Payne for UC summer scholarship as part of Geoff Chases Insulin pump project
   17/12/19
 */

#ifndef EINK_H_INCLUDED
#define EINK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

#define PANEL_SETTINGS 0x00
#define POWER_ON 0x04
#define BOOSTER_SOFT_START 0x06
#define VCOM_DATA_INTERVAL_SETTINGS 0X50
#define RESOLUTION_SETTINGS 0x61

extern struct io_descriptor *spi_io;

void eink_enable();
void eink_disable();
void reset_driver_ic();
void eink_booster_settings();
void eink_SS_on();
void eink_SS_off();
void eink_send_command();
void eink_send_data();
void eink_power_on();
bool check_eink_busy();
void eink_resolution_settings();
void eink_vcom_data_interval_settings();
void display_text(unsigned char** text, uint8_t x, uint8_t y);
void eink_initialise();

#ifdef __cplusplus
}
#endif
#endif
	
	