/** Hardware/CAN/can.h */
#ifndef __CAN_H
#define __CAN_H
#include "sys.h"
#include "stm32f10x_can.h"

#define CAN_INT_ENABLE 0 // 是否开启 CAN 总线接收中断模式（1 开启，0 关闭）
/* 设置波特率 = PCLK1 ÷ ((1+8+7)*9)) = 36MHz ÷ 16 ÷ 9 = 250Kbits   */
/* 设置波特率 64M/2(APB1分频)/60/ (1 + 8 + 7) = 32MHz ÷ 16 ÷ 9 =2/9 */
#define tsjw CAN_SJW_1tq // 取值范围 1 ~ 4       1
#define tbs1 CAN_BS1_6tq  //CAN_BS1_8tq // 取值范围 1 ~ 16      6
#define tbs2 CAN_BS2_1tq // 取值范围 1 ~ 8       1

#define brp 16            // 设定了 1 个时间单位的长度为       16
/* 设置波特率 64M/2(APB1分频)/8/ (1 + 8 + 7) = 32MHz ÷ 16 ÷ 8 = 250Kbits */
u8 CAN1_Configuration(void);        // CAN 总线初始化函数
u8 CAN_Send_Msg(u8 *msg, u8 len);   // CAN 总线数据发送函数
u8 CAN_Receive_Msg(u8 *buf);        // CAN 总线数据接收函数
#endif





