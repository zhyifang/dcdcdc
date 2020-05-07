/** Hardware/CAN/can.h */
#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
#include "stm32f10x_can.h"

#define CAN_INT_ENABLE 0 // �Ƿ��� CAN ���߽����ж�ģʽ��1 ������0 �رգ�
/* ���ò����� = PCLK1 �� ((1+8+7)*9)) = 36MHz �� 16 �� 9 = 250Kbits   */
/* ���ò����� 64M/2(APB1��Ƶ)/60/ (1 + 8 + 7) = 32MHz �� 16 �� 9 =2/9 */
#define tsjw CAN_SJW_1tq // ȡֵ��Χ 1 ~ 4       1
#define tbs1 CAN_BS1_6tq  //CAN_BS1_8tq // ȡֵ��Χ 1 ~ 16      6
#define tbs2 CAN_BS2_1tq // ȡֵ��Χ 1 ~ 8       1

#define brp 16            // �趨�� 1 ��ʱ�䵥λ�ĳ���Ϊ       16
/* ���ò����� 64M/2(APB1��Ƶ)/8/ (1 + 8 + 7) = 32MHz �� 16 �� 8 = 250Kbits */
u8 CAN1_Configuration(void);        // CAN ���߳�ʼ������
u8 CAN_Send_Msg(u8 *msg, u8 len);   // CAN �������ݷ��ͺ���
u8 CAN_Receive_Msg(u8 *buf);        // CAN �������ݽ��պ���
#endif





