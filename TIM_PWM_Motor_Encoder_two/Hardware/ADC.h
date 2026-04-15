#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void ADC_Battery_Init(void);
uint16_t ADC_Battery_GetValue(void);
float ADC_Battery_GetVoltage(void);
float ADC_Battery_GetVoltageAvg(void);
uint8_t ADC_Battery_GetPercent(float voltage);

#endif
