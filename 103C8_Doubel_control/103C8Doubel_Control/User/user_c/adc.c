/***************************************
 * �ļ���  ��adc.c
 * ����    ������ADC         
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�
 * ��汾  ��ST3.0.0  
 
**********************************************************************************/
#include "adc.h"

void ADC123_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB, ENABLE);

    //GPIO�����ҵĽǶȴ������ĵ�ѹ����ֵ
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
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;   // ��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;         // ɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ����ת��������
    
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// �ⲿ������ֹ
	
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_NbrOfChannel = 2;//����ͨ����2����ʱû�õ�
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);//ע���鴥��ԴΪTIM1�Ĵ���
	ADC_InjectedSequencerLengthConfig(ADC1,3);//ע���鳤��

	/* ADC1 regular channel14 configuration */ 

	ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);//ע�����������1,5V��ѹ���
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_1Cycles5);//ע�����������2,L_VOLT
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_1Cycles5);//ע�����������3,R_VOLT

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




