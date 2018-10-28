/*
 *         +----------+
 *         |          |     led
 *         |        A4|-----|>|---[100Om]--- (-)
 *         |    uC    |
 * 
 * DAC outputs:
 * DAC1 - A.4 <--- selected
 * DAC2 - A.5
 */

#include "dac.h"
#include "stm32f10x.h"
#include "stm32f10x_dac.h"
#include "typedefs.h"
#include "pins.h"
#include "debug.h"
#include <delay.h>

void Dac1_Enable(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	Pin_Init(&(pin_t)PinA4, GPIO_Mode_AIN);

	DAC_InitTypeDef DAC_InitStructure;
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}

void Dac1_Disable(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, DISABLE);
	DAC_Cmd(DAC_Channel_1, DISABLE);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, DISABLE);
}

void Dac1_SetAmplitude(u12 value)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, value);
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}
