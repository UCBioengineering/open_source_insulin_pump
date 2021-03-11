/*  **** Module written by Matt Payne as part of UC Summer Scholarship 2019 - 2020 for the insulin pump project.
    **** This module forms part of the insulin pump code originally authored by Max van Empel.
    **** Started on 26/11/2019
    **** This module forms the .h files for the functions to use the SPI code already written by Max van Empel, that was previously in main.c 
*/

#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

void SPI_Begin(void);

void SPI_End(void);

#ifdef _cplusplus
}
#endif
#endif