/* Module containing .h module to interface with the flash memory.
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   08/01/20
 */

#ifndef FLASH_H_INCLUDED
#define FLASH_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void flash_write_operation(uint8_t address, uint8_t data[], uint8_t data_length);
void flash_read_operation(uint8_t address[], uint8_t data[], uint8_t data_length);
void flash_set_status_register();
void flash_init();
void set_write_enable();
void flash_page_program_operation(uint8_t data[], uint8_t address[], uint16_t data_length);
void set_insulin_and_addr_data();
void flash_chip_erase();
bool flash_check_write_in_progress();
void read_status_register(uint8_t data[]);
void get_units(unsigned char date_and_time[20], uint8_t units);
void flash_sector_erase();
void log_time_and_insulin(uint8_t current_rate);
void store_starting_address();
void convert_number_to_bytes(uint32_t number_formatted[], uint8_t number);
uint32_t convert_bytes_to_number(uint8_t number_formatted[]);

#ifdef __cplusplus
}
#endif
#endif