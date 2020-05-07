/***************************************
 * 文件名  ：buzzer.c
 * 描述    ：配置ADC         
 * 实验平台：STM32 基于STM32F103C8T6
 * 硬件连接：
 * 库版本  ：ST3.0.0  
 
**********************************************************************************/

#include "buzzer.h"

BUZZER_Handle_t Buzzer_Handle;
void Buzzer_Init(void)
{
//     GPIO_InitTypeDef         GPIO_InitStructre;
}

void  BUZZER_On(void)
{
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);    
}
void  BUZZER_Off(void)
{
    TIM_CtrlPWMOutputs(TIM1, DISABLE); 
}

void  BUZZER_Work(void)
{

    if(Buzzer_Handle.Buzzer_one_flag == 1)
	{
		Buzzer_Handle.Buzzer_delay_cnt2++;
		if(Buzzer_Handle.Buzzer_delay_cnt2>BUZZER_TIME)        //控制时间
		{
			Buzzer_Handle.Buzzer_delay_cnt2 = 0;
			BUZZER_Off();
			Buzzer_Handle.Buzzer_one_flag = 0;
            if(Open_Close_Switch_Handle.Open_Flag==1)
            {
                Open_Close_Switch_Handle.Open_State=1;
                Open_Close_Switch_Handle.Close_State=0;
                Open_Close_Switch_Handle.Close_Flag=0;
            }
            
		}
		if(Buzzer_Handle.Buzzer_delay_cnt2==1)
		{
			BUZZER_On();
		}
	}
	else if(Buzzer_Handle.Buzzer_two_flag == 1)
	{
		Buzzer_Handle.Buzzer_delay_cnt3++;
		if(Buzzer_Handle.Buzzer_delay_cnt3>BUZZER_TIME)        //控制时间
		{
			Buzzer_Handle.Buzzer_delay_cnt3 = 0;
			BUZZER_Off();
			if(++Buzzer_Handle.Buzzer_beep_cnt>=2)    //Buzzer_Handle.Buzzer_beep_times
			{
				Buzzer_Handle.Buzzer_two_flag = 0;
				Buzzer_Handle.Buzzer_beep_cnt = 0;
			}
		}
		if(Buzzer_Handle.Buzzer_delay_cnt3==1)
		{
			BUZZER_On();
		}
	}
    else if(Buzzer_Handle.Buzzer_long_flag == 1)
	{
		Buzzer_Handle.Buzzer_delay_cnt1++;
	    if(Buzzer_Handle.Buzzer_delay_cnt1>BUZZER_LONG_TIME)        //控制时间
		{
			Buzzer_Handle.Buzzer_delay_cnt1 = 0;
			BUZZER_Off();
			Buzzer_Handle.Buzzer_long_flag = 0; 
		}
		if(Buzzer_Handle.Buzzer_delay_cnt1==1)
		{
			BUZZER_On();
		}
	}
	else
	{
		Buzzer_Handle.Buzzer_one_flag = 0;
		Buzzer_Handle.Buzzer_two_flag = 0;
		BUZZER_Off();
	}

}
