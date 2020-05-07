#ifndef __I2C_H
#define	__I2C_H

#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
/*****�궨��******/
//I2C��GPIO
#define I2C_GPIO         GPIOB 
#define R_I2C_SCL_Pin    GPIO_Pin_4
#define R_I2C_SDA_Pin    GPIO_Pin_5
#define R_I2C_PG0_Pin    GPIO_Pin_6

#define L_I2C_SCL_Pin    GPIO_Pin_8
#define L_I2C_SDA_Pin    GPIO_Pin_9
#define L_I2C_PG0_Pin    GPIO_Pin_7
//IO�������� ��������


#define R_SDA_IN()  {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=8<<20;}
#define R_SDA_OUT() {GPIOB->CRL&=0XFF0FFFFF;GPIOB->CRL|=3<<20;}

#define L_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}
#define L_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}




//IO��������	
#define SCL_R_OUT_H   GPIO_SetBits(I2C_GPIO ,R_I2C_SCL_Pin)
#define SCL_R_OUT_L   GPIO_ResetBits(I2C_GPIO ,R_I2C_SCL_Pin)

#define SDA_R_OUT_H   GPIO_SetBits(I2C_GPIO ,R_I2C_SDA_Pin)
#define SDA_R_OUT_L   GPIO_ResetBits(I2C_GPIO ,R_I2C_SDA_Pin)

#define SCL_L_OUT_H   GPIO_SetBits(I2C_GPIO ,L_I2C_SCL_Pin)
#define SCL_L_OUT_L   GPIO_ResetBits(I2C_GPIO ,L_I2C_SCL_Pin)

#define SDA_L_OUT_H   GPIO_SetBits(I2C_GPIO ,L_I2C_SDA_Pin)
#define SDA_L_OUT_L   GPIO_ResetBits(I2C_GPIO ,L_I2C_SDA_Pin)

//IIC���в�������
void IIC_Init(void);                   //��ʼ��IIC��IO��				 
void R_IIC_Start(void);				    //����IIC��ʼ�ź�
void R_IIC_Stop(void);	  			    //����IICֹͣ�ź�
void R_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 R_IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
u8 R_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void R_IIC_Ack(void);					//IIC����ACK�ź�
void R_IIC_NAck(void);				    //IIC������ACK�ź�

//void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
void L_IIC_Start(void);				    //����IIC��ʼ�ź�
void L_IIC_Stop(void);	  			    //����IICֹͣ�ź�
void L_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 L_IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
u8 L_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void L_IIC_Ack(void);					//IIC����ACK�ź�
void L_IIC_NAck(void);				    //IIC������ACK�ź�

#endif /* __I2C_H */

