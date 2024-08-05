/*
 * I2C_Master.c
 *
 * Created: 8/3/2024 1:18:21 PM
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

#include "I2C/I2C.h"	// Libreria I2C
#include "LCD8/LCD8.h"	// Libreria LCD 8 bits

#define Slave1 0x20		// Direccion Slave1 (Potenciometro)
#define Slave2 0x22		// Direccion Slave2 (Contador)

/**************************************************************************************
* Declaracion de Variables
**************************************************************************************/
uint8_t dato1, dato2;	// Variables de almacenamiento de los Slaves
char buffer[16];	// Buffer para cadena de caracteres 


/**************************************************************************************
* Declaracion de Funciones
**************************************************************************************/



int main(void)
{
	cli();
	
	DDRD = 0xFF;	// Puerto D como salida (LCD)
	DDRB = 0xFF;	// Puerto B como salida
	
	UCSR0B = 0;  // Usar los pines TX y RX como digitales
	
	Lcd_Init8bits();
	Lcd_Clear();
	I2C_Config_MASTER(4,200000);	// Prescaler = 4 , 200kHz de comunicacion
	
	
	sei();	
    
    while (1) 
    {
		_delay_ms(10);
		
		// Menu de la LCD
		Lcd_Set_Cursor(0, 3);
		Lcd_Write_String("S1: ");
		Lcd_Set_Cursor(0, 10);
		Lcd_Write_String("S2: ");
		
		dato1 = I2C_read_data(Slave1, &dato1);	// Lee dato del Slave 1 y lo guarda en la variable
		dato2 = I2C_read_data(Slave2, &dato2);	// Lee dato del Slave 2 y lo guarda en la variable
		
		Lcd_Set_Cursor(1,3);
		snprintf(buffer,sizeof(buffer), "%02d",dato1);	// Convierte los datos en caracteres
		Lcd_Write_String(buffer);
		
		Lcd_Set_Cursor(1, 10);
		snprintf(buffer,sizeof(buffer), "%03d", dato2);	// Convierte los datos en caracteres
		Lcd_Write_String(buffer);
		
		
    }
}

