/***************************************
 * �ļ���  ��usart1.c
 * ����    ������USART1         
 * ʵ��ƽ̨�� STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  
 
 * ����    ������USART2         
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA2  - USART2(Tx)      |
 *          | PA3  - USART2(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0 
 
 * ����    ������USART3         
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PB10  - USART3(Tx)      |
 *          | PB11  - USART3(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0 
*****************************************/

#include "usart1.h"
#include <stdarg.h>
#include "misc.h"
#include "angle_as5600.h"
#include "gpio.h"

//GPS_Handle_t  GPS_Handle;

Communication_Handle_t  RS485_Handle;   //�ұߵ�485ͨ�Ŵ���
Communication_Handle_t  LS485_Handle;   //��ߵ�485ͨ�Ŵ���
Communication_Handle_t  MS485_Handle;   //�м��485ͨ�Ŵ���

//typedef unsigned  char  u8;
//typedef signed    char  s8;
//typedef unsigned  int   u16;
//typedef signed    int   s16;
const u16 CRC16_Tab[256] =
{               
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

//====================================================================
//��������: u16 CalCRC16(u8* Data, u8 length) 
//�������: *Data��Ҫ����CRC�����ݿ飬length���ݿ�ĳ���
//�������: void
//����ֵ  : �������CRC
//��������: ����һ�����ݿ��16λCRC
//ע������: void
//====================================================================
u16 CalCRC16(u8* Data, u8 length)
{
    u8 i = 0;
    u16 Crc = 0;    
    for(i = 0; i < length; i++)
    {
        Crc = CRC16_Tab[(u8)(Crc >> 8) ^ Data[i]] ^ (Crc << 8);
    }    
    return Crc;
}


void USART1_2_3_Init(u32 bound)
{ 
    USART1_Config(bound);
    USART2_Config(bound);
    USART3_Config(bound);
}



void Send_Mode(void)
{
     USART1_SEND_DATA();
    
    
     USART2_SEND_DATA();
}
void Recive_Mode(void)
{
   USART1_RECIVE_DATA();
   USART2_RECIVE_DATA();
}
   
    
    
void USART1_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
    GPIO_InitStructure.GPIO_Pin = USART1_TX_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);    
  
    GPIO_InitStructure.GPIO_Pin = USART1_RX_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);           //��ʼ��GPIOA
	  
    GPIO_InitStructure.GPIO_Pin = USART1_REDE_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //��ʼ��GPIOB
	 
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���    
    
	/* USART1 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = bound;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
    
    /* USART1 ����ģʽ���� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
    USART_Cmd(USART1, ENABLE);// USART1ʹ��
    
}
//��̬����
u8 Write_RxBuf(Communication_Handle_t* p,u8 ch)
{
    p->RxBuf[p->RxTailPoint] = ch;   //��һ���ֽ�[0]
    p->RxTailPoint++;
    if(p->RxTailPoint >=RS485_RXBUFSIZE)
    {
        p->RxTailPoint = 0;
    }
    // overflow
    if(++p->RxCount >= RS485_RXBUFSIZE)
    {
        p->RxCount=0;
        
        return 1;
    }
    return 0;
}



void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
	/* ʹ�� USART2 ʱ��8��*/  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_AFIO, ENABLE);
	/* USART2 ʹ��IO�˿����� */    
    GPIO_InitStructure.GPIO_Pin = USART2_TX_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;         //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);    
  
    GPIO_InitStructure.GPIO_Pin = USART2_RX_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);            //��ʼ��GPIOA
    
    GPIO_InitStructure.GPIO_Pin = USART2_REDE_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //�������
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);            //��ʼ��GPIOA
	    
    NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
    NVIC_Init(&NVIC_InitStructure);                        //û�г�ʼ��������
	/* USART2 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = bound;                //9600  19200   38400    57600 115200��;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	   //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;       //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��

	USART_Init(USART2, &USART_InitStructure);                  //��ʼ��USART2
    /* USART2 ����ģʽ���� */
    USART_ITConfig(USART2,  USART_IT_RXNE, ENABLE);	
    USART_Cmd(USART2, ENABLE);// USART2ʹ��


}

void USART3_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    
	/* ʹ�� USART3 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 ʹ��IO�˿����� */    
    GPIO_InitStructure.GPIO_Pin = USART3_TX_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART3_GPIO, &GPIO_InitStructure);    
  
    GPIO_InitStructure.GPIO_Pin = USART3_RX_Pin;          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
    GPIO_Init(USART3_GPIO, &GPIO_InitStructure);            //��ʼ��GPIOA
    
    GPIO_InitStructure.GPIO_Pin = USART3_REDE_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    //�������
    GPIO_Init(USART3_GPIO, &GPIO_InitStructure);            //��ʼ��GPIOA
	//Usart3 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���    
    
	/* USART3 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = bound;                 //9600  19200   38400    57600 115200��;	//���������ã�115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	    //ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;        //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; // usart3ֻ�Ǹ����ͣ���������ս����뷢�Ͷ�ʹ��
	USART_Init(USART3, &USART_InitStructure);                     //��ʼ��USART2
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);                       // USART1ʹ��
    
}


 

/////*����һ���ֽ�����*/
void UART1_SendByte(uint8_t SendData)
{	   
        USART_SendData(USART1,SendData);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	    
}  

void UART2_SendByte(uint8_t SendData)
{	   
        USART_SendData(USART2,SendData);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	    
}  

 
void UART3_SendByte(uint8_t SendData)
{	   
        USART_SendData(USART3,SendData);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	    
}  

