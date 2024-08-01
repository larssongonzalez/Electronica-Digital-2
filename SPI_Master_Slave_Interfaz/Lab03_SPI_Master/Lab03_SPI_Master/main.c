/*
 * Lab03_SPI_Master.c
 *
 * Created: 7/25/2024 9:15:16 PM
 * Author : polst
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include "SPI_Master/SPI_Master.h"
#include "UART/UART.h"

/*******************************************************************************
* Declaration de Variables
*******************************************************************************/
volatile uint8_t contador = 0;
char buffer[16];
uint8_t valorspi1 = 0;
uint8_t valorspi2 = 0;
uint8_t valorpot1 = 0;
uint8_t valorpot2 = 0;


/*******************************************************************************
* Declaration de Functions
*******************************************************************************/
void refreshPORT(uint8_t valor);
void menuUART(void);
/*******************************************************************************
* Function Principal Program
*******************************************************************************/
int main(void)
{
	
	cli();
	
    // Leds como outputs
	DDRC |= (1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3)|(1<<DDC4)|(1<<DDC5);
	DDRD |= (1<<DDD2)|(1<<DDD3);
	
	PORTC &= ~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3)|(1<<PORTC4)|(1<<PORTC5));
	DDRD &= ~((1<<PORTD2)|(1<<PORTD3));
	
	// Init SPI
	initSPI(SPI_MASTER_OSC_DIV16,SPI_DATA_MSB, SPI_CLOCK_LOW, SPI_CLOCK_FIRST_EDGE);
	
	// Iniciar UART
	initUART();
	
	menuUART();
	
	sei();
	
    while (1) 
    {
		PORTB &= ~(1<<PORTB2);	// Slave select low
		writeSPI('c');	// Comando para valor de pot 1
		valorspi1 = readSPI();
		_delay_ms(20);
		PORTB |= (1<<PORTB2);	//Slave select high
		
		PORTB &= ~(1<<PORTB2);
		writeSPI('d');	// Comando para valor de pot 2
		valorspi2 = readSPI();
		_delay_ms(20);
		PORTB |= (1<<PORTB2);	// Slave select high
		
		
		 
		 
    }
}

/*******************************************************************************
* Interrupts (ISR)
*******************************************************************************/
ISR(USART_RX_vect)
{
	static char lastCommand = '\0';  // Variable para almacenar el último comando recibido
	char receivedChar = UDR0;         // Leer el carácter recibido

	// Manejar el comando de visualización de potenciómetro
	if (receivedChar == '1')
	{
		valorpot1 = valorspi1;//5;
		valorpot2 = valorspi2;//10;
		snprintf(buffer, sizeof(buffer), "POT1: %d", valorpot1);
		stringUART(buffer);
		_delay_ms(5);
		writeUART(10);
		writeUART(13);
		snprintf(buffer, sizeof(buffer), "POT2: %d", valorpot2);
		stringUART(buffer);
		_delay_ms(5);
		writeUART(10);
		writeUART(13);
		//lastCommand = '\0';  // Resetear el último comando
	}
	// Manejar el comando de cambio de contador
	else if (receivedChar == '2')
	{
		stringUART("Contador de Incremento/Decremento");
		writeUART(10);
		writeUART(13);
		lastCommand = '2';  // Guardar el comando de cambio de contador
	}
	// Manejar los caracteres '+' y '-' si el comando anterior fue '2'
	else if ((receivedChar == '+' || receivedChar == '-') && lastCommand == '2')
	{
		if (receivedChar == '+') {
			if (contador < 255) {
				contador++;
			}
		}
		else if (receivedChar == '-') {
			if (contador > 0) {
				contador--;
			}
		}

		refreshPORT(contador);  // Actualizar los LEDs según el valor del contador
		
	}
	
	// Nuevo comando para enviar los valores de los potenciómetros
	else if (receivedChar == '1')
	{
		valorpot1 = valorspi1;
		valorpot2 = valorspi2;
		snprintf(buffer, sizeof(buffer), "P1:%d", valorpot1);
		stringUART(buffer);
		_delay_ms(5);
		writeUART(10);
		writeUART(13);
		snprintf(buffer, sizeof(buffer), "P2:%d", valorpot2);
		stringUART(buffer);
		_delay_ms(5);
		writeUART(10);
		writeUART(13);
	}
	
}

/*******************************************************************************
* Functions
*******************************************************************************/
void refreshPORT(uint8_t valor){
	if (valor & 0b10000000)
	{
		PORTD |= (1<<PORTD3);
	} else {
		PORTD &= ~(1<<PORTD3);
	}
	if (valor & 0b01000000)
	{
		PORTD |= (1<<PORTD2);
	} else {
		PORTD &= ~(1<<PORTD2);
	}
	if (valor & 0b00100000)
	{
		PORTC |= (1<<PORTC5);
	} else {
		PORTC &= ~(1<<PORTC5);
	}
	if (valor & 0b00010000)
	{
		PORTC |= (1<<PORTC4);
	} else {
		PORTC &= ~(1<<PORTC4);
	}
	if (valor & 0b00001000)
	{
		PORTC |= (1<<PORTC3);
	} else {
		PORTC &= ~(1<<PORTC3);
	}
	if (valor & 0b00000100)
	{
		PORTC |= (1<<PORTC2);
	} else {
		PORTC &= ~(1<<PORTC2);
	}
	if (valor & 0b00000010)
	{
		PORTC |= (1<<PORTC1);
	} else {
		PORTC &= ~(1<<PORTC1);
	}
	if (valor & 0b00000001)
	{
		PORTC |= (1<<PORTC0);
	} else {
		PORTC &= ~(1<<PORTC0);
	}
}

void menuUART(void){
	// Muestra el mensaje de bienvenida
	stringUART("Elige una opcion:");
	writeUART(10);
	writeUART(13);
	writeUART(10);
	stringUART("1. Leer Potenciometros");
	writeUART(10);
	writeUART(13);
	writeUART(10);
	stringUART("2. Incremento/Decremento Contador");
	writeUART(10);
	writeUART(13);
	writeUART(10);
}