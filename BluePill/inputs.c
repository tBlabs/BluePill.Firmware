#include "inputs.h"
#include "pins.h"

// ENUM | PIN
#define InputsList(item) \
	item(input_0, 	PinB10) \
	item(input_1, 	PinB11) \
	item(input_2, 	PinB9) \
	item(input_3, 	PinB8) \
	item(input_4, 	PinB13) \
	item(input_5, 	PinB12) \
	item(input_6, 	PinA4) \

typedef struct
{
	pin_t pin;
}
input_t;

#define InputEnum(e,pin) e,
enum { InputsList(InputEnum) };

#define Input(e,pin) {pin},
input_t inputs[] = { InputsList(Input) };

const u8 inputsCount = sizeof(inputs)/sizeof(input_t);

void Inputs_Init(void)
{
	for (u8 i=0; i<inputsCount; i++)
	{
		Pin_Init(&inputs[i].pin, GPIO_Mode_IPU);
	}
}

buttonState_t Input_GetValue(u8 inputIndex)
{
	return Pin_IsLow(&inputs[inputIndex].pin) ? buttonState_PRESSED : buttonState_NOT_PRESSED;
}
