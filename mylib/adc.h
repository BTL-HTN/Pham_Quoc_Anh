#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

#define ADC_CH0    0
#define ADC_CH1    1
#define ADC_CH2    2
#define ADC_CH3    3
#define ADC_CH4    4
#define ADC_CH5    5
#define ADC_CH6    6
#define ADC_CH7    7
#define ADC_CH8    8
#define ADC_CH9    9

// Ð?i tên t? ADC_Init thành ADC_Config_Channel d? tránh xung d?t
void ADC_Config_Channel(uint8_t channel);
uint16_t ADC_Read(void);

#endif
// Nh? ?n Enter ? dây d? t?o 1 dòng tr?ng cu?i file