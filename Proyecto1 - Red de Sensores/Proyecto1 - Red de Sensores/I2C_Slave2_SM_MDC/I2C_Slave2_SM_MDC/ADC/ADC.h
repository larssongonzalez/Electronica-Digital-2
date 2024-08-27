/*
 * ADC.h
 *
 * Created: 8/10/2024 7:21:06 PM
 *  Author: polst
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Funcion de configuracion del ADC
void initADC(void);

// Funcion para hacer una lectura de un ADC especifico
uint16_t readADCChannel(uint8_t channel);



#endif /* ADC_H_ */