/*
 * LED_Interface.h
 *
 * Created: 10/28/2022 3:25:05 PM
 *  Author: Osama Mahmoud
 */ 


#ifndef LED_INTERFACE_H_
#define LED_INTERFACE_H_


#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/TIMRE/TIMER_Interface.h"

typedef enum{
	
	LED_OK,
	LED_NOK	
	
}Led_Error_type;

Led_Error_type LED_On(DIO_Pin_type pin);
Led_Error_type LED_Off(DIO_Pin_type pin);
Led_Error_type LED_Toggle(DIO_Pin_type pin);


#endif /* LED_INTERFACE_H_ */