/*���������ֽ�����16λ*/
void UART1_SendHalfWord(uint16_t data)
{
    uint8_t temp_h ,temp_l;
    temp_h = (data&0xff00)>>8;
    temp_l = data&0xff;
    UART1_SendByte(temp_h);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    
    UART1_SendByte(temp_l);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
void UART2_SendHalfWord(uint16_t data)
{
    uint8_t temp_h ,temp_l;
    temp_h = (data&0xff00)>>8;
    temp_l = data&0xff;
    UART2_SendByte(temp_h);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    
    UART2_SendByte(temp_l);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void UART3_SendHalfWord(uint16_t data)
{
    uint8_t temp_h ,temp_l;
    temp_h = (data&0xff00)>>8;
    temp_l = data&0xff;
    UART3_SendByte(temp_h);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    
    UART3_SendByte(temp_l);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/*����8λ��������*/
//��������
void UART1_SendArray(uint8_t *array ,uint8_t num)
{
    uint8_t i;
    for(i=0;i<num;i++)
    {
      UART1_SendByte(array[i]);
    }
    
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

}
void UART2_SendArray(uint8_t *array ,uint8_t num)
{
    uint8_t i;
    for(i=0;i<num;i++)
    {  
      UART2_SendByte(array[i]); 
    }
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
void UART3_SendArray(uint8_t *array ,uint8_t num)
{
    uint8_t i;
    
    for(i=0;i<num;i++)
    {
      UART3_SendByte(array[i]);
    
    }
    
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

/*�����ַ���*/

void UART1_SendStr(uint8_t *str )
{
  uint8_t i=0;
   do      
  {    
      UART1_SendByte(*(str+i)); 
      i++;
  }while(*(str+i)!='\0');

  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);


}
void UART2_SendStr(uint8_t *str )
{
    
    uint8_t i=0;
    
    do      
   {    
      UART2_SendByte(*(str+i)); 
      i++;
    }while(*(str+i)!='\0');

  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}
//-------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------------------------------//
/*����һ���ֽ�����*/
unsigned char UART1GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
        {  
					return 0;//û���յ����� 
	    }
        *GetData = USART_ReceiveData(USART1); 
        return 1;//�յ�����
}

unsigned char UART2GetByte(unsigned char* GetData)
{   	   
        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
        {  
					return 0;//û���յ����� 
		    }
        *GetData = USART_ReceiveData(USART2); 
          return 1;//�յ�����
}


//�����͵�����ѹ��Buf��    �˴�u8Ϊ
u8 Put_TxBuf(Communication_Handle_t* p,u8 ch,u8 num)
{
//    if (p->TxCount >= RS485_RXBUFSIZE) return 0;
    p->TxBuf[p->TxTailPoint] = ch; 
    p->TxTailPoint++;
    if (p->TxTailPoint >= num)
    {
        p->TxTailPoint = 0;
        p->TxCount=1;
    }
    return 1;
}
//��buf�ж�ȡ���ݷ��ʹ������ݼĴ���
u8 Read_TxBuf (Communication_Handle_t* p)
{
    u8 ch;
    ch = p->TxBuf[p->TxHeadPoint];
    p->TxHeadPoint++;
    if (p->TxHeadPoint >= RS485_RXBUFSIZE)
    {
        p->TxHeadPoint = 0;
        p->TxCount=0;
    }
    return ch;
}
//ʵ�ʵķ����ұߵ�buf�е�����
void Tx_USART1_Data(void)
{

    if ((USART1->SR & 0x0080) != (u16)RESET)
    {
        if (LS485_Handle.TxCount)
        {
           USART1->DR = Read_TxBuf(&LS485_Handle);
        }
    }
    if (LS485_Handle.TxCount == 0)
    {
        if(USART1->SR & 0x40)
        {
//            USART1_RECIVE_DATA();     //��������0��Ϊ����ģʽ
          USART1_SEND_DATA();  
            
        }
    }
}
//ʵ�ʵķ�����ߵ�buf�е�����
void Tx_USART2_Data(void)
{

    if ((USART2->SR & 0x0080) != (u16)RESET)
    {
        if (RS485_Handle.TxCount)
        {
           USART2->DR = Read_TxBuf(&RS485_Handle);
        }
    }
    if (RS485_Handle.TxCount == 0)
    {
        if(USART2->SR & 0x40)
        {
//            USART2_RECIVE_DATA();     //��������0��Ϊ����ģʽ
              USART2_SEND_DATA();
        }
    }
}

void Tx_USART3_Data(void)
{

    if ((USART3->SR & 0x0080) != (u16)RESET)
    {
        if (MS485_Handle.TxCount)
        {
           USART3->DR = Read_TxBuf(&MS485_Handle);
        }
    }
    if (MS485_Handle.TxCount == 0)
    {
        if(USART3->SR & 0x40)
        {
//            USART2_RECIVE_DATA();     //��������0��Ϊ����ģʽ
              USART3_SEND_DATA();
            
        }
    }
}



void SendRL_Handle(void)
{
//    Tx_USART1_Data();
//    Tx_USART2_Data();  //���ʹ���2������    
}
//���Y��ص�����
//����ͷ--��ַ��--������--����--����--У���--����1--����2
void YD_BatteryData_Filling(u8 cmdbyte)
{   
    u8 ipos=0;
    u8 cmdtxCS=0;
    u8 cmdTxBuf[11];
    u8 temp;
        
    cmdTxBuf[ipos++] = BATTERY_HEADER;            //����ͷ
    cmdTxBuf[ipos++] = BATTERY_ADDR;              //��ַ��
    switch(cmdbyte)    //������
    {
       case BATTERY_VOTAGE:  
          cmdTxBuf[ipos++] = BATTERY_VOTAGE;
          cmdTxBuf[ipos++] = DATA_LENG_1;        
          cmdTxBuf[ipos++] = 0x0b;
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_VOTAGE+DATA_LENG_1+0x0b)&0x00ff));
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_VOTAGE+DATA_LENG_1+0x0b)&0xff00)>>8);
        break;
       case BATTERY_SOC:  
          cmdTxBuf[ipos++] = BATTERY_SOC;
          cmdTxBuf[ipos++] = DATA_LENG_1;  
          cmdTxBuf[ipos++] = 0x0b;
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_SOC+DATA_LENG_1+0x0b)&0x00ff));
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_SOC+DATA_LENG_1+0x0b)&0xff00)>>8);
        break;       
      case BATTERY_EE:  
          cmdTxBuf[ipos++] = BATTERY_EE;
          cmdTxBuf[ipos++] = DATA_LENG_1;  
          cmdTxBuf[ipos++] = 0x0b;
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_EE+DATA_LENG_1+0x0b)&0x00ff));
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_EE+DATA_LENG_1+0x0b)&0xff00)>>8);
        break; 
       case BATTERY_OPEN:  
          cmdTxBuf[ipos++] = BATTERY_OPEN;
          cmdTxBuf[ipos++] = DATA_LENG_3;  
          cmdTxBuf[ipos++] = 0xF1;
          cmdTxBuf[ipos++] = 0x81;
          cmdTxBuf[ipos++] = 0x09;
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_OPEN+DATA_LENG_3+0xF1+0x81+0x09)&0x00ff));
          cmdTxBuf[ipos++] = (((BATTERY_ADDR+BATTERY_OPEN+DATA_LENG_3+0xF1+0x81+0x09)&0xff00)>>8);
        break;   

      default:
        break;  
    }
    
    
    cmdTxBuf[ipos++] = BATTERY_END1;               
    cmdTxBuf[ipos++] = BATTERY_END2; 
    
     for(cmdtxCS = 0; cmdtxCS < ipos; cmdtxCS++)
    {  
        temp=cmdTxBuf[cmdtxCS];
        switch(cmdbyte)
        {
           case BATTERY_VOTAGE:  
           
             Put_TxBuf(&RS485_Handle,temp,9);
             Put_TxBuf(&LS485_Handle,temp,9);  
           break;
           
           case BATTERY_SOC:
             Put_TxBuf(&RS485_Handle,temp,9);
             Put_TxBuf(&LS485_Handle,temp,9);  
           break; 
           case BATTERY_EE:
             Put_TxBuf(&RS485_Handle,temp,9);
             Put_TxBuf(&LS485_Handle,temp,9);  
           break; 
           
            case BATTERY_OPEN:
             Put_TxBuf(&RS485_Handle,temp,11);
             Put_TxBuf(&LS485_Handle,temp,11);  
           break;  
           default:
           break;
        }
    }
}




