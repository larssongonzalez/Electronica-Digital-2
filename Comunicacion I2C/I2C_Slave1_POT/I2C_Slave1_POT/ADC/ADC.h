/*
 * ADC.h
 *
 * Created: 8/4/2024 11:23:06 AM
 *  Author: polst
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// Funcion de inicio de ADC
void initADC(void);


#endif /* ADC_H_ */