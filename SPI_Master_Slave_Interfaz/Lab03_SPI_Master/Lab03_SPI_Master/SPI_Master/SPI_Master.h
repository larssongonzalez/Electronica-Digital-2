/*
 * SPI_Master.h
 *
 * Created: 7/25/2024 9:28:13 PM
 *  Author: polst
 */ 


#ifndef SPI_MASTER_H_
#define SPI_MASTER_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>

//Opciones del Oscilador de SPI
typedef enum{
	SPI_MASTER_OSC_DIV2   = 0b01010000,
	SPI_MASTER_OSC_DIV4   = 0b01010001,
	SPI_MASTER_OSC_DIV8   = 0b01010010,
	SPI_MASTER_OSC_DIV16  = 0b01010011,
	SPI_MASTER_OSC_DIV32  = 0b01010100,
	SPI_MASTER_OSC_DIV64  = 0b01010101,
	SPI_MASTER_OSC_DIV128 = 0b01010110,
	SPI_Slave_SS          = 0b01000000
	} SPI_type;

//Opciones para saber si trabajo con bits mas significativos o menos significativos
typedef enum{
	SPI_DATA_MSB = 0b00000000,
	SPI_DATA_LSB = 0b00100000
	} SPI_Data_Order;

//Opciones para polaridad en alto o bajo	
typedef enum{
	SPI_CLOCK_HIGH = 0b00001000,
	SPI_CLOCK_LOW  = 0b00000000
	} SPI_Clock_Polarity;

//Opciones de la fase del clock	
typedef enum{
	SPI_CLOCK_FIRST_EDGE = 0b00000000,
	SPI_CLOCK_LAST_EDGE  = 0b00000100,
	} SPI_Clock_Phase;

void initSPI(SPI_type, SPI_Data_Order, SPI_Clock_Polarity, SPI_Clock_Phase);
void writeSPI(uint8_t data);
unsigned readydataSPI();
uint8_t readSPI();

#endif /* SPI_MASTER_H_ */