//Nϵ�е��   ����ص�����
//--�Ի���ַ��--������--������--���ݳ���--CRCУ����--
void N_BatteryData_Filling(u8 cmdbyte)
{   
    u8 ipos=0;
    u8 cmdtxCS=0;

    u8 cmdTxBuf[10];
    u8 temp;
        
    cmdTxBuf[ipos++] = N_BATTERY_ADDR;     //�Ի���ַ��
    cmdTxBuf[ipos++] = N_FUNCYION;         //������
    switch(cmdbyte)                       //������
    {
    
       case N_COMMAND:  
          cmdTxBuf[ipos++] = N_COMMAND;
          cmdTxBuf[ipos++] = N_LENGTH; 
        break; 
      default:
        break;  
    }
    //CRCУ����
     cmdTxBuf[ipos++] = (CalCRC16(cmdTxBuf,4)&0xff00)>>8;
     cmdTxBuf[ipos++] = (CalCRC16(cmdTxBuf,4)&0x00ff);
     for(cmdtxCS = 0; cmdtxCS < ipos; cmdtxCS++)
    {  
        temp=cmdTxBuf[cmdtxCS];
        Put_TxBuf(&RS485_Handle,temp,6);
        Put_TxBuf(&LS485_Handle,temp,6); 
    }
}
//�������������������ѹ�뷢��Buf��  ��������ת�Ӱ��LCD
void SendData_Filling(u8 cmdbyte,u8 length,u8 address)
{
    u8 ipos=0;
    u8 cmdtxCS=0;
    u8 icnt=0;
    u8 cmdTxBuf[13];
    u8 temp;
    
    cmdTxBuf[ipos++] = HEADER_CMD;           //0   0x28
    cmdTxBuf[ipos++] = address;              //1   
    cmdTxBuf[ipos++] = length;               //2    
    switch(cmdbyte)
    {
        case R_POWER:  
          cmdTxBuf[ipos++] = THROTTLE;
          cmdTxBuf[ipos++] = THROTTLE_Handle.R_Direction;            //��8λ
          cmdTxBuf[ipos++] = THROTTLE_Handle.R_PercentTemp;          //��8λ
        break;
        case L_POWER: 
          cmdTxBuf[ipos++] = THROTTLE;
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_Direction;            //��8λ
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_PercentTemp;          //��8λ
        break;  
        case Only_R_POWER:  
          cmdTxBuf[ipos++] = THROTTLE;
          cmdTxBuf[ipos++] = THROTTLE_Handle.R_Direction;            //��8λ
          cmdTxBuf[ipos++] = THROTTLE_Handle.R_PercentTemp;          //��8λ
        break;
        case Only_L_POWER: 
          cmdTxBuf[ipos++] = THROTTLE;
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_Direction;            //��8λ
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_PercentTemp;          //��8λ
        break;  
        case MOTO_INFMATION: 
          cmdTxBuf[ipos++] = THROTTLE;
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_Direction;            //��8λ
          cmdTxBuf[ipos++] = THROTTLE_Handle.L_PercentTemp;          //��8λ
        break;  
        case DRIVER_VERSION:
          cmdTxBuf[ipos++] = GET_DRIVER_INF;     //03
        break;
        case ADAPTER_VERSION:
          cmdTxBuf[ipos++] = GET_ADAPTER_INF;    //38
        break;
        case ADAPTER_GPS:   
          cmdTxBuf[ipos++] = GET_GPS_DATA;      
        break;
        
        case LCD_MOTOR_MSG:    //������һ�ŵ����Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
          cmdTxBuf[ipos++] =  LCD_MOTOR_MSG;                         
          cmdTxBuf[ipos++] =  MOTOR_Handle1.Motor_Status;                                        
          cmdTxBuf[ipos++] =  (MOTOR_Handle1.power)>>8; 
          cmdTxBuf[ipos++] =  (MOTOR_Handle1.power); 
          cmdTxBuf[ipos++] =  (MOTOR_Handle1.rpm)>>8; 
          cmdTxBuf[ipos++] =  (MOTOR_Handle1.rpm); 
          cmdTxBuf[ipos++] =  MOTOR_Handle1.tMot.temp_state;   
                                                                    
        break;
        case LCD_MOTOR_MSG1:  //�ҵ����2�ŵ����Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��  
          cmdTxBuf[ipos++] =  LCD_MOTOR_MSG1;     
          cmdTxBuf[ipos++] =  MOTOR_Handle.Motor_Status;           
          cmdTxBuf[ipos++] =  (MOTOR_Handle.power)>>8; 
          cmdTxBuf[ipos++] =  (MOTOR_Handle.power); 
          cmdTxBuf[ipos++] =  (MOTOR_Handle.rpm)>>8; 
          cmdTxBuf[ipos++] =  (MOTOR_Handle.rpm); 
          cmdTxBuf[ipos++] =  MOTOR_Handle.tMot.temp_state;        
        break;
        case LCD_ADAPTER_MSG:            
          cmdTxBuf[ipos++] = LCD_ADAPTER_MSG;      //0x62
          cmdTxBuf[ipos++] =  GPS_Handle.speed;
          cmdTxBuf[ipos++] = (GPS_Handle.traveled_time&0xff00)>>8;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
          cmdTxBuf[ipos++] =  GPS_Handle.traveled_time&0x00ff; 
          cmdTxBuf[ipos++] =  (GPS_Handle.traveled_distance&0xff00)>>8; 
          cmdTxBuf[ipos++] =  GPS_Handle.traveled_distance&0x00ff; 
          cmdTxBuf[ipos++] =  0;                                                          //GPS��Ϣ��׼��������̺�����ʱ�䡢����--------������ʾ����GPS����-----------------------------

        break;
        case LCD_ADAPTER_MSG1:            
          cmdTxBuf[ipos++] = LCD_ADAPTER_MSG1;      //0x62
          cmdTxBuf[ipos++] =  GPS_Handle1.speed;
          cmdTxBuf[ipos++] = (GPS_Handle1.traveled_time&0xff00)>>8;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
          cmdTxBuf[ipos++] =  GPS_Handle1.traveled_time&0x00ff; 
          cmdTxBuf[ipos++] =  (GPS_Handle1.traveled_distance&0xff00)>>8; 
          cmdTxBuf[ipos++] =  GPS_Handle1.traveled_distance&0x00ff; 
          cmdTxBuf[ipos++] =  0;                                                          //GPS��Ϣ��׼��������̺�����ʱ�䡢����--------������ʾ����GPS����-----------------------------

        break;
        case LCD_BATTERY_MSG:  
           cmdTxBuf[ipos++] = LCD_BATTERY_MSG;     //0x63    1
           if(BATTERY_Handle.State1==Battery_State_YD)
           {   
             cmdTxBuf[ipos++] =  BATTERY_Handle.State1;                
             cmdTxBuf[ipos++] =  (BATTERY_Handle.Y_Battery_Voltage1&0xff00)>>8;
             cmdTxBuf[ipos++] =  BATTERY_Handle.Y_Battery_Voltage1&0x00ff;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  (BATTERY_Handle.Y_Battery_Soc1&0xff00)>>8; 
             cmdTxBuf[ipos++] =  BATTERY_Handle.Y_Battery_Soc1&0x00ff; 
             cmdTxBuf[ipos++] =  0; 
              
           }
          else if(BATTERY_Handle.State1==Battery_State_N)
          {
             cmdTxBuf[ipos++] =  BATTERY_Handle.State1; 
             cmdTxBuf[ipos++] =  (BATTERY_Handle.N_Battery_Voltage1&0xff00)>>8;
             cmdTxBuf[ipos++] =  BATTERY_Handle.N_Battery_Voltage1&0x00ff;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  (BATTERY_Handle.N_Battery_Soc1&0xff00)>>8; 
             cmdTxBuf[ipos++] =  BATTERY_Handle.N_Battery_Soc1&0x00ff; 
             cmdTxBuf[ipos++] =  0;  
          }
         else if(BATTERY_Handle.State1==Battery_State_NO_Bat)
         {
             cmdTxBuf[ipos++] =  BATTERY_Handle.State1;
             cmdTxBuf[ipos++] =  0;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0;   
         }
         else 
         {
             cmdTxBuf[ipos++] =  0;
             cmdTxBuf[ipos++] =  0;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0;   
         }    
        break;
         
        case LCD_BATTERY_MSG1:  
           cmdTxBuf[ipos++] = LCD_BATTERY_MSG1;          //0x63
           if(BATTERY_Handle.State2==Battery_State_YD)
           {               
             cmdTxBuf[ipos++] =  BATTERY_Handle.State2;  
             cmdTxBuf[ipos++] =  (BATTERY_Handle.Y_Battery_Voltage2&0xff00)>>8;
             cmdTxBuf[ipos++] =  BATTERY_Handle.Y_Battery_Voltage2&0x00ff;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  (BATTERY_Handle.Y_Battery_Soc2&0xff00)>>8; 
             cmdTxBuf[ipos++] =  BATTERY_Handle.Y_Battery_Soc2&0x00ff; 
             cmdTxBuf[ipos++] =  0; 
              
           }
           else if(BATTERY_Handle.State2==Battery_State_N)
           {
             cmdTxBuf[ipos++] =  BATTERY_Handle.State2; 
             cmdTxBuf[ipos++] =  (BATTERY_Handle.N_Battery_Voltage2&0xff00)>>8;
             cmdTxBuf[ipos++] =  BATTERY_Handle.N_Battery_Voltage2&0x00ff;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  (BATTERY_Handle.N_Battery_Soc2&0xff00)>>8; 
             cmdTxBuf[ipos++] =  BATTERY_Handle.N_Battery_Soc2&0x00ff; 
             cmdTxBuf[ipos++] =  0; 
             
           }
           else if(BATTERY_Handle.State2==Battery_State_NO_Bat)
           {
             cmdTxBuf[ipos++] =  BATTERY_Handle.State2;
             cmdTxBuf[ipos++] =  0;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0;   
           }
           else 
           {
             cmdTxBuf[ipos++] =  0;
             cmdTxBuf[ipos++] =  0;                   //Ԥ��Ϊ����¶ȣ�����Ĺ��ʣ����ת��
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0; 
             cmdTxBuf[ipos++] =  0;   
           }    
          break;
         
        case LCD_THROTTLE_MSG:      //0x64
          cmdTxBuf[ipos++] =  LCD_THROTTLE_MSG;
          cmdTxBuf[ipos++] =  THROTTLE_Handle.Saft_Key|(THROTTLE_Handle.L_SetZero_Flag<<1)|(THROTTLE_Handle.R_SetZero_Flag<<2);        //��ȫ����
          cmdTxBuf[ipos++] =  Open_Close_Switch_Handle.Open_Flag;  //               
          cmdTxBuf[ipos++] =  KEY_Handle.Open_Close_Key.Action; 
          cmdTxBuf[ipos++] =  GPS_Handle.num_sv;               //��һ��GPS������Ŀ    
          cmdTxBuf[ipos++] =  GPS_Handle1.num_sv;              //�ڶ���GPS��������Ŀ  
          cmdTxBuf[ipos++] =  0;                                                                       //������λ��Ϣ�Ͱ�ȫ������Ϣ-------������ʾ���Ĳ��ݸ�����-----------------------

        break;

        default:
        break;
    }
		
    for(icnt=2,cmdtxCS=0;icnt<ipos;icnt++)
    {
        cmdtxCS^=cmdTxBuf[icnt];
    }
		
    cmdTxBuf[ipos++] = cmdtxCS&0xff;
    cmdTxBuf[ipos++] = END;     //0x29
    for(cmdtxCS = 0; cmdtxCS < ipos; cmdtxCS++)
    {  
        temp=cmdTxBuf[cmdtxCS];
        switch(cmdbyte)
        {
           case R_POWER:  
           
             Put_TxBuf(&RS485_Handle,temp,DATA_LENG_3+5);
           
           break;
           
           case L_POWER:
           
             Put_TxBuf(&LS485_Handle,temp,DATA_LENG_3+5);
           
           break;    
           case Only_R_POWER:
  
             Put_TxBuf(&RS485_Handle,temp,DATA_LENG_3+5);
             Put_TxBuf(&LS485_Handle,temp,DATA_LENG_3+5);
          
           break;
           
           case Only_L_POWER:  
            
             Put_TxBuf(&RS485_Handle,temp,DATA_LENG_3+5);
             Put_TxBuf(&LS485_Handle,temp,DATA_LENG_3+5);
           break;  
           
           case MOTO_INFMATION: 
               Put_TxBuf(&MS485_Handle,temp,DATA_LENG_3+5);
           break;  
           case DRIVER_VERSION:
               Put_TxBuf(&RS485_Handle,temp,DATA_LENG_1+5);
               Put_TxBuf(&LS485_Handle,temp,DATA_LENG_1+5);
           
           break;
           case ADAPTER_VERSION:
               Put_TxBuf(&RS485_Handle,temp,DATA_LENG_1+5);
               Put_TxBuf(&LS485_Handle,temp,DATA_LENG_1+5);
           break;
           case ADAPTER_GPS:   
               Put_TxBuf(&RS485_Handle,temp,DATA_LENG_1+5);
               Put_TxBuf(&LS485_Handle,temp,DATA_LENG_1+5);
           break;

           case LCD_MOTOR_MSG:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                        //-------------------------------------
            break;
           case LCD_ADAPTER_MSG:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                         //-----------------------------------
            break;
           case LCD_BATTERY_MSG:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                         //-------------------------------------
           break;
           case LCD_MOTOR_MSG1:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                        //-------------------------------------
            break;
           case LCD_ADAPTER_MSG1:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                         //-----------------------------------
            break;
           case LCD_BATTERY_MSG1:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                         //-------------------------------------
           break;
           case LCD_THROTTLE_MSG:   
             Put_TxBuf(&MS485_Handle,temp,12);
                                                         //-------------------------------------
           break;
           
           default:
           break;
        }
    }
}

