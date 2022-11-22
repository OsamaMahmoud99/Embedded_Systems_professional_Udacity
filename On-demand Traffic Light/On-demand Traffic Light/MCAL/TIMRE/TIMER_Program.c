#include "TIMER_Interface.h"

/****************************Pointer to functions to be assigned to ISR*********************************/

static void (*Timer0_OVF_Fptr) (void)=NULLPTR;
static void (*Timer0_OC_Fptr)  (void)=NULLPTR;

Timer0_status_t Timer0_Init( Timer0Mode_type mode,Timer0Scaler_type scaler,OC0Mode_type oc_mode)
{
	Timer0_status_t status = TIMRE0_OK;
	
	switch (mode)
	{
		case TIMER0_NORMAL_MODE:
		CLR_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0,WGM00);
		CLR_BIT(TCCR0,WGM01);
		break;
		case TIMER0_CTC_MODE:
		CLR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
		case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
		default:
		status = TIMRE0_NOK;
		break;
	}

	TCCR0&=0XF8;
	TCCR0|=scaler;

	switch (oc_mode)
	{
		case OCO_DISCONNECTED:
		CLR_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OCO_TOGGLE:
		SET_BIT(TCCR0,COM00);
		CLR_BIT(TCCR0,COM01);
		break;
		case OCO_NON_INVERTING:
		CLR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
		case OCO_INVERTING:
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
		default:
		status = TIMRE0_NOK;
		break;
	}
	
	return status;
}

Timer0_status_t Timer0_setTCNT0(u16 Value)
{
	TCNT0 = Value;
	return TIMRE0_OK;
}

Timer0_status_t Timer0_Stop(void)
{
	TCCR0 &= 0xF8;
	return TIMRE0_OK;
}

void Timer0_DelayMs(u32 Milli)
{
	u32 numofoverflow = ((u32)(Milli*1000)) / 256;
	u32 overflowcounter = 0;
	
	Timer0_Init(TIMER0_NORMAL_MODE , TIMER0_SCALER_8 , OCO_DISCONNECTED);
	
	while(overflowcounter < numofoverflow)
	{
			while(!READ_BIT(TIFR , TOV0));
			SET_BIT(TIFR , TOV0);
			overflowcounter++;
	}
	overflowcounter = 0;
	Timer0_Stop();

}


Timer0_status_t Timer0_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE0);
}
Timer0_status_t Timer0_OV_InterruptDisable(void)
{
	CLR_BIT(TIMSK,TOIE0);
}
Timer0_status_t Timer0_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE0);
}

Timer0_status_t Timer0_OC_InterruptDisable(void)
{
	CLR_BIT(TIMSK,OCIE0);
}

/*******************************************TIMER 0 call Back function***************************************/

void Timer0_OVF_SetCallBack(void(*LocalFptr)(void))
{
	Timer0_OVF_Fptr=LocalFptr;
}

void Timer0_Oc_SetCallBack(void(*LocalFptr)(void))
{
	Timer0_OC_Fptr=LocalFptr;
}

/*********************************Timer 0 ISR functions*********************************************/

ISR(TIMER0_OVF_vect)
{
	if (Timer0_OVF_Fptr!=NULLPTR)
	{
		Timer0_OVF_Fptr();
	}
}
ISR(TIMER0_COMP_vect)
{
	if (Timer0_OC_Fptr!=NULLPTR)
	{
		Timer0_OC_Fptr();
	}
}
