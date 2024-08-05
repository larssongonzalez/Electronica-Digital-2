/*
 * UART.h
 *
 * Created: 7/29/2024 8:49:37 PM
 *  Author: polst
 */ 


#ifndef UART_H_
#define UART_H_
#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>


//Funcion para inicializar UART
void initUART(void);

//Funcion para escribir un caracter
void writeUART(char caracter);

//Funcion para escribir una cadena de caracteres
void stringUART(char *texto);



#endif /* UART_H_ */