//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//

//��ȡ���յ��ֽ�Buf  8λ�ֽڵ���
u8 Read_RxBuf(Communication_Handle_t* p,u8* ch,u8 num)
{
    if (p->RxCount == 0)
    return 0;
    
    *ch = p->RxBuf[p->RxHeadPoint];
    p->RxHeadPoint++;
    
    
    if (p->RxHeadPoint >= num)
        
    
        p->RxHeadPoint = 0;
    
    
    __disable_irq();//__set_PRIMASK(1);
    p->RxCount--;
    __enable_irq();//__set_PRIMASK(0);
    return 1;
}


////�°汾Э��Ľ���485��������    ת�Ӱ� �� ����������  LCD���ݵ�У��
//���յ�����У��
static u8 Receive_Check(Communication_Handle_t* p)
{
    u8 ch,i=0;
    
    for(i=0;i<p->RxTailPoint;i++)
    {
        if(Read_RxBuf(p,&ch,p->RxTailPoint))    
        {
            switch(p->cmdStep)  
            {
                case 0://����ͷ          
                if(ch == HEADER_CMD)        
                {  
                    p->cmdStep++;
                }
                else
                { 
             
                    return 0;
           
                }
                p->cmdPtr = 0;
                break;
        
                case 1://��ַ��
                if((ch == DRIVE_ADDRESS)||(ch == ADAPTER_ADDRESS)||(ch == LCD_ADDRESS))
                {
                 p->cmdStep++;
                }
//                else
//                {
//                 p->cmdStep=0;
//                 p->cmdPtr=0;
//                }
                break;
       
                case 2:  //����
             
                  p->ucCmdData[p->cmdPtr++]=ch;   //0
                  p->cmdStep++;
                  p->ucCheckSum=ch;
                break;     

                case 3:  //������
                p->ucCmdData[p->cmdPtr++]=ch;     //1 2 3 4 5
                p->ucCheckSum^=ch;
                if(p->cmdPtr>=(p->ucCmdData[0]+1))
                {
                    p->cmdStep++;
                }
                break;
           
                case 4:  //У����

                if((p->ucCheckSum&0xff)==ch)
                { 
                    p->cmdStep++;
                }
                else
                {
                  p->cmdStep=0;
                  p->cmdPtr=0;
                  return 0;
                }
                break;
                       
                case 5:  //β��    

                if(END==ch)   
                {    
                 p->cmdStep=0;
                 p->cmdPtr=0;                
                  return p->ucCmdData[1];   //У��ͨ��������ͷ�룬�������룻
                }
                break;
                default:
                break;
        }
      }

    }
    return 0;
}
//����У��YD�����Ϣ
//����У��
static u8 Receive_YD_Battery_Check(Communication_Handle_t* p)
{
    u8 ch,i=0;
    for(i=0;i<p->RxTailPoint;i++)
    {
        
       if(Read_RxBuf(p,&ch,p->RxTailPoint))
      {
        switch(p->cmdStep)
        {
            case 0:   //��ʼ�ֽ�
            if (ch == BATTERY_HEADER)
            {
                p->cmdStep++;
                p->ucCheckSum=0;
            }
            else
            {           
                p->cmdStep=0;
                return 0;
            }
            p->cmdPtr = 0;
            break;
        
            case 1:   //��ַ��
            if(ch==BATTERY_ADDR)
            {
                p->cmdStep++;
                p->ucCheckSum+=ch;
            }
            else
            {
                p->cmdStep=0;
                p->cmdPtr=0;
                p->ucCheckSum=0;
            }
            break;
       
            case 2:  //������
            if(ch==BATTERY_VOTAGE||ch==BATTERY_SOC||ch==BATTERY_OPEN||ch==BATTERY_EE)
            {
              p->ucCmdData[p->cmdPtr++]=ch;   //0
              p->cmdStep++;
              p->ucCheckSum+=ch;
            }
            else
            {
              p->cmdStep=0;
              p->cmdPtr=0;
              p->ucCheckSum=0;
            }
            break;     
                
            case 3:  //����
            p->ucCmdData[p->cmdPtr++]=ch;     //������2���ȣ�10
            p->ucCheckSum+=ch;
            
            if(p->cmdPtr>(p->ucCmdData[1]+1))
            {
                p->cmdStep++;
            }
            break;
       
            case 4:  //С��У����

            if((p->ucCheckSum&0x00ff)==ch)
            { 
                p->cmdStep++;
            }
            else
            {
               p->cmdStep=0;
               p->cmdPtr=0;
               return 0;
            }
            break;
            case 5:  //���У����

            if(((p->ucCheckSum&0xff00)>>8)==ch)
            { 
                p->cmdStep++;
            }
            else
            {
               p->cmdStep=0;
               p->cmdPtr=0;
              return 0;
            }
            break;      
            case 6:  //β��    
        
            if(BATTERY_END1==ch)   
            {     
               p->cmdStep++;
            }
            break;
                    
            case 7:  //β��    
           
            if(BATTERY_END2==ch)   
            {     
               p->cmdStep=0;
               p->cmdPtr=0;
//              return p->ucCmdData[0];   //У��ͨ��������ͷ�룬�������룻
               return 1;     
            }
            break;
            default:
            break;
        }
      } 
    }
              
    
    return 0;
}
//����У��Nϵ�е����Ϣ
//����У��
static u8 Receive_N_Battery_Check(Communication_Handle_t* p)
{

    u8 ch,i=0;
    for(i=0;i<p->RxTailPoint;i++)
    {
        if(Read_RxBuf(p,&ch,p->RxTailPoint))
        {
        switch(p->cmdStep)
        {
            case 0://��վ��ַ
            if (ch == N_BATTERY_ADDR)
            {
                p->cmdPtr = 0;
                p->cmdStep++;
                p->ucCmdData[p->cmdPtr++]=ch;     //0
            }
            else
            {
                return 0;
            }
            break;
        
            case 1://������
            if(ch==N_FUNCYION)
            {
                p->cmdStep++;
                p->ucCmdData[p->cmdPtr++]=ch;    //1=N_FUNCYION;              
            }
            else
            {
                p->cmdStep=0;
                p->cmdPtr=0;
            }

            break;
       
            case 2:  //������
             if(ch==N_COMMAND)
             {
                p->cmdStep++;
                p->ucCmdData[p->cmdPtr++]=ch;      //2  N_COMMAND;
             }
            break;                 
            case 3:  //���ݳ���
             if(ch==N_LENGTH)
             {
                p->cmdStep++;
                p->ucCmdData[p->cmdPtr++]=ch;     //3
             }

            break;
       
            case 4:  //���ص�����
              p->ucCmdData[p->cmdPtr++]=ch;     //4-------> 5-----11
              if(p->cmdPtr>=(N_LENGTH+4))
              {
                  p->cmdStep++;          
              }
            break;
                   
            case 5:  //β��         
             //CRCУ����
            if(((CalCRC16(p->ucCmdData,p->cmdPtr)&0xff00)>>8)==ch)            
            { 
                p->cmdStep++;
//              p->cmdStep=0;
//              p->cmdPtr=0;
//              return p->ucCmdData[1];   //У��ͨ��������ͷ�룬�������룻
             }
             else
             {
                return 0;
             }
             break;
                   
            case 6:  //β��  
             //CRCУ����
              if(((CalCRC16(p->ucCmdData,p->cmdPtr)&0x00ff))==ch)            
             { 

              p->cmdStep=0;
              p->cmdPtr=0;
              return p->ucCmdData[2];   //У��ͨ��������ͷ�룬�������룻
              }
           
            break;
            default:
            break;
        }
        }
   }
    return 0;

}
//�Ի������Ҵ�������ݺ���  �����÷���ģʽ
unsigned char Receive_R_Handle(void)
{
    unsigned char cmdbyte;
    if(Receive_Check(&RS485_Handle))
    {
        cmdbyte =  RS485_Handle.ucCmdData[1];
       //ͨ�ż���
        switch(cmdbyte)
        {
            //��������״̬
           case MOTOR_STATE_1:
               if((RS485_Handle.ucCmdData[2]&0x01)==1)
               {
                 MOTOR_Handle.motor_status=motor_stall;
               }
               else if((RS485_Handle.ucCmdData[2]&0x02)==2)
               {
                 MOTOR_Handle.motor_status=motor_over_temp;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x04)==4)
               {
                 MOTOR_Handle.tMot.temp_state=temp_over;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x08)==8)
               {
                 MOTOR_Handle.motor_status=motor_over_cur;
               
               }      
               else if((RS485_Handle.ucCmdData[2]&0x10)==16)
               {
                 MOTOR_Handle.motor_status=motor_comm_fail;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x20)==32)
               {
                 MOTOR_Handle.motor_status=motor_under_vol;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x40)==64)
               {
                 MOTOR_Handle.motor_status=motor_over_vol;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x80)==128)
               {
                 MOTOR_Handle.tMot.temp_state=temp_damage;
               
               }
               else if((RS485_Handle.ucCmdData[2]&0x01)==1)
               {    
                 MOTOR_Handle.motor_status=motor_temp_warn;
               }
               else
               {
                 MOTOR_Handle.motor_status=motor_nomal;
                 MOTOR_Handle.tMot.temp_state=temp_nomal;
               }

               MOTOR_Handle.Motor_Status=RS485_Handle.ucCmdData[2];
             break;
               
               
           case MOTOR_POWER:
               
                MOTOR_Handle.power=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
           
             break;
           case DC_VOTAGE:

                MOTOR_Handle.dc_votage=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break;
           case MOTOR_SPEED:

                MOTOR_Handle.rpm=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break;  
           case MOTOR_CURRENT:
                MOTOR_Handle.motor_current=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break;
           case MOS_MOTOR_TEMP:
                MOTOR_Handle.motor_temp=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
                MOTOR_Handle.mos_temp=(RS485_Handle.ucCmdData[4]<<8)+RS485_Handle.ucCmdData[5];
           
             break;
           case DC_CURRENT:
                MOTOR_Handle.motor_dc_current=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break; 

           case RESET_CMD:
               
             break; 
           case DRIVER_SOFTHARD_VERSION:
               MOTOR_Handle.product_type=RS485_Handle.ucCmdData[2];
               MOTOR_Handle.soft_version=RS485_Handle.ucCmdData[3];
               MOTOR_Handle.sotf_year_month=(RS485_Handle.ucCmdData[4]<<8)+RS485_Handle.ucCmdData[5];
               MOTOR_Handle.hard_version=RS485_Handle.ucCmdData[6];
               MOTOR_Handle.hard_year_month=(RS485_Handle.ucCmdData[7]<<8)+RS485_Handle.ucCmdData[8];
               MOTOR_Handle.Get_Driver_Flag=1;
             break; 
           //ת�Ӱ�
          case GPS_SPEED:
               GPS_Handle.speed =RS485_Handle.ucCmdData[2];
             break; 
          case GPS_NUMBEL:
               GPS_Handle.num_sv=RS485_Handle.ucCmdData[2];
          
               
          
             break;     
          case GPS_LONGITUDE:
               GPS_Handle.longtitude_H=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
               GPS_Handle.longtitude_L=(RS485_Handle.ucCmdData[4]<<8)+RS485_Handle.ucCmdData[5];
             break;       
          case GPS_LATITUDE:
               GPS_Handle.latitude_H=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
               GPS_Handle.latitude_L=(RS485_Handle.ucCmdData[4]<<8)+RS485_Handle.ucCmdData[5];
             break;    
          case GPS_DIR_ANGLE:
               GPS_Handle.driction_angle=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];

             break;   
          case GPS_RUNED_TIME:
               GPS_Handle.traveled_time=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break;  
          case GPS_RUNED_DISTANCE:
               GPS_Handle.traveled_distance=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break;  
         case GPS_RUNNING_TIME:
               GPS_Handle.remain_time=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break; 
         case GPS_RUNNING_DISTANCE:
               
              GPS_Handle.remain_distance=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
             break; 
         case ADAPTER_VERSION_INF:
              GPS_Handle.adapter_soft_version=RS485_Handle.ucCmdData[2];
              GPS_Handle.soft_year_month=(RS485_Handle.ucCmdData[3]<<8)+RS485_Handle.ucCmdData[4];
              GPS_Handle.adapter_hard_version=RS485_Handle.ucCmdData[5];
              GPS_Handle.hard_year_month=(RS485_Handle.ucCmdData[6]<<8)+RS485_Handle.ucCmdData[7];         
              MOTOR_Handle.Get_Adapter_Flag=1;
         
             break; 
         case GPS_TIME:
              GPS_Handle.TimeH=(RS485_Handle.ucCmdData[2]<<8)+RS485_Handle.ucCmdData[3];
              GPS_Handle.TimeL=(RS485_Handle.ucCmdData[3]<<8)+RS485_Handle.ucCmdData[4];
              GPS_Handle.Time=(GPS_Handle.TimeH<<8)+GPS_Handle.TimeL;
             break; 
         
           default:
             break;
        }
