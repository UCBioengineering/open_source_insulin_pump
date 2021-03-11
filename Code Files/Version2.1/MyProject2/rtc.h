/* Module containing .h module to interface with the real time clock.
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   08/01/20
 */

#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"

void rtc_write_operation(uint8_t address, uint8_t data[], uint8_t data_length);
void rtc_read_operation(uint8_t address, uint8_t data[], uint8_t data_length);
void set_control_1_register();
void set_control_2_register();
void set_status_register();
void set_interrupt_mask_register();
void set_squarewave_register();
void set_osc_control();
void set_alarm_register();
void initialise_rtc();
void rtc_set_current_time(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
void bcd2ascii(uint8_t byte, uint8_t ascii[2]);
void rtc_get_time(unsigned char date_and_time[40]);
void rtc_clear_interrupts();
bool check_rtc_initialised();
void rtc_get_hour(uint8_t current_hour[1]);
uint8_t bcd2bin(uint8_t value);
uint8_t bin2bcd(uint8_t value);



#ifdef __cplusplus
}
#endif
#endif