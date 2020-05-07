#ifndef __BUZZER_H
#define	__BUZZER_H

#include "stm32f10x.h"
#include <stdio.h>
#include "gpio.h"
/*****∫Í∂®“Â******/
#define BUZZER_PORT        GPIOA
#define BUZZER_Pin         GPIO_Pin_8
#define BUZZER_TIME        30
#define BUZZER_LONG_TIME   60
typedef struct
{
	uint8_t Buzzer_on_flag;
	uint8_t Buzzer_one_flag;
	uint8_t Buzzer_two_flag;
	uint8_t Buzzer_beep_cnt;
    uint8_t Buzzer_long_flag;
	uint8_t Buzzer_beep_times;
	uint8_t Buzzer_delay_cnt1;
	uint8_t Buzzer_delay_cnt2;
	uint8_t Buzzer_delay_cnt3;
}BUZZER_Handle_t;

extern BUZZER_Handle_t Buzzer_Handle;

void  BUZZER_On(void);
void  BUZZER_Off(void);
void  BUZZER_Work(void);
#endif /* __BUZZER_H */