//        USART2_SEND_DATA();    //�������������øߣ�Ϊ����ģʽ
    }
    return 0;
}
unsigned char Receive_YD_Handle1(Communication_Handle_t* p)
{
    unsigned char cmdbyte;
    if(Receive_YD_Battery_Check(p))
    {
         cmdbyte =  p->ucCmdData[0];              //�����룺ucCmdData[0]  �� ���ȣ� ucCmdData[1]����һ���ֽڣ� ucCmdData[2]
         BATTERY_Handle.State1=Battery_State_YD;  //�����YD���
        //ͨ�ż���
        switch(cmdbyte)
        {                     
            case BATTERY_VOTAGE:
           BATTERY_Handle.Y_Battery_Voltage1=(p->ucCmdData[3]<<8)+p->ucCmdData[4];
               break;
            case BATTERY_SOC:
           BATTERY_Handle.Y_Battery_Soc1=p->ucCmdData[2];  //(p->ucCmdData[3]<<8)+p->ucCmdData[4]
               break;
            case BATTERY_EE:

              BATTERY_Handle.Y_Battery_Openflag1=(p->ucCmdData[4]&0x04);
            
            
               break;
           case BATTERY_OPEN:
        
           BATTERY_Handle.Y_Battery_Data1 = p->ucCmdData[2];

               break;      
           default:
             break;
        }
//        USART2_SEND_DATA();    //�������������øߣ�Ϊ����ģʽ
    }
    return 0;
}
unsigned char Receive_N_Handle1(Communication_Handle_t* p)
{
    unsigned char cmdbyte;
    if(Receive_N_Battery_Check(p))
    {
         cmdbyte =  p->ucCmdData[2];
         BATTERY_Handle.State1=Battery_State_N;
        //ͨ�ż���
        switch(cmdbyte)
        {
             case N_COMMAND:
                 
               BATTERY_Handle.N_Battery_Voltage1= (p->ucCmdData[4]<<8)+p->ucCmdData[5];
               BATTERY_Handle.N_Battery_Current1=(p->ucCmdData[6]<<8)+p->ucCmdData[7];
               BATTERY_Handle.N_Battery_Power1= (p->ucCmdData[8]<<8)+p->ucCmdData[9];
               BATTERY_Handle.N_Battery_Soc1= p->ucCmdData[10];
               break;
           default:
             break;
             
        }
//        USART2_SEND_DATA();    //�������������øߣ�Ϊ����ģʽ
    }
    return 0;
} 
    


