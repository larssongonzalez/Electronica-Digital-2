/*
 * SPI_Slave.c
 *
 * Created: 7/26/2024 8:18:39 AM
 *  Author: polst
 */ 

#include "SPI_Slave.h"

void initSPI(SPI_type sType, SPI_Data_Order sDataOrder, SPI_Clock_Polarity sClockPolarity, SPI_Clock_Phase sClockPhase){
	// PB2 -> SS(Slave Select)
	// PB3 -> MOSI
	// PB4 -> MISO
	// PB5 -> SCK
	
	if (sType & (1<<MSTR))	// If Master Mode
	{
		DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<DDB2);	// MOSI, SCK, SS
		DDRB &= ~(1<<DDB4);		// MISO
		SPCR |= (1<<MSTR);		// Master
		
		uint8_t temp = sType & 0b00000111;
		switch(temp){
			case 0:	//DIV 2
			SPCR &= ~((1<<SPR1)|(1<<SPR0));
			SPSR |= (1<<SPI2X);
			break;
			case 1:	//DIV 4
			SPCR &= ~((1<<SPR1)|(1<<SPR0));
			SPSR &= ~(1<<SPI2X);
			break;
			case 2:	//DIV 8
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			SPSR |= (1<<SPI2X);
			break;
			case 3:	//DIV 16
			SPCR |= (1<<SPR0);
			SPCR &= ~(1<<SPR1);
			SPSR &= ~(1<<SPI2X);
			break;
			case 4:	//DIV 32
			SPCR &= ~(1<<SPR0);
			SPCR |= (1<<SPR1);
			SPSR |= (1<<SPI2X);
			break;
			case 5:	//DIV 64
			SPCR &= ~(1<<SPR0);
			SPCR |= (1<<SPR1);
			SPSR &= ~(1<<SPI2X);
			break;
			case 6:	//DIV 128
			SPCR |= (1<<SPR0);
			SPCR |= (1<<SPR1);
			SPSR &= ~(1<<SPI2X);
			break;
		}
	}
	else  // If Slave Mode
	{
		DDRB |= (1<<DDB4);	//MISO
		DDRB &= ~((1<<DDB3)|(1<<DDB5)|(1<<DDB2));	// MOSI, SCK, SS
		SPCR &= ~(1<<MSTR);	// Slave
	}
	
	/* Enable SPI, Data Order, Clock Polarity, Clock Phase*/
	SPCR |= (1<<SPE)|sDataOrder|sClockPolarity|sClockPhase;
	
}

void writeSPI(uint8_t data){
	SPDR = data;
}

unsigned readydataSPI(){
	if (SPSR & (1<<SPIF))
	{
		return 1;
		} else {
		return 0;
	}
}

uint8_t readSPI(){		// Read the received data
	while(!(SPSR & (1<<SPIF)));		// Wait for data receive complete
	return (SPDR);		// Read the received data from buffer
}