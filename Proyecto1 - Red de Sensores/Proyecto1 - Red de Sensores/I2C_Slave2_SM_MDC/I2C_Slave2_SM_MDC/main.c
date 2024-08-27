/*
 * I2C_Slave2_SM_MDC.c
 *
 * Created: 8/10/2024 11:23:39 AM
 * Author : polst
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/twi.h>

#include "I2C/I2C.h"
#include "ADC/ADC.h"

#define THRESHOLD 250	// Umbral del sensor de metales

volatile uint8_t adcvalor = 0, data, temperatura;
uint8_t valordigital = 0;


int main(void)
{
    cli();
	
	DDRC = 0;	// Puerto C como entrada
	DDRD |= (1<<DDD2);	// Buzzer como output
	PORTD &= ~(1<<PORTD2);	// Buzzer apagado
	
	DDRB |= (1<<DDB0);	// Ventilador como output
	PORTB &= ~(1<<PORTB0);	// Ventilador apagado
	
	initADC();
	I2C_Config_SLAVE(0x22);		// Direccion del Slave 2 (Sensor de Metales)
	
	sei();
	
    while (1) 
    {
		_delay_ms(10);
		ADCSRA |= (1<<ADSC);	// inicia la conversion
		
		// Detecta metal activa un buzzer
		if (adcvalor < THRESHOLD)
		{
			PORTD |= (1<<PORTD2);
		} else {
			PORTD &= ~(1<<PORTD2);
		}
		
		// Temperatura >= 28, activa un ventilador
		if (temperatura >= 28)
		{
			PORTB |= (1<<PORTB0);	// Enciende ventilador
		} else{
			PORTB &= ~(1<<PORTB0);	// Apaga ventilador
		}
    }
}

/************************************************************************/
/* Interrupciones ISR                                                   */                  
/************************************************************************/
ISR(ADC_vect){
	adcvalor = ADCH;	// Lectura del valor del sensor de metales
	//valordigital = (adcvalor > 200) ? 1 : 0; // Actualiza valordigital en tiempo real
	ADCSRA |= (1<<ADIF);
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
			temperatura = TWDR;  // Almacena la temperatura recibida
			TWCR |= (1 << TWINT);
			break;
		/*case 0x90:	// Recibe datos del Master
			temperatura = TWDR;  // Almacena la temperatura recibida
			TWCR |= (1 << TWINT);
			break;*/
		case 0xA8: // SLA+R recibido, Master mode Lecture
		case 0xB8: // Dato transmitido y ACK recibido
			TWDR = adcvalor; // Carga el dato en el registro de datos
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