#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
/*****�궨��******/
/*****ADC��غ궨��******/
#define ADC_GPIO          GPIOA
#define L_R_VOUT_GPIO     GPIOB

#define ADC_5V_Pin        GPIO_Pin_0
#define L_VOUT_Pin        GPIO_Pin_0
#define R_VOUT_Pin        GPIO_Pin_1

void ADC123_Init(void);

#endif /* __ADC_H */
