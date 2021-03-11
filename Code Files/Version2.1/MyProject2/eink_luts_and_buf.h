#ifndef EINK_LUTS_AND_BUF_INCLUDED
#define EINK_LUTS_AND_BUF_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "eink.h"
#include "stdio.h"
#include "stdlib.h"

#define SCREEN_WIDTH 152
#define SCREEN_HEIGHT 152
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 8
#define ASCII_TO_LUT_OFFSET 32
#define BYTES_WIDTH 19
extern const unsigned char small_font[60][8];

void initialise_buffer();
void clear_buffer();
uint8_t calculate_rows(char* text);
void split_string_to_rows(unsigned char* string, char row_array[][20], uint8_t rows);
void get_font_index(char* text, uint8_t* font_index_array);
void rotate_character(const unsigned char* character, unsigned char* rotated_character);
void fill_screen_buffer(char* string, uint8_t x, uint8_t y);
void send_screen_buffer();


#ifdef __cplusplus
}
#endif
#endif