/*
 * Project.c
 *
 * Created: 12/7/2021 2:44:13 AM
 * Author : Lenovo
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "ADC.h"
#include "Water_Heater.h"
#include "I2C.h"

float Sensor_temp = 0; //Sensor reading
float Set_temp = 60; //Temperature to be set 
float average_temp = 0; //The average of last 10 readings
int System_Lock = 0; //for turning ON & OFF

int main(void)
{
	DIO_Init();
	ADC_Init();
	
	DDRB &=~ (1<<0); //Up Button
	DDRD &=~ (1<<6); //Down Button
	DDRD &=~ (1<<2); //ON-OFF Button
	
	DDRC |= (1<<7); //LED1

    DDRA |= (1<<4)|(1<<5)|(1<<6)|(1<<7);
    DDRB |= (1<<1)|(1<<2); //for 7 Segment
	
	DDRD |= (1<<4)|(1<<5); //EN-1 & EN-2 for heating and cooling motors
	DDRC |= (1<<3)|(1<<4)|(1<<5)|(1<<6);
	TCCR1A = (1<<COM1A1)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);//N=64 - FPWM - NON Inverting
	ICR1 = 1000;
	OCR1A = 700;//Motor speed
		
    while (1) 
    {
		PORTD &=~ (1<<4);
		PORTD &=~ (1<<5);
		PORTB &=~ (1<<1);
		PORTB &=~ (1<<2);//Make all ports initially OFF
		_delay_ms(100);
		while (PIND & (1<<2))
		{
			while (PIND & (1<<2));
			System_Lock = 1; //Turn ON the system
			while (System_Lock == 1)
			{
				Sensor_temp = ADC_Read(1);
				Sensor_temp = Sensor_temp/4;
				Seven_Segment_display(Sensor_temp);
					
				while((PINB & (1<<0)) || (PIND & (1<<6)))
				{
					Setting_temp_mode();
				}
					
				Average();
				
				Heater();
				Cooler();
				while (PIND & (1<<2))
				{
					while (PIND & (1<<2));
					System_Lock = 0; //Turn OFF the system
				}
			}
		}	
    }
}

