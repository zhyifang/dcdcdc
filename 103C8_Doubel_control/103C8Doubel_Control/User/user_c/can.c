/** Hardware/CAN/can.c */
#include "can.h"
/** CAN 总线初始化函数，返回 0 表示成功，其它表示失败 */
u8 CAN1_Configuration(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;

#if CAN_INT_ENABLE
  NVIC_InitTypeDef NVIC_InitStructure;
#endif
  /* 设置时钟 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能 GPIOA 分组时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  // 使能 CAN1 总线时钟

  /* 设置 GPIO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       // 复用推挽输出模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         // 上拉输入模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 设置 CAN 总线功能 */
  CAN_InitStructure.CAN_TTCM = DISABLE;         // 关闭时间触发通信模式
  CAN_InitStructure.CAN_ABOM = DISABLE;         // 关闭软件自动离线管理
  CAN_InitStructure.CAN_AWUM = DISABLE;         // 关闭睡眠模式通过软件唤醒
  CAN_InitStructure.CAN_NART = ENABLE;          // 开启禁止报文自动传送
  CAN_InitStructure.CAN_RFLM = DISABLE;         // 关闭报文锁定
  CAN_InitStructure.CAN_TXFP = DISABLE;         // 优先级由报文标识符确定
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN_Mode_Normal 普通模式，CAN_Mode_LoopBack 回环模式

  /* 设置 CAN 总线波特率 */
  CAN_InitStructure.CAN_SJW = tsjw;
  CAN_InitStructure.CAN_BS1 = tbs1;
  CAN_InitStructure.CAN_BS2 = tbs2;
  CAN_InitStructure.CAN_Prescaler = brp;
  CAN_Init(CAN1, &CAN_InitStructure);                                  // 初始化 CAN1

  /* 设置 CAN 总线过滤器 */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;                        // 设置过滤器 0
  CAN_FilterInitStructure.CAN_FilterMode   = CAN_FilterMode_IdMask;    // 设置为屏蔽位模式
  CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_32bit;    // 设置为 32bit 宽度
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                   // 设置 32 位的 ID 值
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;               // 设置 32 位屏蔽值
  CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; // 将过滤器 0 关联至 FIFO0 邮箱
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               // 激活过滤器 0
  CAN_FilterInit(&CAN_FilterInitStructure);                            // 使用上述设置初始化过滤器

#if CAN_INT_ENABLE
  /* CAN 总线中断接收设置 */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                   // 允许 FIFO0 邮箱产生中断
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // 设置中断主优先级为 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;  // 设置中断次优先级为 0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
  return 0;
}

/** CAN 数据发送函数，参数 msg 是待发送数据(最大 8 字节)，参数 len 是待发送的数据长度(最大值 8)，返回值为 0 表示发送成功，其它值表示失败*/
u8 CAN_Send_Msg(u8 *msg, u8 len)
{
  u8 mbox;
  u16 i = 0;
  CanTxMsg TxMessage;
  TxMessage.StdId = 0x12;           // 设置标准格式下，标识符的值
  TxMessage.ExtId = 0x18E0F7F9;     // 设置扩展格式下，标识符的值

  TxMessage.IDE = CAN_Id_Extended;  // 设置为标准帧格式
  TxMessage.RTR = CAN_RTR_Data;     // 设置为数据帧
  TxMessage.DLC = len;              // 待发送的数据长度

  for (i = 0; i < len; i++) 
  {
    TxMessage.Data[i] = msg[i]; // 将参数 msg 数组内的元素，逐个发送至 CAN 总线控制器
  }
  mbox = CAN_Transmit(CAN1, &TxMessage); // 将 TxMessage 数据写入至 CAN 总线 1，返回值 mbox 表示数据被放入了哪个邮箱
  i = 0;
  /* 等待数据发送至邮箱，成功返回 0，失败返回 1 */
  while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0X4E2))   // 0XFFF  0X4E2=5ms
  
    i++; // 等待发送结束
    if (i >= 0X4E2)
    {
      return 1; // 超出预定时间，数据发送失败，返回 1
    }
    return 0;
}



/** 采用查询模方式接收 CAN 总线数据，参数 buf 是接收数据缓冲区，返回 0 表示当前没有数据接收，其它值则表示接收数据的长度 */
u8 CAN_Receive_Msg(u8 *buf) 
{
  u32 i;
  CanRxMsg RxMessage;

  /* 判断 FIFO0 是否有数据 */
  if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0) 
  {
    return 0;                                 // 未接收到数据返回 0
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); // 读取 FIFO0 数据
    for (i = 0; i < 8; i++) 
    {
      buf[i] = RxMessage.Data[i];
    }
    return RxMessage.DLC; // 返回数据长度
  }
}

/** CAN 中断接收程序，需要将 can.h 的 CAN_INT_ENABLE 置为 1 才会使能中断 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  vu8 CAN_ReceiveBuff[8]; // CAN 数据接收数组
  vu8 i = 0;
  vu8 u8_RxLen = 0;
  /* 清空相关的寄存器 */
  CAN_ReceiveBuff[0] = 0;
  RxMessage.StdId = 0x00;
  RxMessage.ExtId = 0x00;
  RxMessage.IDE   = 0;
  RxMessage.RTR   = 0;
  RxMessage.DLC   = 0;
  RxMessage.FMI   = 0;

  for (i = 0; i < 8; i++) {
    RxMessage.Data[i] = 0x00;
  }

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);    // 读取 FIFO0 邮箱数据
  u8_RxLen = RxMessage.DLC;                    // 获取数据的数量

  /* 判断标识符是否一致 */
  if (RxMessage.StdId == 0x12)
  {
    CAN_ReceiveBuff[0] = RxMessage.DLC;        // 将接收到的数据存放至 CAN 数据接收数组的第 0 位
    for (i = 0; i < u8_RxLen; i++) {
      CAN_ReceiveBuff[i] = RxMessage.Data[i];  // 将 8 位数据存放至 CAN 数据接收数组的其它位
    }
  }
}














