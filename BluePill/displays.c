#include "displays.h"
#include "display.h"
#include <config.h>
#include <debug.h>
#include <text.h>
#include <stm32f10x.h>
#include <general.h>
#include <pins.h>

// ENUM | WIDTH | DATA PIN | CLOCK PIN | LATCH PIN
#define DisplaysList(display) \
	display(display_0, 4, PinB5,  PinB6,  PinB7) \

#define DisplayEnum(e,width,dataPin,clockPin,latchPin) e,
enum { DisplaysList(DisplayEnum) };

#define Display(e,width,dataPin,clockPin,latchPin) {width,dataPin,clockPin,latchPin,(-1)},
display_t displays[] = { DisplaysList(Display) };

const u8 displaysCount = sizeof(displays)/sizeof(display_t);

void Displays_Init(void)
{
	for (u8 i=0; i<displaysCount; i++)
	{
		Display_Init(&displays[i]);
	}
}

void Display_Set(u8 displayIndex, u32 value)
{
	Display_SetValue(&displays[displayIndex], value);
}

void DisplayDot_Set(u8 displayIndex, u32 dot)
{
	Display_SetDot(&displays[displayIndex], dot);
}

u32 Display_GetValue(u8 displayIndex)
{
	return displays[displayIndex].value;
}

u32 DisplayDot_GetValue(u8 displayIndex)
{
	return displays[displayIndex].dot;
}