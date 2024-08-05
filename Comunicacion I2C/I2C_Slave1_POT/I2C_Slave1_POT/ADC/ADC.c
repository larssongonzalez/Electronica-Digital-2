/*
 * ADC.c
 *
 * Created: 8/4/2024 11:23:18 AM
 *  Author: polst
 */ 

#include "ADC.h"


// Funcion de inicio de ADC
void initADC(void){
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