//�Ի�������������ݺ���  �����÷���ģʽ
unsigned char Receive_L_Handle(void)
{ 
    unsigned char cmdbyte;
    if(Receive_Check(&LS485_Handle))
    {
        cmdbyte =  LS485_Handle.ucCmdData[1];
        //ͨ�ż���
        switch(cmdbyte)
        {
            //��������״̬
           case MOTOR_STATE_1:
               if((LS485_Handle.ucCmdData[2]&0x01)==1)
               {
                 MOTOR_Handle1.motor_status=motor_stall;         ////////---------------------------��������������״̬
               }
               else if((LS485_Handle.ucCmdData[2]&0x02)==2)
               {
                 MOTOR_Handle1.motor_status=motor_over_temp;
               }
               else if((LS485_Handle.ucCmdData[2]&0x04)==4)
               {
                 MOTOR_Handle1.tMot.temp_state=temp_over;
               
               }
               else if((LS485_Handle.ucCmdData[2]&0x08)==8)
               {
                 MOTOR_Handle1.motor_status=motor_over_cur;
               
               }      
               else if((LS485_Handle.ucCmdData[2]&0x10)==16)
               {
                 MOTOR_Handle1.motor_status=motor_comm_fail;
               
               }
               else if((LS485_Handle.ucCmdData[2]&0x20)==32)
               {
                 MOTOR_Handle1.motor_status=motor_under_vol;
               
               }
               else if((LS485_Handle.ucCmdData[2]&0x40)==64)
               {
                 MOTOR_Handle1.motor_status=motor_over_vol;
               
               }
               else if((LS485_Handle.ucCmdData[2]&0x80)==128)
               {
                 MOTOR_Handle1.tMot.temp_state=temp_damage;
               
               }
               else if((LS485_Handle.ucCmdData[2]&0x01)==1)
               {    
                 MOTOR_Handle1.motor_status=motor_temp_warn;
               }
               else
               {
                 MOTOR_Handle1.motor_status=motor_nomal;
                 MOTOR_Handle1.tMot.temp_state=temp_nomal;
               }

               MOTOR_Handle1.Motor_Status=LS485_Handle.ucCmdData[2];

             break;
           case MOTOR_POWER:
               
                MOTOR_Handle1.power=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
           
             break;
           case DC_VOTAGE:

                MOTOR_Handle1.dc_votage=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break;
           case MOTOR_SPEED:

                MOTOR_Handle1.rpm=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break;  
           case MOTOR_CURRENT:
                MOTOR_Handle1.motor_current=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break;
           case MOS_MOTOR_TEMP:
                MOTOR_Handle1.motor_temp=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
                MOTOR_Handle1.mos_temp=(LS485_Handle.ucCmdData[4]<<8)+LS485_Handle.ucCmdData[5];
           
             break;
           case DC_CURRENT:
                MOTOR_Handle1.motor_dc_current=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break; 

           case RESET_CMD:
               
             break; 
           case DRIVER_SOFTHARD_VERSION:
               MOTOR_Handle1.product_type=LS485_Handle.ucCmdData[2];
               MOTOR_Handle1.soft_version=LS485_Handle.ucCmdData[3];
               MOTOR_Handle1.sotf_year_month=(LS485_Handle.ucCmdData[4]<<8)+LS485_Handle.ucCmdData[5];
               MOTOR_Handle1.hard_version=LS485_Handle.ucCmdData[6];
               MOTOR_Handle1.hard_year_month=(LS485_Handle.ucCmdData[7]<<8)+LS485_Handle.ucCmdData[8];
               MOTOR_Handle1.Get_Driver_Flag=1;
             break; 
           //ת�Ӱ�
          case GPS_SPEED:
               GPS_Handle1.speed =LS485_Handle.ucCmdData[2];
             break; 
          case GPS_NUMBEL:
               GPS_Handle1.num_sv=LS485_Handle.ucCmdData[2];
             break;     
          case GPS_LONGITUDE:
               GPS_Handle1.longtitude_H=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
               GPS_Handle1.longtitude_L=(LS485_Handle.ucCmdData[4]<<8)+LS485_Handle.ucCmdData[5];
             break;       
          case GPS_LATITUDE:
               GPS_Handle1.latitude_H=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
               GPS_Handle1.latitude_L=(LS485_Handle.ucCmdData[4]<<8)+LS485_Handle.ucCmdData[5];
             break;    
          case GPS_DIR_ANGLE:
               GPS_Handle1.driction_angle=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];

             break;   
          case GPS_RUNED_TIME:
               GPS_Handle1.traveled_time=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break;  
          case GPS_RUNED_DISTANCE:
               GPS_Handle1.traveled_distance=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break;  
         case GPS_RUNNING_TIME:
               GPS_Handle1.remain_time=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break; 
         case GPS_RUNNING_DISTANCE:
               
              GPS_Handle1.remain_distance=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
             break; 
         case ADAPTER_VERSION_INF:
              GPS_Handle1.adapter_soft_version=LS485_Handle.ucCmdData[2];
              GPS_Handle1.soft_year_month=(LS485_Handle.ucCmdData[3]<<8)+LS485_Handle.ucCmdData[4];
              GPS_Handle1.adapter_hard_version=LS485_Handle.ucCmdData[5];
              GPS_Handle1.hard_year_month=(LS485_Handle.ucCmdData[6]<<8)+LS485_Handle.ucCmdData[7];         
              MOTOR_Handle1.Get_Adapter_Flag=1;
         
             break; 
         case GPS_TIME:
              GPS_Handle1.TimeH=(LS485_Handle.ucCmdData[2]<<8)+LS485_Handle.ucCmdData[3];
              GPS_Handle1.TimeL=(LS485_Handle.ucCmdData[3]<<8)+LS485_Handle.ucCmdData[4];
              GPS_Handle1.Time=(GPS_Handle1.TimeH<<8)+GPS_Handle1.TimeL;
             break; 
         
           default:
             break;
        }

    }
    return 0;   
    
}






