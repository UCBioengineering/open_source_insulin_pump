/*  **** Module written by Matt Payne as part of UC Summer Scholarship 2019 - 2020 for the insulin pump project.
    **** Started on 26/11/2019
    **** This is the .h file for the motor functions defined in motor.c
*/

#ifndef MOTOR_H_INCLUDED
#define MOTOR_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <stdbool.h>

bool read_motor_fault_pin();
void set_motor_direction(bool direction);
void step_motor(bool direction);
void turn_x_steps(bool direction, uint16_t steps_to_turn);
void motor_return_to_zero();

#ifdef _cplusplus
}
#endif
#endif
