#ifndef DISPLAYS_H_
#define DISPLAYS_H_

#include "typedefs.h"
#include "display.h"

void Displays_Init(void);
void Display_Set(u8 displayIndex, u32 value);
void DisplayDot_Set(u8 displayIndex, u32 dot);
u32 Display_GetValue(u8 displayIndex);
u32 DisplayDot_GetValue(u8 displayIndex);

#endif /* DISPLAYS_H_ */
