/** Hardware/CAN/can.c */
#include "can.h"
/** CAN ���߳�ʼ������������ 0 ��ʾ�ɹ���������ʾʧ�� */
u8 CAN1_Configuration(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;

#if CAN_INT_ENABLE
  NVIC_InitTypeDef NVIC_InitStructure;
#endif
  /* ����ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ�� GPIOA ����ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  // ʹ�� CAN1 ����ʱ��

  /* ���� GPIO */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       // �����������ģʽ
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         // ��������ģʽ
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ���� CAN ���߹��� */
  CAN_InitStructure.CAN_TTCM = DISABLE;         // �ر�ʱ�䴥��ͨ��ģʽ
  CAN_InitStructure.CAN_ABOM = DISABLE;         // �ر�����Զ����߹���
  CAN_InitStructure.CAN_AWUM = DISABLE;         // �ر�˯��ģʽͨ���������
  CAN_InitStructure.CAN_NART = ENABLE;          // ������ֹ�����Զ�����
  CAN_InitStructure.CAN_RFLM = DISABLE;         // �رձ�������
  CAN_InitStructure.CAN_TXFP = DISABLE;         // ���ȼ��ɱ��ı�ʶ��ȷ��
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN_Mode_Normal ��ͨģʽ��CAN_Mode_LoopBack �ػ�ģʽ

  /* ���� CAN ���߲����� */
  CAN_InitStructure.CAN_SJW = tsjw;
  CAN_InitStructure.CAN_BS1 = tbs1;
  CAN_InitStructure.CAN_BS2 = tbs2;
  CAN_InitStructure.CAN_Prescaler = brp;
  CAN_Init(CAN1, &CAN_InitStructure);                                  // ��ʼ�� CAN1

  /* ���� CAN ���߹����� */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;                        // ���ù����� 0
  CAN_FilterInitStructure.CAN_FilterMode   = CAN_FilterMode_IdMask;    // ����Ϊ����λģʽ
  CAN_FilterInitStructure.CAN_FilterScale  = CAN_FilterScale_32bit;    // ����Ϊ 32bit ���
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;                   // ���� 32 λ�� ID ֵ
  CAN_FilterInitStructure.CAN_FilterIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;               // ���� 32 λ����ֵ
  CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; // �������� 0 ������ FIFO0 ����
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;               // ��������� 0
  CAN_FilterInit(&CAN_FilterInitStructure);                            // ʹ���������ó�ʼ��������

#if CAN_INT_ENABLE
  /* CAN �����жϽ������� */
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);                   // ���� FIFO0 ��������ж�
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  // �����ж������ȼ�Ϊ 1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;  // �����жϴ����ȼ�Ϊ 0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif
  return 0;
}

/** CAN ���ݷ��ͺ��������� msg �Ǵ���������(��� 8 �ֽ�)������ len �Ǵ����͵����ݳ���(���ֵ 8)������ֵΪ 0 ��ʾ���ͳɹ�������ֵ��ʾʧ��*/
u8 CAN_Send_Msg(u8 *msg, u8 len)
{
  u8 mbox;
  u16 i = 0;
  CanTxMsg TxMessage;
  TxMessage.StdId = 0x12;           // ���ñ�׼��ʽ�£���ʶ����ֵ
  TxMessage.ExtId = 0x18E0F7F9;     // ������չ��ʽ�£���ʶ����ֵ

  TxMessage.IDE = CAN_Id_Extended;  // ����Ϊ��׼֡��ʽ
  TxMessage.RTR = CAN_RTR_Data;     // ����Ϊ����֡
  TxMessage.DLC = len;              // �����͵����ݳ���

  for (i = 0; i < len; i++) 
  {
    TxMessage.Data[i] = msg[i]; // ������ msg �����ڵ�Ԫ�أ���������� CAN ���߿�����
  }
  mbox = CAN_Transmit(CAN1, &TxMessage); // �� TxMessage ����д���� CAN ���� 1������ֵ mbox ��ʾ���ݱ��������ĸ�����
  i = 0;
  /* �ȴ����ݷ��������䣬�ɹ����� 0��ʧ�ܷ��� 1 */
  while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0X4E2))   // 0XFFF  0X4E2=5ms
  
    i++; // �ȴ����ͽ���
    if (i >= 0X4E2)
    {
      return 1; // ����Ԥ��ʱ�䣬���ݷ���ʧ�ܣ����� 1
    }
    return 0;
}



/** ���ò�ѯģ��ʽ���� CAN �������ݣ����� buf �ǽ������ݻ����������� 0 ��ʾ��ǰû�����ݽ��գ�����ֵ���ʾ�������ݵĳ��� */
u8 CAN_Receive_Msg(u8 *buf) 
{
  u32 i;
  CanRxMsg RxMessage;

  /* �ж� FIFO0 �Ƿ������� */
  if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0) 
  {
    return 0;                                 // δ���յ����ݷ��� 0
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); // ��ȡ FIFO0 ����
    for (i = 0; i < 8; i++) 
    {
      buf[i] = RxMessage.Data[i];
    }
    return RxMessage.DLC; // �������ݳ���
  }
}

/** CAN �жϽ��ճ�����Ҫ�� can.h �� CAN_INT_ENABLE ��Ϊ 1 �Ż�ʹ���ж� */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;
  vu8 CAN_ReceiveBuff[8]; // CAN ���ݽ�������
  vu8 i = 0;
  vu8 u8_RxLen = 0;
  /* �����صļĴ��� */
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

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);    // ��ȡ FIFO0 ��������
  u8_RxLen = RxMessage.DLC;                    // ��ȡ���ݵ�����

  /* �жϱ�ʶ���Ƿ�һ�� */
  if (RxMessage.StdId == 0x12)
  {
    CAN_ReceiveBuff[0] = RxMessage.DLC;        // �����յ������ݴ���� CAN ���ݽ�������ĵ� 0 λ
    for (i = 0; i < u8_RxLen; i++) {
      CAN_ReceiveBuff[i] = RxMessage.Data[i];  // �� 8 λ���ݴ���� CAN ���ݽ������������λ
    }
  }
}














