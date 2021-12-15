/*
 * EEPROM.h
 *
 * Created: 11/13/2021 7:39:00 PM
 *  Author: Lenovo
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "I2C.h"

void EEPROM_Write(uint8 WDevice_Address , uint8 WLocation_Address , uint8 data);
uint8 EEPROM_Read(uint8 Device_Address , uint8 Location_Address);

#endif /* EEPROM_H_ */