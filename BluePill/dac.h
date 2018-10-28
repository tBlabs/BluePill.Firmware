/*
 * dac.h
 *
 *  Created on: Nov 29, 2015
 *      Author: tB
 */

#ifndef DAC_H_
#define DAC_H_


#include "typedefs.h"


void Dac1_Enable(void);
void Dac1_Disable(void);
void Dac1_SetAmplitude(u12 value);
void DAC1_Test(void);

#endif /* DAC_H_ */
