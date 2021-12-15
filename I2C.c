/*
 * I2C.c
 *
 * Created: 11/12/2021 9:37:58 PM
 *  Author: Lenovo
 */ 

#include "I2C.h"


void TWI_Init(uint8 address)
{
	TWAR = address; //To Set the device Address
	TWBR = MyTWBR; //To Set the Clock
}
uint8 TWI_Start(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while(!(TWCR & (1<<TWINT)));
	return TWI_Status();
}
void TWI_Stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}
uint8 TWI_Status(void)
{
	return (TWSR & (0xF8)); //To read the most five significant bits in TWSR Reg.  "0xF8 ----> 0b11111000"
}
uint8 TWI_Send(uint8 data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN); //Clear the flag and enable TWI
	while(!(TWCR & (1<<TWINT))); //Stay here till the flag is one
	return TWI_Status();
}
uint8 TWI_Read_ACK(uint8 * data)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA); //Clear the flag and enable TWI and Acknowledge
	while(!(TWCR & (1<<TWINT))); //Stay here till the flag is one
	*data = TWDR;
	return TWI_Status();
}
uint8 TWI_Read_NACK(uint8 * data)
{
	TWCR = (1<<TWINT)|(1<<TWEN); //Clear the flag and enable TWI
	while(!(TWCR & (1<<TWINT))); //Stay here till the flag is one
	*data = TWDR;
	return TWI_Status();
}