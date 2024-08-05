/*
 * Lab03_SPI_Slave.c
 *
 * Created: 7/25/2024 9:15:59 PM
 * Author : polst
 */ 
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "SPI_Slave/SPI_Slave.h"



/*******************************************************************************
* Declaration de Variables
*******************************************************************************/
volatile uint8_t adcValue1 = 0;
volatile uint8_t adcValue2 = 0;
volatile uint8_t caso = 0;

/*******************************************************************************
* Declaration de Functions
*******************************************************************************/
void initADC();


/*******************************************************************************
* Function Principal Program
*******************************************************************************/
int main(void)
{
    cli();
	
	DDRC = 0;// Puerto C como entrada
	
	initSPI(SPI_Slave_SS, SPI_DATA_MSB, SPI_CLOCK_LOW, SPI_CLOCK_FIRST_EDGE);
	initADC();
	SPCR |= (1<<SPIE);		// Activar ISR SPI
	//ADCSRA |= (1<<ADSC); // Iniciar la primera conversión ADC
	
	sei();
	
	
    while (1) 
    {
		ADCSRA |= (1<<ADSC); // Iniciar la primera conversión ADC
		_delay_ms(100);
		
    }
}

/*******************************************************************************
* Interrupts (ISR)
*******************************************************************************/
ISR(ADC_vect){
	if (caso == 0) {
		ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));	// Selecciona el ADC0
		adcValue1 = ADC;	// guarda el valalor del ADC en la variable
		caso = 1;
	} else {
		ADMUX = (ADMUX & ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | (1 << MUX0);		// Selecciona el ADC1
		adcValue2 = ADC;	// guarda el valalor del ADC en la variable
		caso = 0;
	}
	ADCSRA |= (1 << ADSC);  //Inicia la conversion del ADC
}

ISR(SPI_STC_vect){
	// Lee el valor recibido por SPI
	uint8_t spivalor = SPDR;
	
	 // Comprobar el valor recibido y enviar el valor del ADC correspondiente
	if (spivalor == 'c')
	{
		writeSPI(adcValue1);
		//_delay_ms(5);
	}
	if (spivalor == 'd')
	{
		writeSPI(adcValue2);
		//_delay_ms(5);
	}
}
/*******************************************************************************
* Functions
*******************************************************************************/
void initADC(){
	ADMUX = 0;
	
	//Vref = AVCC = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	// Justificacion hacia la izquierda
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	// Enciende ADC
	ADCSRA |= (1<<ADEN);
	
	// Habilitamos ISR ADC
	ADCSRA |= (1<<ADIE);
	
	//Prescaler de 128 > 16M/128 = 125kHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	// Deshabilitando la entrada digital PC0, PC1
	DIDR0 |= (1<<ADC0D);
	DIDR1 |= (1<<ADC1D);
}

