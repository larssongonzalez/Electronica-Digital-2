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

#define Slave1 0x20		// Direccion Slave 1 (Sensor Ultrasonico)
#define Slave2 0x22		// Direccion Slave 2 (Sensor de Metales)
#define Slave3 0x48		// Direccion Slave 3 (Sensor de Temperatura LM75B con I2C)
#define Slave4 0x18		// Direccion Slave 4 (ESP32)

#define THRESHOLD 250	// Umbral del sensor de metales

uint8_t dato1, dato2, status;	// Variables de almacenamiento de los Slaves
uint16_t temp;
uint16_t dato3;
char buffer[30];	// Buffer para cadena de caracteres




int main(void)
{
    cli();
	
	DDRD = 0xFF;	// Puerto D como Output (LCD)
	DDRB = 0xFF;	// Puerto B como Output
	
	UCSR0B = 0;		// Usar los pines TX y RX como digitales
	Lcd_Clear();
	Lcd_Init8bits();
	Lcd_Clear();
	I2C_Config_MASTER(4,200000);	// Prescaler = 4 , 200kHz de comunicacion
	
	sei();
	
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
		
		// Envia los tres datos de los sensores al esp32
		I2C_esclavo(dato1, Slave4);		// Envia dato de sensor ultrasonico a esp32
		I2C_esclavo(dato2, Slave4);		// Envia dato de sensor de metales a esp32
		I2C_esclavo(temp, Slave4);		// Envia dato de sensor de temperatura a esp32
    }
}



