/*
 * Traffic_Interface.h
 *
 * Created: 11/1/2022 2:46:20 PM
 *  Author: Osama Mahmoud
 */ 


#ifndef TRAFFIC_INTERFACE_H_
#define TRAFFIC_INTERFACE_H_


#define  F_CPU           8000000UL

#include "../HAL/LED/LED_Interface.h"
#include "../BUTTON/BUTTON_Interface.h"

//modes
#define NORMAL_MODE     0
#define PEDESTRIAN_MODE 1

//states
#define CAR_GREEN      0
#define CAR_YELLOW     1
#define CAR_RED        2

#define RED            0
#define GREEN          1
#define YELLOW1        2
#define YELLOW2        3

//pins
#define LED_CAR_GREEN  PINA0
#define LED_CAR_YELLOW PINA1
#define LED_CAR_RED    PINA2

#define LED_PED_GREEN  PINB0
#define LED_PED_YELLOW PINB1
#define LED_PED_RED    PINB2
     

void AppStart(void);


#endif /* TRAFFIC_INTERFACE_H_ */