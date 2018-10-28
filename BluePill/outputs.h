#ifndef LED_H_
#define LED_H_

#include "typedefs.h"

void Outputs_Init(void);
void Output_Set(u8 outputIndex, u32 ledState);
boolean Output_GetValue(u8 outputIndex);
void Output_Toggle(u8 outputIndex);

#endif /* LED_H_ */
