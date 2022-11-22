#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include "../LIB/StdTypes.h"
#include "../LIB/MemMap.h"
#include "../LIB/Utils.h"

typedef enum{
	TIMER0_STOP=0,
	TIMER0_SCALER_1,
	TIMER0_SCALER_8,
	TIMER0_SCALER_64,
	TIMER0_SCALER_256,
	TIMER0_SCALER_1024,
	EXTERNAL_FALLING,
	EXTERNAL_RISING
}Timer0Scaler_type;

typedef enum
{
	TIMER0_NORMAL_MODE=0,
	TIMER0_PHASECORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE

}Timer0Mode_type;
typedef enum
{
	OCO_DISCONNECTED=0,
	OCO_TOGGLE,
	OCO_NON_INVERTING,
	OCO_INVERTING

}OC0Mode_type;

typedef enum
{
	TIMRE0_OK,
	TIMRE0_NOK	
}Timer0_status_t;

Timer0_status_t Timer0_Init( Timer0Mode_type mode,Timer0Scaler_type scaler,OC0Mode_type oc_mode);
Timer0_status_t Timer0_OV_InterruptEnable(void);            //over flow interrupt
Timer0_status_t Timer0_OV_InterruptDisable(void);
Timer0_status_t Timer0_OC_InterruptEnable(void);            //output compare interrupt
Timer0_status_t Timer0_OC_InterruptDisable(void);


#endif /* TIMER_INTERFACE_H_ */