unsigned char Receive_YD_Handle2(Communication_Handle_t* p)
{

    unsigned char cmdbyte;
    if(Receive_YD_Battery_Check(p))
    {
         cmdbyte =  p->ucCmdData[0];
         BATTERY_Handle.State2=Battery_State_YD;
        //ͨ�ż���
        switch(cmdbyte)
        {                     
           case BATTERY_VOTAGE:
           BATTERY_Handle.Y_Battery_Voltage2=(p->ucCmdData[3]<<8)+p->ucCmdData[4];
               break;
           case BATTERY_SOC:
           BATTERY_Handle.Y_Battery_Soc2=p->ucCmdData[2];
               break;
           case BATTERY_EE:
           BATTERY_Handle.Y_Battery_Openflag2  =   (p->ucCmdData[4]&0x04);
               break;
           case BATTERY_OPEN:     
           BATTERY_Handle.Y_Battery_Data2 = p->ucCmdData[2];

               break;    
           default:
               
             break;
        }

    }
    return 0;
}
unsigned char Receive_N_Handle2(Communication_Handle_t* p)
{
    unsigned char cmdbyte;
    if(Receive_N_Battery_Check(p))
    {
         cmdbyte =  p->ucCmdData[2];
         BATTERY_Handle.State2=Battery_State_N;
        //ͨ�ż���
        switch(cmdbyte)
        {
             case N_COMMAND:
                 
               BATTERY_Handle.N_Battery_Voltage2= (p->ucCmdData[4]<<8)+p->ucCmdData[5];
               BATTERY_Handle.N_Battery_Current2=(p->ucCmdData[6]<<8)+p->ucCmdData[7];
               BATTERY_Handle.N_Battery_Power2= (p->ucCmdData[8]<<8)+p->ucCmdData[9];
               BATTERY_Handle.N_Battery_Soc2= p->ucCmdData[10];
               break;
           default:
             break;
             
        }
 //        USART2_SEND_DATA();    //�������������øߣ�Ϊ����ģʽ
    }
    return 0;
} 
    






