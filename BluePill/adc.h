#ifndef ADC_H_
#define ADC_H_

#include "typedefs.h"

void Adc_Init(void);
u32 Adc_GetValue(u8 adcIndex);
void AdcTask(void);

#endif /* ADC_H_ */
