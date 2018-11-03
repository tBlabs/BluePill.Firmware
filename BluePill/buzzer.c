/*
 *
 *		BUZZER IS NOT WORKING
 *		
 *		DONT KNOW WHY
 *
 *
 *		Disabled in init and host!
 *
 *
 *
 *	Works on @A4
 *	Using Timer7.
 */

#include "buzzer.h"
#include <timer7.h>
#include <dac.h>
#include <typedefs.h>
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

static boolean isEnabled = true;
static u32 frequency = 4000;
static u32 amplitude = 1500;

void TIM7_IRQHandler(void)
{
	static u8 toggle = 1;

	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

		if ((toggle = 1 - toggle))
		{
			Dac1_SetAmplitude(amplitude);
		}
		else
		{
			Dac1_SetAmplitude(0);
		}
	}
}

void Buzzer1_Init(void)
{
	Buzzer1_Enable(true);   // Bez tego pojawia siê pisk zaraz po starcie
	Buzzer1_Enable(false);
}

void Buzzer1_Enable(boolean enable)
{
	isEnabled = enable;
	
	if (isEnabled)
	{
		Dac1_Enable();
		Timer7_Enable();
		Buzzer1_SetFrequency(frequency);
		Buzzer1_SetVolume(amplitude);
	}
	else
	{
		Timer7_Disable();
		Dac1_Disable();
	}
}

boolean Buzzer1_IsEnabled()
{
	return isEnabled;
}

void Buzzer1_SetVolume(u32 volume)
{
	amplitude = volume;
}

void Buzzer1_SetFrequency(u32 freq)
{
	frequency = freq;
	Timer7_SetPeriod(frequency);
}

u32 Buzzer1_GetVolume()
{
	return amplitude;
}

u32 Buzzer1_GetFrequency()
{
	return frequency;
}
