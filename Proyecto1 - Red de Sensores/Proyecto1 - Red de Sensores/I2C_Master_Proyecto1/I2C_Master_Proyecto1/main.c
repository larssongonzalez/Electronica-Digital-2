/*
 * I2C_Master_Proyecto1.c
 *
 * Created: 8/10/2024 11:20:59 AM
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
#include "LCD8/LCD8.h"

// Configuración UART
#define UART_BAUD_RATE 9600
#define UART_UBRR (F_CPU / 16 / UART_BAUD_RATE - 1)

#define Slave1 0x20		// Direccion Slave 1 (Sensor Ultrasonico)
#define Slave2 0x22		// Direccion Slave 2 (Sensor de Metales)
#define Slave3 0x48		// Direccion Slave 3 (Sensor de Temperatura LM75B con I2C)
#define Slave4 0x18		// Direccion Slave 4 (ESP32)

#define THRESHOLD 250	// Umbral del sensor de metales

uint8_t dato1, dato2, status;	// Variables de almacenamiento de los Slaves
uint16_t temp;
uint16_t dato3;
char buffer[30];	// Buffer para cadena de caracteres

void uart_send_dato1(uint8_t data);
void uart_send_dato2(uint8_t data);
void uart_send_temp(uint16_t temp);

void uart_init(void) {
	// Configuración de los pines de UART
	DDRD |= (1 << DDD1); // Pin TX como salida
	DDRD &= ~(1 << DDD0); // Pin RX como entrada
	
	UBRR0H = (UART_UBRR >> 8);
	UBRR0L = UART_UBRR;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send_char(uint8_t data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int main(void)
{
	cli();
	
	DDRD = 0xFF;	// Puerto D como Output (LCD)
	DDRB = 0xFF;	// Puerto B como Output
	
	//UCSR0B = 0;		// Usar los pines TX y RX como digitales
	Lcd_Clear();
	Lcd_Init8bits();
	Lcd_Clear();
	I2C_Config_MASTER(4,200000);	// Prescaler = 4 , 200kHz de comunicacion
	
	sei();
	
	uart_init();

	
	while (1)
	{
		_delay_ms(10);
		
		// Menu a mostrar en la LCD
		Lcd_Set_Cursor(0,0);
		Lcd_Write_String("S1: ");
		Lcd_Set_Cursor(0,7);
		Lcd_Write_String("S2: ");
		Lcd_Set_Cursor(0, 13);
		Lcd_Write_String("S3: ");
		
		
		
		dato1 = I2C_read_data(Slave1, &dato1); // Lee dato del Slave 1 y lo almacena en la variable
		dato2 = I2C_read_data(Slave2, &dato2);	// Lee dato del Slave 2 y lo almacena en la variable
		
		// Muestra los valores en LCD del sensor de metales
		Lcd_Set_Cursor(1,8);
		if (dato2<THRESHOLD)
		{
			Lcd_Write_String("1");
			} else {
			Lcd_Write_String("0");
		}
		
		// Muestra los valores en LCD del sensor ultrasonico en cm
		Lcd_Set_Cursor(1,0);
		snprintf(buffer,sizeof(buffer), "%03d", dato1);	// Convierte los datos en una cadena de caracteres
		Lcd_Write_String(buffer);
		
		
		dato3 = I2C_read_data_16bits(Slave3, &dato3);
		temp = dato3/256;
		
		// Muestra la lectura en LCD
		Lcd_Set_Cursor(1,13);
		snprintf(buffer,sizeof(buffer), "%02d", temp);
		Lcd_Write_String(buffer);
		
		// Envia el valor de la temperatura al esclavo
		I2C_esclavo(&temp, Slave2);
		
		// Envía los datos de los sensores al ESP32
		uart_send_dato1(dato1);
		uart_send_dato2(dato2);
		uart_send_temp(temp);

		_delay_ms(10);
	}
}

void uart_send_dato1(uint8_t data) {
	uart_send_char('a');	// Identificador de inicio del dato1 (sensor ultrasonico)
	uart_send_char(data);
}

void uart_send_dato2(uint8_t data) {
	uart_send_char('b'); // Identificador de inicio del dato 2 (sensor de metales)
	uart_send_char(data);
}

void uart_send_temp(uint16_t temp) {
	uart_send_char('T'); // Identificador de inicio de temperatura
	uart_send_char(temp);
}