/*
 * EEPROM.c
 *
 * Created: 11/13/2021 7:38:30 PM
 *  Author: Lenovo
 */ 

#include "EEPROM.h"

//Master Transmit "Write"
void EEPROM_Write(uint8 WDevice_Address , uint8 WLocation_Address , uint8 data)
{
	if(TWI_Start() == 0x08) //0x08 ---> Confirmation address for the start "in the data sheet"
	{
		if(TWI_Send(WDevice_Address) == 0x18)
		{
			if(TWI_Send(WLocation_Address) == 0x28) //considered as a data to the master so the master should receive 0x28 "in the data sheet"
			{
				if(TWI_Send(data) == 0x28)
				{
					TWI_Stop();
				}
			}
		}
	}
}

//Master Receive "Read"
uint8 EEPROM_Read(uint8 Device_Address , uint8 Location_Address)
{
	uint8 WDevice_Address = (Device_Address & 0xFE); //To make the least significant bit 0 "For write"
	uint8 RDevice_Address = (Device_Address | (1<<0)); //To make the least significant bit 1 "For Read"
	uint8 ReadValue = 0;
	if(TWI_Start() == 0x08) 
	{
		if(TWI_Send(WDevice_Address) == 0x18)
		{
			if(TWI_Send(Location_Address) == 0x28) //considered as a data to the master so the master should receive 0x28 "in the data sheet"
			{
				if(TWI_Start() == 0x10)
				{
					if(TWI_Send(RDevice_Address) == 0x40)
					{
						TWI_Read_NACK(&ReadValue);
						TWI_Stop();
					}
				}
			}
		}
	}
	return ReadValue;
}