//������ʾ�������ݴ���
u8 LCD_flag=0;
void Receive_M_Handle(void)
{
    unsigned char cmdbyte;
    if(Receive_Check(&MS485_Handle))
    {
        cmdbyte =  MS485_Handle.ucCmdData[1];
        //ͨ�ż���
        switch(cmdbyte)
        {
          //LCD�Ľ��յ�����Ϣ����
          case LCD_POWER_LIMIT:
                LCD_flag=1;
             break; 
          default:
             break;
        }    
    }
}


void USART1_IRQHandler(void)                	             //����1��Ӧ������� ����2��Ӧ�����Ҷ� �жϷ������
{
    u8 i=0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        Write_RxBuf(&LS485_Handle,USART1->DR); 

        if(LS485_Handle.RxBuf[0]==0x28)         
        {
          if(LS485_Handle.RxBuf[2]>=1)
          {
              if(LS485_Handle.RxBuf[LS485_Handle.RxBuf[2]+4]==0x29)
              { 
                Receive_L_Handle();                          //�������ݽ���
                  
                for(i=0;i<RS485_RXBUFSIZE;i++)
                {
                LS485_Handle.RxBuf[i]=0; 
                }
                LS485_Handle.RxTailPoint=0;    
              }
          }
        }
        else if(LS485_Handle.RxBuf[0]==BATTERY_HEADER)      //3A
        {
            if(LS485_Handle.RxBuf[3]>=1)
            {
               if(LS485_Handle.RxBuf[3+LS485_Handle.RxBuf[3]+4]==BATTERY_END2)
              { 
                Receive_YD_Handle1(&LS485_Handle);           //�������ݽ���
                  for(i=0;i<RS485_RXBUFSIZE;i++)
                {
                LS485_Handle.RxBuf[i]=0; 
                }
                LS485_Handle.RxTailPoint=0;   
                  
              }
            }
        }
        else if(LS485_Handle.RxBuf[0]==N_BATTERY_ADDR)       //90
        {
             if (LS485_Handle.RxTailPoint==14)
             {
                    Receive_N_Handle1(&LS485_Handle);        //�������ݽ���7+2=9
                   for(i=0;i<RS485_RXBUFSIZE;i++)
                   {
                    LS485_Handle.RxBuf[i]=0; 
                    }
                    LS485_Handle.RxTailPoint=0;   
                
             }   
        }  
        else
        { 
           for(i=0;i<RS485_RXBUFSIZE;i++)
          {
             LS485_Handle.RxBuf[i]=0; 
          }
            LS485_Handle.RxTailPoint=0; 
        }
    } 
} 
void USART2_IRQHandler(void) 
{
     u8 ii=0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
	    USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        Write_RxBuf(&RS485_Handle,USART2->DR);               //��ÿ�ν��ܵ������ݴ��룬����buff��
        if(RS485_Handle.RxBuf[0]==0x28)         
        {
          if(RS485_Handle.RxBuf[2]>=1)
            {
              if(RS485_Handle.RxBuf[RS485_Handle.RxBuf[2]+4]==0x29)
              { 
                Receive_R_Handle();        //�������ݽ���   
  
                for(ii=0;ii<RS485_RXBUFSIZE;ii++)
                {
                 RS485_Handle.RxBuf[ii]=0; 
                }
                 RS485_Handle.RxTailPoint=0;  
              }
            
            }            
        }
        else if(RS485_Handle.RxBuf[0]==BATTERY_HEADER)      //YDϵ�е��//�������ݽ���
        { 
            
            if(RS485_Handle.RxBuf[3]>=1)
            {
               if(RS485_Handle.RxBuf[3+RS485_Handle.RxBuf[3]+4]==BATTERY_END2)
              { 
                 Receive_YD_Handle2(&RS485_Handle);
                 for(ii=0;ii<RS485_RXBUFSIZE;ii++)
                 {
                  RS485_Handle.RxBuf[ii]=0; 
                 }
                 RS485_Handle.RxTailPoint=0;   
              }
            }
            
            
        }
        else if(RS485_Handle.RxBuf[0]==N_BATTERY_ADDR)    //Nϵ�е��//�������ݽ���
        {
            
                if (RS485_Handle.RxTailPoint==14)
                {
                    Receive_N_Handle2(&RS485_Handle);       //�������ݽ���7+2=9
                    for(ii=0;ii<RS485_RXBUFSIZE;ii++)
                    {
                     RS485_Handle.RxBuf[ii]=0; 
                    }
                     RS485_Handle.RxTailPoint=0; 
                }                      
  
        }
        else
        { 
           for(ii=0;ii<RS485_RXBUFSIZE;ii++)
          {
             RS485_Handle.RxBuf[ii]=0; 
          }
             RS485_Handle.RxTailPoint=0; 
        }
    } 
}


void USART3_IRQHandler(void) 
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
	    USART_ClearITPendingBit(USART3,USART_IT_RXNE);
        Write_RxBuf(&MS485_Handle,USART3->DR);
        if(MS485_Handle.RxBuf[0]==0x28)         
        {
            if(MS485_Handle.RxBuf[MS485_Handle.RxTailPoint-1]==0x29)
            { 
                Receive_M_Handle();            //�������ݽ���
                MS485_Handle.RxTailPoint=0;   
            }   
        }
    } 
}




