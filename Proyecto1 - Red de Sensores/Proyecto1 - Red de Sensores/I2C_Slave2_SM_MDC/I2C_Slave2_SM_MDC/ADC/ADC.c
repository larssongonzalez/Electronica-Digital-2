/*
 * ADC.c
 *
 * Created: 8/10/2024 7:21:17 PM
 *  Author: polst
 */ 

#include "ADC.h"

// Funcion de configuracion del ADC
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

// Funcion para hacer una lectura de un ADC especifico
uint16_t readADCChannel(uint8_t channel){
	// Selecciona canal del ADC
	ADMUX = (ADMUX & 0xF0)|channel;
	// Inicia la conversion
	ADCSRA |= (1<<ADSC);
	// Espera a que la conversión ADC se complete
	while (!(ADCSRA & (1 << ADIF)));
	// Lee el valor analógico del canal seleccionado
	return ADC;
}