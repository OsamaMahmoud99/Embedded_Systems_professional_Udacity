/*
 * BUTTON_Program.c
 *
 * Created: 11/15/2022 10:30:25 AM
 *  Author: Osama
 */ 

#include "BUTTON_Interface.h"

DIO_PinVoltage_type Button_read(DIO_Pin_type pin)
{
	DIO_PinVoltage_type volt = LOW;
	
	DIO_ReadPin(pin ,&volt);
	
	return volt;
}