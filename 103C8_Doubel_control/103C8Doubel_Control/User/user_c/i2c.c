/***************************************
 * �ļ���  ��i2c.c
 * ����    ������i2c        
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�
 * ��汾  ��ST3.0.0  
 
**********************************************************************************/

#include "i2c.h"

//��ʼ��IIC����·
void IIC_Init(void)
{	
#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	   //ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = R_I2C_SCL_Pin|R_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;          //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(I2C_GPIO,R_I2C_SCL_Pin|R_I2C_SDA_Pin); 	   //PB4,PB5  6 �����
    GPIO_SetBits(I2C_GPIO,R_I2C_PG0_Pin); 
 
	GPIO_InitStructure.GPIO_Pin = L_I2C_SCL_Pin|L_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;          //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(I2C_GPIO,L_I2C_SCL_Pin|L_I2C_SDA_Pin); 	   //PB8,PB9 10 �����
    GPIO_SetBits(I2C_GPIO,L_I2C_PG0_Pin);
#endif
}

//����R_IIC��ʼ�ź�
void R_IIC_Start(void)
{
    R_SDA_OUT();    //SDA�����
    SCL_R_OUT_H;
    SDA_R_OUT_H; 
    delay_us(10);
    SDA_R_OUT_L;
    delay_us(10);
    SCL_R_OUT_L;   
}



//����R_IICֹͣ�ź�
void R_IIC_Stop(void)
{
    R_SDA_OUT(); //SDA�����
    SCL_R_OUT_L;
    SDA_R_OUT_L;
    
    delay_us(10);
    SCL_R_OUT_H;
    SDA_R_OUT_H;
    delay_us(10); 
}

//R_IIC�ȴ�ACK�ź�
u8 R_IIC_Wait_Ack(void) 				
{
    u8 ucErrTime=0;
    R_SDA_IN();   //sda����Ϊ����ģʽ
    
    SDA_R_OUT_H;  //ʱ�����
    delay_us(5);  //   
	SCL_R_OUT_H;  //
    delay_us(6);  //
    while(GPIO_ReadInputDataBit(I2C_GPIO, R_I2C_SDA_Pin))
    {
       ucErrTime++;
		if(ucErrTime>250)
		{
			R_IIC_Stop();
			return 1;
		} 
    }
    SCL_R_OUT_L;  
    return 0;
}

//R_IIC����ACK�ź�
void R_IIC_Ack(void)				
{
    SCL_R_OUT_L;   //Ϊ0
    R_SDA_OUT();
    SDA_R_OUT_L;
    delay_us(5);
    SCL_R_OUT_H;
    delay_us(6);
    SCL_R_OUT_L;

}
void R_IIC_NAck(void)				//IIC������ACK�ź�
{
    SCL_R_OUT_L;   //Ϊ0
    R_SDA_OUT();
    SDA_R_OUT_H;
    delay_us(5);
    SCL_R_OUT_H;
    delay_us(6);
    SCL_R_OUT_L;
}

//R_IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void R_IIC_Send_Byte(u8 txd)			//IIC����һ���ֽ�
{
    u8 t;   
	R_SDA_OUT(); 	     //�����������    
    SCL_R_OUT_L;         //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
       if((txd&0x80)>>7)
           SDA_R_OUT_H;
       else
           SDA_R_OUT_L;
        txd<<=1; 	  
		delay_us(5);     //��TEA5767��������ʱ���Ǳ����
		SCL_R_OUT_H;     //����ʱ��
		delay_us(5); 
		SCL_R_OUT_L;	 //����ʱ�ӿ�ʼ���ݴ���
		delay_us(6);
    }	
    
}

u8 R_IIC_Read_Byte(unsigned char ack)//IIC��ȡһ���ֽ�
{
    unsigned char i,receive=0;
	R_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SCL_R_OUT_L; 
        delay_us(5);
		SCL_R_OUT_H;
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_GPIO, R_I2C_SDA_Pin))receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        R_IIC_NAck();//����nACK
    else
        R_IIC_Ack(); //����ACK   
    return receive;

}




//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//

//����L_IIC��ʼ�ź�
void L_IIC_Start(void)
{
    L_SDA_OUT(); //SDA�����
    SCL_L_OUT_H;
    SDA_L_OUT_H;
    delay_us(10);
    SDA_L_OUT_L;
    delay_us(10);
    SCL_L_OUT_L;  
}
void L_IIC_Stop(void)//����IICֹͣ�ź�
{
    L_SDA_OUT(); //SDA�����
    SCL_L_OUT_L;
    SDA_L_OUT_L;
    
    delay_us(10);
    SCL_L_OUT_H;
    SDA_L_OUT_H;
    
    delay_us(10);
    
}

u8 L_IIC_Wait_Ack(void)				//IIC�ȴ�ACK�ź�
{
    u8 ucErrTime=0;
    L_SDA_IN();  //sda����Ϊ����ģʽ
    
    SDA_L_OUT_H;
    delay_us(5);	   
	SCL_L_OUT_H;
    delay_us(6);
    while(GPIO_ReadInputDataBit(I2C_GPIO, L_I2C_SDA_Pin))
    {
       ucErrTime++;
		if(ucErrTime>250)
		{
			L_IIC_Stop();
			return 1;
		} 
    }
    SCL_L_OUT_L;
    
    return 0;
    
}
void L_IIC_Ack(void)				//IIC����ACK�ź�
{
    SCL_L_OUT_L;//Ϊ0
    L_SDA_OUT();
    SDA_L_OUT_L;
    delay_us(5);
    SCL_L_OUT_H;
    delay_us(6);
    SCL_L_OUT_L;
}

void L_IIC_NAck(void)			//IIC������ACK�ź�
{
    SCL_L_OUT_L;//Ϊ0
    L_SDA_OUT();
    SDA_L_OUT_H;
    delay_us(5);
    SCL_L_OUT_H;
    delay_us(6);
    SCL_L_OUT_L;
    

}

void L_IIC_Send_Byte(u8 txd)			//IIC����һ���ֽ�
{
   
    u8 t;   
	L_SDA_OUT(); 	     //�����������    
    SCL_L_OUT_L;         //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
            SDA_L_OUT_H;
        
        else
            SDA_L_OUT_L;
            
        txd<<=1; 	  
		delay_us(5);     //��TEA5767��������ʱ���Ǳ����
		SCL_L_OUT_H;     //����ʱ��
		delay_us(5); 
		SCL_L_OUT_L;	 //����ʱ�ӿ�ʼ���ݴ���
		delay_us(6);
    }	 

}
u8 L_IIC_Read_Byte(unsigned char ack)//IIC��ȡһ���ֽ�
{
    unsigned char i,receive=0;
	L_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        SCL_L_OUT_L; //left����low
        delay_us(5);
		SCL_L_OUT_H;
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_GPIO, L_I2C_SDA_Pin))receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        L_IIC_NAck();//����nACK
    else
        L_IIC_Ack(); //����ACK   
    return receive;
}


