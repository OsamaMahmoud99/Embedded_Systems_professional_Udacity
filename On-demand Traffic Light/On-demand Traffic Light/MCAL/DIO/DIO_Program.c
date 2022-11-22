#include "DIO_Interface.h"
#include "DIO_Private.h"

static Error_state_type DIO_InitPin(DIO_Pin_type pin , DIO_PinStatus_type status)
{
	DIO_Port_type port = pin/8;
	u8 pin_num  = pin % 8;
	Error_state_type Dio_status = DIO_OK;
	switch(status)
	{
		case OUTPUT:
		switch(port)
		{
			case PA:
			SET_BIT(DDRA , pin_num);
			CLR_BIT(PORTA , pin_num);
			break;
			case PB:
			SET_BIT(DDRB , pin_num);
			CLR_BIT(PORTB , pin_num);
			break;
			case PC:
			SET_BIT(DDRC , pin_num);
			CLR_BIT(PORTC , pin_num);
			break;
			case PD:
			SET_BIT(DDRD , pin_num);
			CLR_BIT(PORTD , pin_num);
			break;
			default:
			Dio_status = DIO_NOK;
			break;
		}
		break;
		case INFREE:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA , pin_num);
			CLR_BIT(PORTA , pin_num);
			break;
			case PB:
			CLR_BIT(DDRB , pin_num);
			CLR_BIT(PORTB , pin_num);
			break;
			case PC:
			CLR_BIT(DDRC , pin_num);
			CLR_BIT(PORTC , pin_num);
			break;
			case PD:
			CLR_BIT(DDRD , pin_num);
			CLR_BIT(PORTD , pin_num);
			break;
			default:
			Dio_status = DIO_NOK;
			break;
		}
		break;
		case INPULLUP:
		switch(port)
		{
			case PA:
			CLR_BIT(DDRA , pin_num);
			SET_BIT(PORTA , pin_num);
			break;
			case PB:
			CLR_BIT(DDRB , pin_num);
			SET_BIT(PORTB , pin_num);
			break;
			case PC:
			CLR_BIT(DDRC , pin_num);
			SET_BIT(PORTC , pin_num);
			break;
			case PD:
			CLR_BIT(DDRD , pin_num);
			SET_BIT(PORTD , pin_num);
			break;
			default:
			Dio_status = DIO_NOK;
			break;
		}
		break;
		default:
		Dio_status = DIO_NOK;
		break;
	}
	return Dio_status;
}


Error_state_type DIO_Init(void)
{
	DIO_Pin_type counter;
	Error_state_type Dio_status = DIO_OK;
	for (counter=PINA0; counter<TOTAL_PINS; ++counter)
	{
		Dio_status = DIO_InitPin(counter , PinsStatusArray[counter]);
	}
	
	return Dio_status;
}

Error_state_type DIO_WritePin(DIO_Pin_type pin, DIO_PinVoltage_type volt)
{
	DIO_Port_type port = pin/8;
	u8 pin_num  = pin % 8;
	Error_state_type Dio_status = DIO_OK;
	
	if (volt == LOW)
	{
		switch(port)
		{
			case PA:
			CLR_BIT(PORTA,pin_num);
			break;
			case PB:
			CLR_BIT(PORTB,pin_num);
			break;
			case PC:
			CLR_BIT(PORTC,pin_num);
			break;
			case PD:
			CLR_BIT(PORTD,pin_num);
			break;
			default:
			Dio_status = DIO_NOK;
			break;
		}
	}
	else if (volt == HIGH)
	{
		switch(port)
		{
			case PA:
			SET_BIT(PORTA,pin_num);
			break;
			case PB:
			SET_BIT(PORTB,pin_num);
			break;
			case PC:
			SET_BIT(PORTC,pin_num);
			break;
			case PD:
			SET_BIT(PORTD,pin_num);
			break;
			default:
			Dio_status = DIO_NOK;
			break;
		}
	}
	else{
		Dio_status = DIO_NOK;
	}
	return Dio_status;
}

Error_state_type DIO_TogglePin(DIO_Pin_type pin)
{
	DIO_Port_type port = pin/8;
	u8 pin_num  = pin % 8;
	Error_state_type Dio_status = DIO_OK;
	
	switch (port)
	{
		case PA:
		TOG_BIT(PORTA,pin_num);
		break;
		case PB:
		TOG_BIT(PORTB,pin_num);
		break;
		case PC:
		TOG_BIT(PORTC,pin_num);
		break;
		case PD:
		TOG_BIT(PORTD,pin_num);
		break;
		default:
		Dio_status = DIO_NOK;
		break;
	}
	return Dio_status;
}

Error_state_type DIO_ReadPin(DIO_Pin_type pin ,DIO_PinVoltage_type* value)
{
	DIO_PinVoltage_type volt = LOW;
	DIO_Port_type port = pin/8;
	u8 pin_num  = pin % 8;
	
	switch (port)
	{
		case PA:
		volt = READ_BIT(PINA,pin_num);
		break;
		case PB:
		volt = READ_BIT(PINB,pin_num);
		break;
		case PC:
		volt = READ_BIT(PINC,pin_num);
		break;
		case PD:
		volt = READ_BIT(PIND,pin_num);
		break;
	}
	*value = volt;
	return DIO_OK;
}
