/*
	ADC pins:
	@A0 - ADC_IN0 
	@A1 - ADC_IN1 
	@A2 - ADC_IN2 
	@A3 - ADC_IN3
 */

#include "adc.h"
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
#include "pins.h"

typedef struct
{
	pin_t pin;
	u8 channel;
	u12 oldValue;    // for engine
	u12 value;
	u12 val;
	u32 samplesSum;
	u16 sampleIndex;
}
adc_t;

// ENUM | PIN | ADC CHANNEL
#define AdcChannelsList(channel) \
	channel(adcChannel_1, 		PinA0,	ADC_Channel_0) \
	channel(adcChannel_2, 		PinA1,	ADC_Channel_1) \
	channel(adcChannel_3, 		PinA2,	ADC_Channel_2) \
	channel(adcChannel_4, 		PinA3,	ADC_Channel_3) \

#define AdcChannelEnum(e,pin,channel) e,
enum { AdcChannelsList(AdcChannelEnum) }
;

#define AdcChannel(e,pin,channel) {pin,channel},
adc_t adcChannels[] = { AdcChannelsList(AdcChannel) };

#define adcChannelsCount (sizeof(adcChannels)/sizeof(adc_t))

volatile uint16_t adcValues[adcChannelsCount];

#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define RANGE_CHANGE		41	// Adc is 12-bit: 0-4095


static void DMA_Conf(void)
{
	RCC->AHBENR |= 1;    // DMA Clock enable

	DMA_DeInit(DMA1_Channel1);

	DMA_InitTypeDef DMA_InitStruct;
	DMA_InitStruct.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStruct.DMA_MemoryBaseAddr = (u32)&adcValues;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStruct.DMA_BufferSize = adcChannelsCount;
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStruct);

	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*
 * Channel #0 - @A0
 * Channel #1 - @A1
 * Temperature (channel #16) - @internal
 */
void Adc_Init(void)
{
	// Enable ADC clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);

	//IoInit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, GPIO_Mode_AIN);
	for(u8 i = 0 ; i < adcChannelsCount ; i++)
	{
		Pin_Init(&adcChannels[i].pin, GPIO_Mode_AIN);
	}

	DMA_Conf();

	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfChannel = adcChannelsCount;

	ADC_Init(ADC1, &ADC_InitStruct);

	u8 rank = 1;

	for (u8 i = 0; i < adcChannelsCount; i++)
	{
		ADC_RegularChannelConfig(ADC1, adcChannels[i].channel, rank++, ADC_SampleTime_71Cycles5);
	}

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);


	ADC_ResetCalibration(ADC1);

	while (ADC_GetResetCalibrationStatus(ADC1)) {}
	;

	ADC_StartCalibration(ADC1);

	while (ADC_GetCalibrationStatus(ADC1)) {}
	;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

#define MAX_SAMPLES_COUNT	1024 // max value: {max of u32}/{adc max value}=1'048'576

static void Adc_Engine(adc_t * adc)
{
	u16 adcValue = (u16)adcValues[adc->channel];

	adc->samplesSum += adcValue;

	if ((++adc->sampleIndex) >= MAX_SAMPLES_COUNT)
	{
		adc->value = adc->samplesSum / MAX_SAMPLES_COUNT;
		adc->val = adc->value / 10;
		adc->samplesSum = 0;
		adc->sampleIndex = 0;
	}
}

void AdcTask(void)
{
	for (u8 i = 0; i < adcChannelsCount; i++)
	{
		Adc_Engine(&adcChannels[i]);
	}
}

u32 Adc_GetValue(u8 adcIndex)
{
	 return adcChannels[adcIndex].val;
}

