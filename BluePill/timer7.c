#include "timer7.h"
#include "dac.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "debug.h"
#include "global.h"
#include "general.h"

void Timer7_Disable(void)
{
	TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
	TIM_Cmd(TIM7, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);
}

void Timer7_SetPeriod(u16 period)
{
	if (period == 0) return;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 12 - 1;   // 24MHz / 12 = 2MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = (u16)(1000000UL / (u32)period) - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;

	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
}

void Timer7_Enable(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
////////////////	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);

	// Interrupt enable
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// Enable counter
	TIM_Cmd(TIM7, ENABLE);
}
