/* .h module containing functions to set and retreive insulin rate to determine delivery amount
   Code written by Matt Payne for UC summer scholarship as part the Insulin pump project,
   15/01/20
 */

#ifndef	INSULIN_RATE_H_INCLUDED
#define INSULIN_RATE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void set_insulin_data(uint8_t insulin_rates[]);
uint8_t get_current_rate(uint8_t insulin_rates[], uint8_t address[]);

#ifdef __cplusplus
}
#endif
#endif