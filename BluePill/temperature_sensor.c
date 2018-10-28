#include "temperature_sensor.h"
#include <spi2.h>
#include <typedefs.h>
#include <timeout.h>
#include <debug.h>

void TemperatureSensor_Init(void)
{
	Spi2_Init();
}

u32 TemperatureSensor_GetTemperature(void)
{
	static u32 temperature;
	static u32 sum;
	static u16 samplesCount;
	static timeout_t readSensorMoment;

	if (Tick(&readSensorMoment, 100))
	{
		u32 temp = 0;

		u16 data = Spi2_Read();

		if ((data & 0b100)==0) return 0;
		else
		{
			temp = data >> 3;
			temp *= 625;
			temp /= 1000;
		}

		sum += temp;

		if (samplesCount >= 20)
		{

			temperature = (temperature_t)(sum/samplesCount);
			sum = 0;
			samplesCount = 0;
		}
		samplesCount++;
	}

	return temperature;
}
