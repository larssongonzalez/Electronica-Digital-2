/*
 * LCD8.h
 *
 * Created: 8/3/2024 5:15:56 PM
 *  Author: polst
 */ 


#ifndef LCD8_H_
#define LCD8_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Funcion inicio de la LCD
void Lcd_Init8bits();

// Funcion de repeticion de comandos
void inicio(char a);    

// Funcion de bits 
void dato_a_mostrar(char a);

// Funcion para limpiar la LCD
void Lcd_Clear();

// Funcion de posicionamiento del cursor
void Lcd_Set_Cursor(char a, char b);

// Funcion para escribir un caracter
void Lcd_Write_Char(char data);

// Funcion para escribir una cadena de caracteres
void Lcd_Write_String(char *a);

void Lcd_Shift_Right();

void Lcd_Shift_Left();



#endif /* LCD8_H_ */