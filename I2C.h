/*
 * I2C.h
 *
 * Created: 11/12/2021 9:37:34 PM
 *  Author: Lenovo
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>

typedef unsigned char uint8;

#define F_CPU 16000000UL
#define F_TWI 400000UL

#define MyTWPS 1

#define MyTWBR (((F_CPU/F_TWI)-16)/(2*(4^MyTWPS)))

void TWI_Init(uint8 address);
uint8 TWI_Start(void);
void TWI_Stop(void);
uint8 TWI_Status(void);
uint8 TWI_Send(uint8 data);
uint8 TWI_Read_ACK(uint8 * data); //Function Takes an address of the place where the data is stored
uint8 TWI_Read_NACK(uint8 * data); //Not acknowledge to stop receiving data

#endif /* I2C_H_ */