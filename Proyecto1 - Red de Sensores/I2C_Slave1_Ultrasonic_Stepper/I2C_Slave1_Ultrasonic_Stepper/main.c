/*
 * I2C_Slave1_Ultrasonic_Stepper.c
 *
 * Created: 8/10/2024 11:22:21 AM
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


volatile uint8_t data = 0;
volatile uint16_t distancia = 0, tiempo = 0;


void init_stepper(void);
void stepper_step(int steps, int direction);
void stepper_rotate(int angle);



int main(void)
{
    cli();
	
	DDRD |= (1<<DDD3);	// Trigger como output
	DDRD &= ~(1<<DDD2);	// Echo como input
	
	PORTD &= ~((1<<PORTD2)|(1<<PORTD3));	// Ambos apagados
	
	TCCR1A = 0x00;
	TCCR1B |= (1<<CS11);	// Prescaler de 8
	
	I2C_Config_SLAVE(0x20);		// Configura direccion del Slave
	init_stepper();		// Configura los pines del motor stepper
	
	
	sei();
	
    while (1) 
    {
		// Generamos el pulso del TRIG
		PORTD |= (1<<PORTD3);
		_delay_us(10);
		PORTD &= ~(1<<PORTD3);
		
		// Esperamos la entrada en echo
		while((PIND & (1<<PIND2))==0);
		TCNT1 = 0;
		while(PIND & (1<<PIND2));
		
		// Calculamos la distancia
		tiempo = TCNT1 * 0.5;
		distancia = tiempo/58.3;
		
		
		if (distancia <= 8)	// Distancia menor o igual a 8cm
		{
			stepper_rotate(90);		// Mueve el motor 90 grados en sentido horario
			_delay_ms(5000);	// Espera 5 segundos
			stepper_rotate(-130);	// Regresa el motor stepper a su posicion inicial
		}
		
		// Prueba del motor stepper
		/*stepper_rotate(90);	// gira 90 grados en sentido horario
		_delay_ms(2000);
		stepper_rotate(-180);	// gira 180 grados en sentido antihorario
		_delay_ms(2000);*/
    }
}

/****************************************************************************************
* Interrupciones ISR
****************************************************************************************/
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
			TWDR = distancia; // Carga el dato en el registro de datos
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

/****************************************************************************************
* Funciones
****************************************************************************************/
// Funcion de Inicio del motor stepper
void init_stepper(void){
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3);	// Salida IN1, IN2, IN3, IN4
	//PORTB &= ~((1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3));	// Inicializadas apagadas
}


void stepper_step(int steps, int direction){
	for (int i = 0; i < steps/8; i++)
	{
		if (direction == 1)	// Sentido Horario
		{
			PORTB = (1<<PORTB0);  // Paso 1
			_delay_ms(2);
			PORTB = (1<<PORTB1);  // Paso 2
			_delay_ms(2);
			PORTB = (1<<PORTB2);  // Paso 3
			_delay_ms(2);
			PORTB = (1<<PORTB3);  // Paso 4
			_delay_ms(2);
		} else {	// Sentido Antihorario
			PORTB = (1<<PORTB3);  // Paso 4
			_delay_ms(2);
			PORTB = (1<<PORTB2);  // Paso 3
			_delay_ms(2);
			PORTB = (1<<PORTB1);  // Paso 2
			_delay_ms(2);
			PORTB = (1<<PORTB0);  // Paso 1
			_delay_ms(2);
		}
	}
}


void stepper_rotate(int angle){
	int direction = 1;	// Sentido horario
	if (angle < 0)
	{
		angle = -angle;	// Convierte el angulo negativo en positivo
		direction = 0;
	}
	
	float steps_per_degree = 64 / 5.625; // Calculate steps per degree
	int steps = (int)(angle*steps_per_degree);	// 11.25 es el angulo de paso
	stepper_step(steps,direction);
}