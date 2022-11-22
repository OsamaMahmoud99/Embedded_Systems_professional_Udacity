#ifndef DIO_PRIVATE_H_
#define DIO_PRIVATE_H_


extern const DIO_PinStatus_type PinsStatusArray[TOTAL_PINS];

static Error_state_type DIO_InitPin(DIO_Pin_type pin, DIO_PinStatus_type status);

#endif /* DIO_PRIVATE_H_ */