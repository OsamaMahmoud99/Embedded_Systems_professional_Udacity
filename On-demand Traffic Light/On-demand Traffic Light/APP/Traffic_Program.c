/*
 * Traffic_Program.c
 *
 * Created: 11/1/2022 2:45:47 PM
 *  Author: Osama Mahmoud
 */ 

#include "Traffic_Interface.h"


u8 carLedState;
u8 transationState = GREEN;
u8 flag = 0;
u8 mode = NORMAL_MODE;
u8 button_status = 0;
u8 volatile counter = 0;

void Delay_5s(void)
{
	u16 i;
	Timer0_Init(TIMER0_NORMAL_MODE , TIMER0_SCALER_8 , OCO_DISCONNECTED);
	
	TCNT0 = 56;
	
	for(i=0; i<25000; ++i)  //200*25000 = 5*10^6
	{
		if (mode == PEDESTRIAN_MODE)
		{
			break;
		}
		
		while(TCNT0);
		TCNT0 = 56;
	}
	
	Timer0_Stop();
}

static void Blink_Yellow(void)
{
	Timer0_Init(TIMER0_NORMAL_MODE , TIMER0_SCALER_8 , OCO_DISCONNECTED);
	
	Timer0_setTCNT0(56);
	
	for (u16 i=0; i<25000; ++i)
	{
		if (mode == PEDESTRIAN_MODE)
		{
			 break;
	    }
		while(TCNT0);
		if ((i%2500) == 0)             //every 0.5s
		{
			LED_Toggle(LED_CAR_YELLOW);
			LED_Toggle(LED_PED_YELLOW);
		}
		Timer0_setTCNT0(56);
		
	
	}
	
	Timer0_Stop();
}

static void Traffic_LedsOff(void)
{
	LED_Off(LED_CAR_GREEN);
	LED_Off(LED_CAR_YELLOW);
	LED_Off(LED_CAR_RED);
	LED_Off(LED_PED_GREEN);
	LED_Off(LED_PED_YELLOW);
	LED_Off(LED_PED_RED);
}

static void Normal_Mode(void)
{
	mode = NORMAL_MODE;
	
	if (transationState == GREEN)
	{
		carLedState = CAR_GREEN;
		transationState = YELLOW1;
		
		LED_On(LED_CAR_GREEN);
		LED_On(LED_PED_RED);
		Delay_5s();
	}
	
	if (transationState == YELLOW1 )
	{
		carLedState = CAR_YELLOW;
		transationState = RED;
		
		Blink_Yellow();
		LED_Off(LED_PED_YELLOW);
		LED_Off(LED_CAR_YELLOW);
		LED_Off(LED_CAR_GREEN);
		LED_Off(LED_PED_RED);
	}
	
	if (transationState == RED)
	{
		carLedState = CAR_RED;
		transationState = YELLOW2;
	
		LED_On(LED_CAR_RED);
		LED_On(LED_PED_GREEN);
		Delay_5s();
	}
	
	if (transationState == YELLOW2 )
	{
		carLedState = CAR_YELLOW;
		transationState = GREEN;
		
		Blink_Yellow();
		LED_Off(LED_PED_YELLOW);
		LED_Off(LED_CAR_YELLOW);
		LED_Off(LED_CAR_RED);
		LED_Off(LED_PED_GREEN);
	}
}

static void Pedestrian_Mode(void)
{
	flag = 1;
	Traffic_LedsOff();
		
	if ((carLedState == CAR_GREEN) || (carLedState == CAR_YELLOW))
	{
		//the CAR Green led still on.
		LED_On(LED_CAR_GREEN);
		
		//the PED RED led will be on.
		LED_On(LED_PED_RED);
		
		//both Yellow LEDs start to blink for five seconds.
		Blink_Yellow();
		LED_Off(LED_CAR_YELLOW);
		LED_Off(LED_PED_YELLOW);
		LED_Off(LED_CAR_GREEN);
		LED_Off(LED_PED_RED);
		
		//the cars' Red LED and pedestrian Green LEDs are on for five seconds.
		LED_On(LED_CAR_RED);
		LED_On(LED_PED_GREEN);
		Delay_5s();
		
		Blink_Yellow();
		LED_Off(LED_PED_YELLOW);
		LED_Off(LED_CAR_YELLOW);
		LED_Off(LED_PED_GREEN);
		LED_Off(LED_CAR_RED);
		
		LED_On(LED_CAR_GREEN);
		LED_On(LED_PED_RED);
	}
	else if (carLedState == CAR_RED)
	{
		LED_On(LED_PED_GREEN);
		LED_On(LED_CAR_RED);
		Delay_5s();
		
		Blink_Yellow();
		LED_Off(LED_CAR_YELLOW);
		LED_Off(LED_PED_YELLOW);
		LED_Off(LED_PED_GREEN);
		LED_Off(LED_CAR_RED);
		
		LED_On(LED_CAR_GREEN);
		LED_On(LED_PED_RED);
	}
	transationState = GREEN;
	mode = PEDESTRIAN_MODE;
	flag = 0;
}

void CallBackFunc(void)
{
	if (flag == 0)
	{
		Pedestrian_Mode();
	}
}

void AppStart(void)
{
	// init global interrupt
	sei();
	
	//init pins
	DIO_Init(); 
	
	//init external interrupt
	EXI_TriggerEdge(EX_INT0 , ANY_LOGIC_CHANGE);
	EXI_SetCallBack(EX_INT0 , CallBackFunc);
	EXI_Enable(EX_INT0);
	

	while(1)
	{
		Normal_Mode();	
	}
	
}

