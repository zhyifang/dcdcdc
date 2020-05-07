/***************************************
 * 文件名  ：i2c.c
 * 描述    ：配置i2c        
 * 实验平台：STM32 基于STM32F103C8T6
 * 硬件连接：
 * 库版本  ：ST3.0.0  
 
**********************************************************************************/

#include "i2c.h"

//初始化IIC，两路
void IIC_Init(void)
{	
#if 1
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	   //使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = R_I2C_SCL_Pin|R_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;          //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(I2C_GPIO,R_I2C_SCL_Pin|R_I2C_SDA_Pin); 	   //PB4,PB5  6 输出高
    GPIO_SetBits(I2C_GPIO,R_I2C_PG0_Pin); 
 
	GPIO_InitStructure.GPIO_Pin = L_I2C_SCL_Pin|L_I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;          //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	GPIO_SetBits(I2C_GPIO,L_I2C_SCL_Pin|L_I2C_SDA_Pin); 	   //PB8,PB9 10 输出高
    GPIO_SetBits(I2C_GPIO,L_I2C_PG0_Pin);
#endif
}

//产生R_IIC起始信号
void R_IIC_Start(void)
{
    R_SDA_OUT();    //SDA线输出
    SCL_R_OUT_H;
    SDA_R_OUT_H; 
    delay_us(10);
    SDA_R_OUT_L;
    delay_us(10);
    SCL_R_OUT_L;   
}



//产生R_IIC停止信号
void R_IIC_Stop(void)
{
    R_SDA_OUT(); //SDA线输出
    SCL_R_OUT_L;
    SDA_R_OUT_L;
    
    delay_us(10);
    SCL_R_OUT_H;
    SDA_R_OUT_H;
    delay_us(10); 
}

//R_IIC等待ACK信号
u8 R_IIC_Wait_Ack(void) 				
{
    u8 ucErrTime=0;
    R_SDA_IN();   //sda设置为输入模式
    
    SDA_R_OUT_H;  //时间控制
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

//R_IIC发送ACK信号
void R_IIC_Ack(void)				
{
    SCL_R_OUT_L;   //为0
    R_SDA_OUT();
    SDA_R_OUT_L;
    delay_us(5);
    SCL_R_OUT_H;
    delay_us(6);
    SCL_R_OUT_L;

}
void R_IIC_NAck(void)				//IIC不发送ACK信号
{
    SCL_R_OUT_L;   //为0
    R_SDA_OUT();
    SDA_R_OUT_H;
    delay_us(5);
    SCL_R_OUT_H;
    delay_us(6);
    SCL_R_OUT_L;
}

//R_IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void R_IIC_Send_Byte(u8 txd)			//IIC发送一个字节
{
    u8 t;   
	R_SDA_OUT(); 	     //数据线输出高    
    SCL_R_OUT_L;         //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
       if((txd&0x80)>>7)
           SDA_R_OUT_H;
       else
           SDA_R_OUT_L;
        txd<<=1; 	  
		delay_us(5);     //对TEA5767这三个延时都是必须的
		SCL_R_OUT_H;     //拉高时钟
		delay_us(5); 
		SCL_R_OUT_L;	 //拉低时钟开始数据传输
		delay_us(6);
    }	
    
}

u8 R_IIC_Read_Byte(unsigned char ack)//IIC读取一个字节
{
    unsigned char i,receive=0;
	R_SDA_IN();//SDA设置为输入
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
        R_IIC_NAck();//发送nACK
    else
        R_IIC_Ack(); //发送ACK   
    return receive;

}




//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------//

//发送L_IIC开始信号
void L_IIC_Start(void)
{
    L_SDA_OUT(); //SDA线输出
    SCL_L_OUT_H;
    SDA_L_OUT_H;
    delay_us(10);
    SDA_L_OUT_L;
    delay_us(10);
    SCL_L_OUT_L;  
}
void L_IIC_Stop(void)//发送IIC停止信号
{
    L_SDA_OUT(); //SDA线输出
    SCL_L_OUT_L;
    SDA_L_OUT_L;
    
    delay_us(10);
    SCL_L_OUT_H;
    SDA_L_OUT_H;
    
    delay_us(10);
    
}

u8 L_IIC_Wait_Ack(void)				//IIC等待ACK信号
{
    u8 ucErrTime=0;
    L_SDA_IN();  //sda设置为输入模式
    
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
void L_IIC_Ack(void)				//IIC发送ACK信号
{
    SCL_L_OUT_L;//为0
    L_SDA_OUT();
    SDA_L_OUT_L;
    delay_us(5);
    SCL_L_OUT_H;
    delay_us(6);
    SCL_L_OUT_L;
}

void L_IIC_NAck(void)			//IIC不发送ACK信号
{
    SCL_L_OUT_L;//为0
    L_SDA_OUT();
    SDA_L_OUT_H;
    delay_us(5);
    SCL_L_OUT_H;
    delay_us(6);
    SCL_L_OUT_L;
    

}

void L_IIC_Send_Byte(u8 txd)			//IIC发送一个字节
{
   
    u8 t;   
	L_SDA_OUT(); 	     //数据线输出高    
    SCL_L_OUT_L;         //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
            SDA_L_OUT_H;
        
        else
            SDA_L_OUT_L;
            
        txd<<=1; 	  
		delay_us(5);     //对TEA5767这三个延时都是必须的
		SCL_L_OUT_H;     //拉高时钟
		delay_us(5); 
		SCL_L_OUT_L;	 //拉低时钟开始数据传输
		delay_us(6);
    }	 

}
u8 L_IIC_Read_Byte(unsigned char ack)//IIC读取一个字节
{
    unsigned char i,receive=0;
	L_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        SCL_L_OUT_L; //left——low
        delay_us(5);
		SCL_L_OUT_H;
        receive<<=1;
        if(GPIO_ReadInputDataBit(I2C_GPIO, L_I2C_SDA_Pin))receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        L_IIC_NAck();//发送nACK
    else
        L_IIC_Ack(); //发送ACK   
    return receive;
}


