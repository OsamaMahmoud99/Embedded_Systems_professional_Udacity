/*
 * BUTTON_Interface.h
 *
 * Created: 11/15/2022 10:27:46 AM
 *  Author: Osama
 */ 


#ifndef BUTTON_INTERFACE_H_
#define BUTTON_INTERFACE_H_

#include "../MCAL/DIO/DIO_Interface.h"
#include "../MCAL/EXTI/EXTI_Interface.h"

DIO_PinVoltage_type Button_read(DIO_Pin_type pin);



#endif /* BUTTON_INTERFACE_H_ */