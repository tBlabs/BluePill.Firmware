#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usart1.h"
#include "usart2.h"
#include "host.h"
#include "adc.h"
#include "inputs.h"
#include "outputs.h"
#include "pwms.h"
#include "systemClock.h"
#include "buzzer.h"
#include "displays.h"
#include "systick.h"
#include "temperature_sensor.h"

void SystemSetup(void)
{
	SystemClockInit();
	SysTickInit();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// Disable JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  // Without it A14, B3 and B4 pins can not be used as standard IO

	Usart1_Init();
}

void IoSetup()
{
	Inputs_Init();
	Adc_Init();
	RTC_Init();
	Outputs_Init();
	Pwm_Init();
	Buzzer1_Init();
	Displays_Init();
}

void IoTasks()
{
	AdcTask();
}

int main(void)
{
	SystemSetup();

 	IoSetup();

	while (true)
	{
		HostTask();
		
		IoTasks();
	}
}

