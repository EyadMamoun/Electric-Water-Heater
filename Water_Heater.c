/*
 * project.c
 *
 * Created: 12/7/2021 6:46:23 PM
 *  Author: Lenovo
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include "STD_Types.h"
#include <util/delay.h>
#include "EEPROM.h"
#include "ADC.h"
#include "I2C.h"

extern float Set_temp;
extern float Sensor_temp;
extern float average_temp;

float Up_Button(void) //For increasing set temperature by 5 degrees
{
	while(PINB & (1<<0));
	Set_temp += 5;
	_delay_ms(50);
	return Set_temp;
}

float Down_Button(void) //For decreasing set temperature by 5 degrees
{
	while(PIND & (1<<6));
	Set_temp -= 5;
	_delay_ms(50);
	return Set_temp;
}

void Seven_Segment_display(float temp) //To display the temperature on the two 7 segments
{
	float DecimalValue = ((temp)/10);
	int Seg1 = DecimalValue;
	int Seg2 = ((DecimalValue - Seg1)*10);

	PORTA = (PORTA & 0x0F) | (Seg2<<4);
	PORTB |= (1<<1);
	PORTB &=~ (1<<2);
	_delay_ms(5);
	PORTA = (PORTA & 0x0F) | (Seg1<<4);
	PORTB |= (1<<2);
	PORTB &=~ (1<<1);
	_delay_ms(5);
}

float Setting_temp_mode(void)
{
	while(PINB & (1<<0) || (PIND & (1<<6)));
	int Counter = 0; //A variable to count 5 seconds and exits the Setting Mode
	int Flag = 0; //A variable to enter the Setting Mode
	Flag = 1;
	while (Flag == 1)
	{
		_delay_ms(100); //blinking
		Seven_Segment_display(Set_temp);
		while (Set_temp <= 70 && (PINB & (1<<0))) //Maximum value to reach is 75 degree
		{
			Up_Button();
			Counter = 0;
		}
		while (Set_temp >= 40 && (PIND & (1<<6))) //Minimum value to reach is 35 degree
		{
			Down_Button();
			Counter = 0;
		}
		Counter++;
		while(Counter == 50)
		{
			Flag = 0;
			PORTB &=~ (1<<1);
			PORTB &=~ (1<<2);
		}
	}
	EEPROM_Write(0b10100000,0b10110010,Set_temp); //To store the set temperature in the EEPROM
	return Set_temp;
}

void Heater(void) //DC Motor operates the Heater
{
	while(average_temp < (Set_temp - 5))
	{
		PORTC |= (1<<5);
		PORTC &=~ (1<<6);
		PORTC ^= (1<<7); //Toggle LED1
		_delay_ms(1000);
	}
	if(average_temp >= (Set_temp - 5))
	{
		PORTC &=~ (1<<5);
		PORTC &=~ (1<<6);
		PORTC &=~ (1<<7);
	}
}

void Cooler(void) //DC Motor operates the Cooler
{
	while(average_temp > (Set_temp + 5))
	{
		PORTC |= (1<<4);
		PORTC &=~ (1<<3);
		PORTC |= (1<<7); //Turn on LED1
	}
	if(average_temp <= (Set_temp + 5)) 
	{
		PORTC &=~ (1<<4);
		PORTC &=~ (1<<3);
		PORTC &=~ (1<<7);
	}
}

float Average(void) //To store the last 10 measures in an array and calculate their average
{
	float arr[10] = {0};
	float result = 0;
	
	for(int i=0;i<10;i++)
	{
		_delay_ms(100);
		Sensor_temp = ADC_Read(1);
		Sensor_temp = Sensor_temp/4;
		arr[i] = Sensor_temp;
		result = result + arr[i] ;
	}
	average_temp = result / 10;
	return average_temp;
}



