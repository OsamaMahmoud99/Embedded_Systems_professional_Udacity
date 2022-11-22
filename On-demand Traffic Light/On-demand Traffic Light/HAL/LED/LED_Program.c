/*
 * LED_Program.c
 *
 * Created: 10/28/2022 3:24:40 PM
 *  Author: Osama 
 */ 

#include "LED_Interface.h"

Led_Error_type LED_On(DIO_Pin_type pin)
{
	Led_Error_type status = LED_NOK;
	if(DIO_WritePin(pin , HIGH) == DIO_OK)
	{
		status = LED_OK;
	}
	return status;
}
Led_Error_type LED_Off(DIO_Pin_type pin)
{
	Led_Error_type status = LED_NOK;
	if(DIO_WritePin(pin , LOW) == DIO_OK)
	{
		status = LED_OK;
	}
	return status;
}
Led_Error_type LED_Toggle(DIO_Pin_type pin)
{
	Led_Error_type status = LED_NOK;
	if(DIO_TogglePin(pin) == DIO_OK)
	{
		status = LED_OK;
	}
	return status;
}
