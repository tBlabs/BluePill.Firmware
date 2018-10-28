#ifndef PWM_H_
#define PWM_H_

#include "typedefs.h"

void Pwm_Init(void);
void Pwm_Set(u8 pwmIndex, u32 value);
u32 Pwm_GetValue(u8 pwmIndex);

#endif /* PWM_H_ */
