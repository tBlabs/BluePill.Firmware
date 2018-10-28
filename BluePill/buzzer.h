#ifndef BUZZER_H_
#define BUZZER_H_

#include <typedefs.h>

void Buzzer1_Init(void);
void Buzzer1_Enable(boolean enable);
boolean Buzzer1_IsEnabled();
void Buzzer1_SetVolume(u32 volume);
void Buzzer1_SetFrequency(u32 freq);
u32 Buzzer1_GetVolume();
u32 Buzzer1_GetFrequency();

#endif /* BUZZER_H_ */
