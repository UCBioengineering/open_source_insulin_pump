/* Module containing functions for SPI 
   Code written by Max van Empel unless stated otherwise
   26/11/2019
   */

#include <spi.h>
#include <hpl_spi.h>
#include <driver_init.h>


extern struct io_descriptor *spi_io;

void SPI_Begin(void){
	spi_m_sync_get_io_descriptor(&SPI_0, &spi_io);
	spi_m_sync_set_baudrate(&SPI_0, 100000);
	spi_m_sync_set_char_size(&SPI_0, SPI_CHAR_SIZE_8);
	spi_m_sync_enable(&SPI_0);
}

void SPI_End(void){
	spi_m_sync_disable(&SPI_0);
}

