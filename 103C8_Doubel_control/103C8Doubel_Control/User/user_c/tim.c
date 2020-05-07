/***************************************
 * �ļ���  ��tim.c
 * ����    ������TIM         
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�
 * ��汾  ��ST3.0.0  
 
**********************************************************************************/

#include "tim.h"

void TIM1_Init(void)
{
    
    GPIO_InitTypeDef        GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM1_TimeBaseStructure;
	TIM_OCInitTypeDef       TIM1_OCInitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	/* TIM1 Registers reset */
	/* Enable TIM1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//ʹ��APB2ʱ��
	TIM_DeInit(TIM1);
	
	/*PA8 Ϊ������buzzer*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM1_TimeBaseStructure);
	/* Time Base configuration */
    
    TIM1_TimeBaseStructure.TIM_Period    = 2000-1;      //2000
	TIM1_TimeBaseStructure.TIM_Prescaler = 0x0;         //64M����Ƶ��16K
	TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;//���¼���ģʽ
	
	TIM1_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0; //0
	TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);

	TIM1->CR2=0X0020;//�����¼���Ϊ����Դ

	TIM_OCStructInit(&TIM1_OCInitStructure);
	
	
//	/* Channel 1, 2,3 in PWM mode */
	TIM1_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;   //PWMģʽ1λ������ֵС��ccRx�Ƚ�ֵʱ��Ϊ��Ч
	TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM1_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Disable;	
	TIM1_OCInitStructure.TIM_Pulse       = 900;                           //dummy value 500
	TIM1_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;      //����Ϊ����Ч

	TIM_OC1Init(TIM1, &TIM1_OCInitStructure); 

	/* Enables the TIM1 Preload on CC1 Register */
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
//	/* Enables the TIM1 Preload on CC2 Register */
//	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
//	/* Enables the TIM1 Preload on CC3 Register */
//	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);


	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;        //TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIM��ARR�ϵ�Ԥװ�ؼĴ���
    
    
	TIM_CtrlPWMOutputs(TIM1, DISABLE);  //ʧ��PWM���
//    TIM_CtrlPWMOutputs(TIM1, ENABLE);  //ʧ��PWM���
    
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_Cmd(TIM1, ENABLE);
}


u16 VOLT_5V=0;
u16 VOLT_L=0;
u16 VOLT_R=0;

u16 counter_1ms=0;
u8 counter_10ms=0;
u8 counter_100ms=0;
u8 counter_1ms_flag=0;
u8 counter_10ms_flag=0;
u8 counter_100ms_flag=0;
//16KPWM��Ƶ��
void TIM1_UP_IRQHandler(void)
{
//	static u8 ucHallLast=0;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update); 

        counter_1ms++;    
        if(counter_1ms==16)   //16k��pwm����16��Ϊ1ms������16000λһ��
        {
            counter_1ms_flag=1;
            counter_10ms++;
            counter_1ms=0;
        }
        if(counter_10ms==10)
        {
            counter_10ms_flag=1;
            counter_100ms++;
            counter_10ms=0;
        }
        if(counter_100ms==10)
        {
            counter_100ms_flag=1;
            counter_100ms=0;
        }
        
        
		if((TIM1->RCR==0)&&((TIM1->CNT)>100))
		{
			TIM1->RCR=1;      // ���Ȳ��������¼�
		}
        VOLT_5V = ADC1->JDR1; //5V��ѹ        
		VOLT_L  = ADC1->JDR2; //L_volt
		VOLT_R  = ADC1->JDR3; //R_volt    
	}
}

