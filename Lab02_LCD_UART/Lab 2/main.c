/******************************************************************************
* Universidad del Valle de Guatemala
* Programacion de Microcontroladores 2
* Archivo: Lab 2
* Hardware: ATMEGA328P
* Autor: Larsson González
* Carnet: 221121
* Description: Simulacion de sensores analogicos con dos potenciometros y mostrar
* su voltaje en una pantalla LCD. Ademas se implementara comunicación UART para mostrar
* el incremento y decremento de un contador como un sensor analogico extra.
*******************************************************************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LCD8bits/LCD8bits.h"
#include "ADC/ADC.h"

#define  BAUD 9600
#define  BRC ((F_CPU/16/BAUD) - 1)

uint8_t adcval1 = 0, adcval2 = 0, caso = 0;	//Variables para almacenar los valores del pot
volatile uint8_t cont = 0, newLCD = 0, newADC = 0;	//Variables para actualizar y mostrar nuevo sensor
char lista[10] = {'0','1','2','3','4','5','6','7','8','9'};
char lista1[4], lista2[4], lista3[4] = {'0', '0', '0'};

void setup(void) {
    cli();  
    DDRD = 0xFF;  // Puerto D como salida
    DDRB = 0xFF;  // Puerto B como salida
    DDRC = 0;     // Puerto C como entrada
    //UCSR0B = 0;   // Usar los pines TX y RX como digitales

	//Configuracion UART
	UBRR0H = (BRC >> 8);
	UBRR0L = BRC;
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
    Lcd_Init8bits();  // Iniciar pantalla LCD
	_delay_ms(20);
    Lcd_Clear();
    initADC();  // Iniciar ADC
	ADCSRA |= (1<<ADSC);	//Inicia la conversion

    sei();  
}

void mostrarLista(char *lista, int valor) {
    lista[0] = '0' + (valor / 100);
    lista[1] = '0' + ((valor / 10) % 10);
    lista[2] = '0' + (valor % 10);
    lista[3] = '\0';
}

void enviarUART(char data){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;	
}

void stringUART(char *str){
	while(*str){
		enviarUART(*str++);
	}
}

// Función para actualizar la pantalla LCD
void actualizarLCD(void) {
	Lcd_Clear();  // Limpiar pantalla
	Lcd_Set_Cursor(0, 2);
	Lcd_Write_String("S1:");  // Escribir  Sensor 1
	Lcd_Set_Cursor(0, 7);
	Lcd_Write_String("S2:");  // Escribir Sensor 2
	Lcd_Set_Cursor(0, 12);
	Lcd_Write_String("S3:");  // Escribir Sensor 3
	
	// Actualizar las cadenas con los valores actuales
	mostrarLista(lista1, adcval1);
	mostrarLista(lista2, adcval2);
	mostrarLista(lista3, cont);

	// Mostrar los valores en la LCD
	Lcd_Set_Cursor(1, 2);
	Lcd_Write_String(lista1);
	Lcd_Set_Cursor(1, 7);
	Lcd_Write_String(lista2);
	Lcd_Set_Cursor(1, 12);
	Lcd_Write_String(lista3);
}


int main(void) {
    setup();
    
	uint8_t pval1 = 255, pval2 = 255, pcont = 255;
	
    while (1) {         
		
		if ((adcval1 != pval1 || adcval2 != pval2 || cont != pcont || newLCD))
		{
			actualizarLCD();
			pval1 = adcval1;
			pval2 = adcval2;
			pcont = cont;
			newLCD = 0;
			newADC = 0;
			
		}
		
		mostrarLista(lista1, adcval1);
		mostrarLista(lista2, adcval2);
		mostrarLista(lista3, cont);
		
		stringUART("S1: ");
		stringUART(lista1);
		stringUART("S2: ");
		stringUART(lista2);
		stringUART("S3: ");
		stringUART(lista3);
		enviarUART('\n');
		
		_delay_ms(100);
		 
       
		
		
		
		/*ADCSRA |= (1 << ADSC);  // Leer ADC

        Lcd_Set_Cursor(0, 2);
        Lcd_Write_String("S1:");
        Lcd_Set_Cursor(0, 7);
        Lcd_Write_String("S2:");
        
        mostrarLista(lista1, adcval1);
        mostrarLista(lista2, adcval2);
        mostrarLista(lista3, 255);  

        Lcd_Set_Cursor(1, 2);
        Lcd_Write_String(lista1);
        Lcd_Set_Cursor(1, 7);
        Lcd_Write_String(lista2);
        
        _delay_ms(100);
        Lcd_Clear(); */
    }
}

ISR(ADC_vect) {
    if (caso == 0) {
        ADMUX &= ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0));
        adcval1 = ADCH;
        caso = 1;
    } else {
        ADMUX = (ADMUX & ~((1 << MUX2) | (1 << MUX1) | (1 << MUX0))) | (1 << MUX0);
        adcval2 = ADCH;
        caso = 0;
    }
    ADCSRA |= (1 << ADSC);  //Inicia proxima conversion
	newADC = 1;
}

ISR(USART_RX_vect){
	char received = UDR0;  // Leer dato recibido
	if (received == '+') {
		if (cont < 255) {
			cont++;  // Incrementar contador si es menor a 255
		}
		} else if (received == '-') {
		if (cont > 0) {
			cont--;  // Decrementar contador si es mayor a 0
		}
	}
	newLCD = 1; // Indicar que se debe actualizar la LCD

}
