#ifndef __I2C_H
#define	__I2C_H

#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
/*****宏定义******/
//I2C的GPIO
#define I2C_GPIO         GPIOB 
#define R_I2C_SCL_Pin    GPIO_Pin_4
#define R_I2C_SDA_Pin    GPIO_Pin_5
#define R_I2C_PG0_Pin    GPIO_Pin_6

#define L_I2C_SCL_Pin    GPIO_Pin_8
#define L_I2C_SDA_Pin    GPIO_Pin_9
#define L_I2C_PG0_Pin    GPIO_Pin_7
//IO方向设置 输入和输出


#define R_SDA_IN()  {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=8<<20;}
#define R_SDA_OUT() {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=3<<20;}

#define L_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define L_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}




//IO操作函数	
#define SCL_R_OUT_H   GPIO_SetBits(I2C_GPIO ,R_I2C_SCL_Pin)
#define SCL_R_OUT_L   GPIO_ResetBits(I2C_GPIO ,R_I2C_SCL_Pin)

#define SDA_R_OUT_H   GPIO_SetBits(I2C_GPIO ,R_I2C_SDA_Pin)
#define SDA_R_OUT_L   GPIO_ResetBits(I2C_GPIO ,R_I2C_SDA_Pin)

#define SCL_L_OUT_H   GPIO_SetBits(I2C_GPIO ,L_I2C_SCL_Pin)
#define SCL_L_OUT_L   GPIO_ResetBits(I2C_GPIO ,L_I2C_SCL_Pin)

#define SDA_L_OUT_H   GPIO_SetBits(I2C_GPIO ,L_I2C_SDA_Pin)
#define SDA_L_OUT_L   GPIO_ResetBits(I2C_GPIO ,L_I2C_SDA_Pin)

//IIC所有操作函数
void IIC_Init(void);                   //初始化IIC的IO口				 
void R_IIC_Start(void);				    //发送IIC开始信号
void R_IIC_Stop(void);	  			    //发送IIC停止信号
void R_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 R_IIC_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 R_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void R_IIC_Ack(void);					//IIC发送ACK信号
void R_IIC_NAck(void);				    //IIC不发送ACK信号

//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
void L_IIC_Start(void);				    //发送IIC开始信号
void L_IIC_Stop(void);	  			    //发送IIC停止信号
void L_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 L_IIC_Read_Byte(unsigned char ack); //IIC读取一个字节
u8 L_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void L_IIC_Ack(void);					//IIC发送ACK信号
void L_IIC_NAck(void);				    //IIC不发送ACK信号

#endif /* __I2C_H */

