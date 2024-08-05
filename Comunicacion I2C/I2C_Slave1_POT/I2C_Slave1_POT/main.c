/*
 * I2C_Slave1_POT.c
 *
 * Created: 8/4/2024 11:19:58 AM
 * Author : polst
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "ADC/ADC.h"
#include "I2C/I2C.h"

/**************************************************************************************
* Declaracion de Variables
**************************************************************************************/
volatile uint8_t adcval = 0, data;



/**************************************************************************************
* Declaracion de Funciones
**************************************************************************************/




int main(void)
{
	cli();
	
	DDRC = 0;	// Puerto C como entrada
	initADC();
	I2C_Config_SLAVE(0x20);
	
	sei();
    
    while (1) 
    {
		_delay_ms(10);
		ADCSRA |= (1<<ADSC);	// Inicia la conversion del ADC
    }
}

/**************************************************************************************
* Interrupciones (ISR)
**************************************************************************************/
ISR(ADC_vect){
	adcval = ADCH;	// Lectura del valor del pot
	ADCSRA |= (1<<ADIF);	//Borra la bandera de interrupcion
}


ISR(TWI_vect){
	uint8_t state;
	
	// Lee el estado del bus de datos
	state = TWSR & 0xFC;	
	
	switch(state){
		case 0x60:
		case 0x70: 
			TWCR |= (1 << TWINT);
			break;
		case 0x80:
		case 0x90:
			data = TWDR;  // Recibió el dato, llamada general
			TWCR |= (1 << TWINT); 
			break;
		case 0xA8: // SLA+R recibido, Master mode Lecture 
		case 0xB8: // Dato transmitido y ACK recibido
			TWDR = adcval; // Carga el dato en el registro de datos
			TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE) | (1 << TWEA); // Listo para la próxima operación
			break;
		case 0xC0: // Dato transmitido y NACK recibido
		case 0xC8: // Último dato transmitido y ACK recibido
			TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA); 
			break;
		default: // Libera el BUS de datos
			TWCR |= (1 << TWINT) | (1 << TWSTO);
			break;
	}
	
	
}



