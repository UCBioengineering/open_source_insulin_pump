/* Module containing functions to interface Buttons, and also set up three external interrupts
   to those buttons.
   Code written by Matt Payne for University of Canterbury Final Year Project as part the Insulin pump project,
   Project supervised by Geoff Chase, Jennifer Knopp, Lui Holder Pearson and Jake Campbell.
   22/08/20
 */

#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
	#endif

// Initialise interrupts
void init_interrupts(void);

void TIMER_0_setup(void);

void init_button_array(void);

void set_leds(uint8_t button_count);

void leds_off(void);

	
#ifdef __cplusplus
}
#endif
#endif