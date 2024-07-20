/***********************************************************************************
 * Universidad del Valle de Guatemala
 * Electrónica Digital 2
 * ED2 - Lab01 - ContadorDescendente.c
 * Created: 11/7/2024 18:41:42
 * Author : polst
 * Descripción: Se realizara un contador descendente en un display de 7 segmentos
 * de 5 a 0 con un boton de inicio, luego se creara dos contadores de 4 bits y el 
 * contador que incremente y llegue a su fin antes se mostrara el numero del 
 * contador ganador en el display.
 **********************************************************************************/ 
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



volatile uint8_t  contdis = 0;	// Variable global para boton de display
volatile uint8_t  contbt1 = 0;	// Variable global para boton de jugador 1
volatile uint8_t  contbt2 = 0;	// Variable global para boton de jugador 2


int main(void)
{
    
	cli();
	
	//Puerto B como salida - Leds jugador 1
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3);
	
	//Puerto C como salida - Leds jugador 2
	DDRC |= (1<<DDC0)|(1<<DDC1)|(1<<DDC2)|(1<<DDC3);
	
	// Puerto D como salida (Display)
	DDRD = 0xFF;
	
	// PC5 como entrada (Boton) - Display
	DDRC &= ~(1<<DDC5);	//PC5 como entrada
	PORTC |= (1<<PORTC5);	// activamos Pull Up
	
	// PC5 como entrada - Boton Jugador 2
	DDRC &= ~(1<<DDC4);	//PC5 como entrada
	PORTC |= (1<<PORTC4);	// activamos Pull Up
	
	// PB4 como entrada - Boton Jugador 1
	DDRB &= ~(1<<DDB4); //PB4 como entrada
	PORTB |= (1<<PORTB4);	// Activamos Pull Up
	
	//Interrupcion PCINT1
	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT12)|(1<<PCINT13); // Mascara de interrupcion para PC4 - PC5
	
	//Interrupcion PCINT0
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT4);	// Mascara de interrupcion para PB4
	
	// Asegurarse de que todos los LEDs estén apagados al inicio
	PORTB &= ~(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);
	PORTC &= ~(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);
	PORTD = 0b01101101;
	
	sei();

	
	
    while (1) 
    {
		PORTB &= ~(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);  // Apaga todos los LEDs
		PORTC &= ~(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);  // Apaga todos los LEDs
		
		if (contdis == 1)
		{
			contbt1 = 0;
			contbt2 = 0;
			PORTB &= ~(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);  // Apaga todos los LEDs
			PORTC &= ~(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);  // Apaga todos los LEDs
			PORTD = 0b01101101;	//5
			_delay_ms(1000);
			PORTD = 0b01100110;	//4
			_delay_ms(1000);
			PORTD = 0b01001111;	//3
			_delay_ms(1000);
			PORTD = 0b01011011;	//2
			_delay_ms(1000);
			PORTD = 0b00000110;	//1
			_delay_ms(1000);
			PORTD = 0b00111111;	//0
			_delay_ms(1000);
			contdis = 2;
				
		} 
		
		// Control de LEDs del jugador 2
		PORTC &= ~(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC2)|(1<<PORTC3);  // Apaga todos los LEDs
		
		switch(contbt2){
			case 0: break;
			case 1: PORTC |= (1<<PORTC3); break;
			case 2:	PORTC |= (1<<PORTC2); break;
			case 3: PORTC |= (1<<PORTC1); break;
			case 4: 
				PORTC |= (1<<PORTC0);
				PORTD |= 0b00000000;	//apagado
				_delay_ms(2);
				PORTD = 0b01011011;	//2
				contbt1 = 0;
			break;
			
		}
		
		// Control de LEDs del jugador 1
		PORTB &= ~(1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3);  // Apaga todos los LEDs
		
		switch(contbt1){
			case 0: break;
			case 1: PORTB |= (1<<PORTB0); break;
			case 2:	PORTB |= (1<<PORTB1); break;
			case 3: PORTB |= (1<<PORTB2); break;
			case 4:
				PORTB |= (1<<PORTB3);
				PORTD |= 0b00000000;	//apagado
				_delay_ms(2);
				PORTD = 0b00000110;	//1
				contbt2 = 0;
			break;
		}
		
		
		
    }
}

ISR(PCINT1_vect){
	
	if ((PINC & (1<<PINC5)) == 0) // PC5 = 0?
	{
		if (contdis == 0) // Primera pulsación
		{
			contdis = 1;
		}
		else if (contdis == 1) // Segunda pulsación
		{
			contdis = 2;
		}
		else if (contdis == 2)	// Tercera pulsacion
		{
			contdis = 1;
			// Apagar todos los LEDs
			PORTB &= ~(1<<PORTB0 | 1<<PORTB1 | 1<<PORTB2 | 1<<PORTB3);
			PORTC &= ~(1<<PORTC0 | 1<<PORTC1 | 1<<PORTC2 | 1<<PORTC3);
		}
	}

	if (contdis == 2) // Solo incrementa si contdis es 2
	{
		if ((PINC & (1<<PINC4)) == 0) // Botón del jugador 2
		{
			contbt2 = (contbt2 < 4) ? contbt2 + 1 : contbt2; // Limita a 4
		}
	}
}

ISR(PCINT0_vect){
	if (contdis == 2) // Verifica el estado de la cuenta regresiva
	{
		if ((PINB & (1<<PINB4)) == 0)	//Verifica el estado del boton PB4
		{
			contbt1 = contbt1 < 4 ? contbt1 + 1 : contbt1; // Limita a 4
		}
	}
}


