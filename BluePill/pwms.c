#include "pwms.h"
#include "timer3.h"
#include "general.h"
#include "stm32f10x.h"
#include "typedefs.h"

/*
 *  * Timer 3 remap options:
 * - no remap:
 * 		ch1 @A6	<-- used for STM32F103C8_BOARD
 * 		ch2 @A7	<-- used for STM32F103C8_BOARD
 * 		ch3 @B0	<-- used for STM32F103C8_BOARD
 * 		ch4 @B1	<-- used for STM32F103C8_BOARD
 * - partial remap:
 * 		ch1 @B4
 * 		ch2 @B5
 * 		ch3 @B0
 * 		ch4 @B1
 */

static u32 pwmValues[4] = { 0, 0, 0, 0 };

void Pwm_Init(void)
{
	Timer3_Init();
}

void Pwm_Set(u8 pwmIndex, u32 value)
{
	if (value == pwmValues[pwmIndex]) return ;
	
	pwmValues[pwmIndex] = value;
	
	switch (pwmIndex)
	{
		case 0: Timer3Channel1Duty(value); break;
		case 1: Timer3Channel2Duty(value); break;
		case 2: Timer3Channel3Duty(value); break;
		case 3: Timer3Channel4Duty(value); break;
	}
}

u32 Pwm_GetValue(u8 pwmIndex)
{
	return pwmValues[pwmIndex];
}