 #include "outputs.h"
#include "typedefs.h"
#include "pins.h"

// ENUM | PIN
#define OutputsList(led) \
	led(output_0, PinC13) \
	led(output_1, PinA5) \
	led(output_2, PinA12) \
	led(output_3, PinA11) \
	led(output_4, PinA8) \
	led(output_5, PinB15) \
	led(output_6, PinB14) \

typedef struct
{
	pin_t pin;
}
output_t;

#define OutputEnum(e,pin) e,
enum { OutputsList(OutputEnum) };

#define Output(e,pin) {pin},
output_t outputs[] = { OutputsList(Output) };

const u8 outputsCount = sizeof(outputs)/sizeof(output_t);

void Outputs_Init(void)
{
	for (u8 i=0; i<outputsCount; i++)
	{
		Pin_Init(&outputs[i].pin, GPIO_Mode_Out_PP);
	}
}

void Output_Set(u8 outputIndex, u32 ledState)
{
	Pin_Set(&outputs[outputIndex].pin, (ledState != 0) ? pinState_HIGH : pinState_LOW);
}

boolean Output_GetValue(u8 outputIndex)
{
	return Pin_IsLow(&outputs[outputIndex].pin);
}

void Output_Toggle(u8 outputIndex)
{
	Pin_Toggle(&outputs[outputIndex].pin);
}
