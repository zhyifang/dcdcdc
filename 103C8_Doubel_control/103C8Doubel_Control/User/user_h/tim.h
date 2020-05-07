#ifndef __TIM_H
#define	__TIM_H

#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_tim.h"
extern u8 counter_1ms_flag;
extern u8 counter_10ms_flag;
extern u8 counter_100ms_flag;

void TIM1_Init(void);
void TIM1_UP_IRQHandler(void);


#endif /* __TIM_H */
