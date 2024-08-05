/*
 * I2C_Slave2_Buttons.c
 *
 * Created: 8/4/2024 11:43:03 AM
 * Author : polst
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "I2C/I2C.h"

/**************************************************************************************
* Declaracion de Variables
**************************************************************************************/
uint8_t contador = 0, data;



/**************************************************************************************
* Declaracion de Funciones
**************************************************************************************/



int main(void)
{
    cli();
	
	DDRB &= ~((1<<DDB0)|(1<<DDB1));	// PB0 and PB1 how input
	PORTB |= ((1<<PORTB0)|(1<<PORTB1));	// we activate Pull-Up
	
	DDRD |= (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3);	// LEDS outputs
	PORTD &= ~((1<<PORTD0)|(1<<PORTD1)|(1<<PORTD2)|(1<<PORTD3));	// LEDS off
	
	UCSR0B = 0;  //Usar los pines TX y RX como digitales
	
	// Active ISR PCINT0
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1);	// Active mask ISR the PCINT0 and PCINT1
	PCICR |= (1<<PCIE0);	// Active vector ISR the PCINT0
	
	I2C_Config_SLAVE(0x22); // Init I2C Slave2 and direction
	
	sei();
	
    while (1) 
    {
		_delay_ms(10);
    }
}

/**************************************************************************************
* Interrupciones (ISR)
**************************************************************************************/
ISR(PCINT0_vect){
	
	// Revisa el estado del boton PB0 (Incrementa)
	if (!(PINB & (1<<PINB0)) == 0)
	{
		
		contador++;
		if (contador >= 15)	// Delimita el contador a 15
		{
			contador = 15;
		}
		
		PORTD = contador;
	}
	
	// Revisa el estado del boton PB1 (Decrementa)
	if (!(PINB & (1<<PINB1)) == 0)
	{
		
		contador--;
		if (contador <= 0)
		{
			contador = 0;
		}
		
		PORTD = contador;
	}
}

ISR(TWI_vect){
	uint8_t estado;
	
	estado = TWSR & 0xFC;
	
	switch(estado){
		case 0x60:
		case 0x70:              // Direccion del Slave2
			TWCR |= (1 << TWINT); 
			break;
		case 0x80:
		case 0x90:
			data = TWDR;  // Recibió el dato, llamada general
			TWCR |= 1 << TWINT; 
			break;
		case 0xA8: // SLA+R recibido, Master mode Lecture 
		case 0xB8: // Dato transmitido y ACK recibido
			TWDR = contador; // Carga el dato en el registro de datos
			TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWIE)| (1 << TWEA); 
			break;
		case 0xC0: // Dato transmitido y NACK recibido
		case 0xC8: // Último dato transmitido y ACK recibido
			TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA); 
			break;
		default:    // Libera el BUS de datos
			TWCR |= (1 << TWINT) | (1 << TWSTO);
			break;
	}
}

