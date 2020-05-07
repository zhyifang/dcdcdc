/***************************************
 * 文件名  ：adc.c
 * 描述    ：配置ADC         
 * 实验平台：STM32 基于STM32F103C8T6
 * 硬件连接：
 * 库版本  ：ST3.0.0  
 
**********************************************************************************/
#include "adc.h"

void ADC123_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB, ENABLE);

    //GPIO的左右的角度传感器的电压采样值
	GPIO_InitStructure.GPIO_Pin = L_VOUT_Pin|R_VOUT_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(L_R_VOUT_GPIO, &GPIO_InitStructure);
	//ADC_5V
	GPIO_InitStructure.GPIO_Pin = ADC_5V_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_GPIO, &GPIO_InitStructure);
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   // 独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;         // 扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 连续转换不是能
    
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 外部触发禁止
	
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_NbrOfChannel = 2;//规则通道数2，暂时没用到
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);//注入组触发源为TIM1的触发
	ADC_InjectedSequencerLengthConfig(ADC1,3);//注入组长度

	/* ADC1 regular channel14 configuration */ 

	ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);//注入组采样序列1,5V电压检测
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_1Cycles5);//注入组采样序列2,L_VOLT
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_1Cycles5);//注入组采样序列3,R_VOLT

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibaration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibaration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);     
}




