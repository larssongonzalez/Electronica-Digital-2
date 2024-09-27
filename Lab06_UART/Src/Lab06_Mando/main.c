/*
 * Lab06_Mando.c
 *
 * Created: 9/20/2024 10:52:51 AM
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

/**************************************************************************************
* Librerias Propias
**************************************************************************************/

/**************************************************************************************
*Declaracion de Variables DEFINE
**************************************************************************************/
#define UART_BAUD_RATE 9600
#define UART_UBRR_VALUE ((F_CPU / (UART_BAUD_RATE * 16UL)) - 1)

/**************************************************************************************
* Declaracion de Variables
**************************************************************************************/

/**************************************************************************************
* Declaracion de Funciones
**************************************************************************************/
void inituart(void);	// Funcion de inicio de UART
void uart_puts(char *str);	// Funcion de envio de string por UART
void uart_putc(char c);	// Funcion de envio de caracter por UART

int main(void)
{
	 cli();
	 
    // Push como inputs
	DDRD &= ~(1<<DDD7);		// PD7 how input
	PORTD |= (1<<PORTD7);	// Active Pull Up
	DDRB &= ~((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)|(1<<DDB4));
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4);	// Active Pull Up
	
	// Habilitar interrupciones de PCINT para el puerto B (PB0 a PB4)
	PCICR |= (1 << PCIE0);     // Habilitar el grupo de interrupciones PCINT0 para el puerto B
	PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4); // Habilitar los pines PB0 a PB4

	// Habilitar interrupciones de PCINT para el puerto D (PD7)
	PCICR |= (1 << PCIE2);     // Habilitar el grupo de interrupciones PCINT2 para el puerto D
	PCMSK2 |= (1 << PCINT23);  // Habilitar el pin PD7
	
	inituart();
	
	sei();
	
    while (1) 
    {
    }
}

/**************************************************************************************
* Funciones
**************************************************************************************/
// Funcion de inicio de UART
void inituart(void){
	// Configuración de los pines de UART
	DDRD |= (1 << DDD1);  // PD1 como salida (TX)
	DDRD &= ~(1 << DDD0);  // PD0 como entrada (RX)

	// Configuración del registro UBRR
	UBRR0H = (UART_UBRR_VALUE >> 8);
	UBRR0L = UART_UBRR_VALUE;

	// Configurar el formato de los datos: 8 bits, sin paridad, 1 bit de stop
	UCSR0C = 0;
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	// Configuración del registro UCSR0B
	UCSR0B = 0;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Habilitar RX y TX
}

// Funcion de envio de string por UART
void uart_puts(char *str){
	uint8_t i;
	for (i = 0; str[i] != '\0'; i++)
	{
		while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = str[i];
	}
}

// Funcion de envio de caracter por UART
void uart_putc(char c){
	while (!(UCSR0A & (1 << UDRE0)));  // Esperar a que el buffer esté vacío
	UDR0 = c;  // Enviar el carácter
}



/**************************************************************************************
* Interrupciones (ISR)
**************************************************************************************/
ISR(PCINT0_vect){
	_delay_ms(50);
	if (!(PINB & (1<<PINB0)))	// Verifica si se presiono PB0
	{
		uart_putc('a');		// Envia el comando 'a'
	} else if (!(PINB & (1<<PINB1)))	// Verifica si se presiono PB1
	{
		uart_putc('b');		// Envia el comando 'b'
	} else if (!(PINB & (1<<PINB2)))	// Verifica si se presiono PB2
	{
		uart_putc('c');		// Envia el comando 'c'
	} else if (!(PINB & (1<<PINB3)))	// Verifica si se presiono PB3
	{
		uart_putc('d');		// Envia el comando 'd'
	} else if (!(PINB & (1<<PINB4)))	// Verifica si se presiono PB4
	{
		uart_putc('e');		// Envia el comando 'e'
	}
}

ISR(PCINT2_vect){
	_delay_ms(50);
	if (!(PIND & (1<<PIND7)))	// Verifica si se presiono PD7
	{
		uart_putc('f');		//Envia el comando 'f'
	}
}


