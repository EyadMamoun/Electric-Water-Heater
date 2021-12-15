/*
 * ADC.c
 *
 * Created: 10/22/2021 9:23:11 PM
 *  Author: user
 */ 

#include "ADC.h"
#include <avr/io.h>

void ADC_Init(){
	ADMUX = (1<<REFS0)|(1<<REFS1);//used internal Vref 2.56v 1100 0000
	ADCSRA = (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);//enable ADC & used prescale 128
}
unsigned short ADC_Read(unsigned char channel){
	unsigned short data = 0;
	ADMUX |= (channel & 0x1F);//to use only first 5 bits in ADMUX
	ADCSRA |= (1<<ADSC);//start conversion
	while(!((ADCSRA>>ADIF)&1));//waiting ADC to finish conversion 
	ADCSRA |= (1<<ADIF);//clear ADIF bit.
	data = ADCL;
	data |= (ADCH<<8);//save the ADC value
	return data;
}