#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include "adapter.h"
#include "battery.h"
#include "motor_driver.h"
/*****�궨��******/
#define USART1_GPIO      GPIOA
#define USART1_TX_Pin    GPIO_Pin_9
#define USART1_RX_Pin    GPIO_Pin_10

#define USART1_REDE_Pin  GPIO_Pin_13    //PB13λ�� ���ͽ���ʹ�����ţ��˴���Ҫ�Ķ�����ΪCAN��ͨ������

#define USART2_GPIO      GPIOA
#define USART2_TX_Pin    GPIO_Pin_2
#define USART2_RX_Pin    GPIO_Pin_3
#define USART2_REDE_Pin  GPIO_Pin_1

#define USART3_GPIO      GPIOB
#define USART3_TX_Pin    GPIO_Pin_10
#define USART3_RX_Pin    GPIO_Pin_11
#define USART3_REDE_Pin  GPIO_Pin_12

#define USART1_SEND_DATA()          GPIO_SetBits(GPIOB , GPIO_Pin_13)
#define USART1_RECIVE_DATA()        GPIO_ResetBits(GPIOB , GPIO_Pin_13)

#define USART2_SEND_DATA()          GPIO_SetBits(USART2_GPIO , USART2_REDE_Pin)
#define USART2_RECIVE_DATA()        GPIO_ResetBits(USART2_GPIO , USART2_REDE_Pin)

#define USART3_SEND_DATA()          GPIO_SetBits(USART3_GPIO , USART3_REDE_Pin)
#define USART3_RECIVE_DATA()        GPIO_ResetBits(USART3_GPIO , USART3_REDE_Pin)

//�궨��
#define RS485_RXBUFSIZE      19    //�����ֽ���
#define RS485_TXBUFSIZE      19    //�����ֽ���
//����ͷ
#define CMD_HEADER           0xCA 
//�°汾����ͷ
#define HEADER_CMD           0x28 
//�Ի���ַ  
#define DRIVE_ADDRESS        0x01   //������ַ 
#define ADAPTER_ADDRESS      0x02   //ת�Ӱ��ַ 
#define LCD_ADDRESS          0x03   //LCD��ַ 

//���ݳ��� 
#define DATA_LENG_1            1    //length
#define DATA_LENG_2            2    //length

#define DATA_LENG_3            3    //length
#define DATA_LENG_5            5    //length
#define DATA_LENG_7            7    //length


//��䷢�����ݵ�������
#define R_POWER              0x66
#define L_POWER              0x68
#define Only_R_POWER         0x69
#define Only_L_POWER         0x70
#define MOTO_INFMATION       0x71
#define DRIVER_VERSION       0x72
#define ADAPTER_VERSION      0x73
#define ADAPTER_GPS          0x74



#define MOTOR_STATE           0x41
//���͸���������������
#define THROTTLE              0x01   //����������Ϣ
#define MOTOR_STOP            0x02   //���͵��ͣ��ָ����Ϣ
#define GET_DRIVER_INF        0x03   //��ȡ��������Ϣ
//��������   �������ظ�Զ����
#define MOTOR_STATE_1         0x12
#define MOTOR_POWER           0x13
#define DC_VOTAGE             0x14
#define MOTOR_SPEED           0x15
#define MOTOR_CURRENT         0x16
#define MOS_MOTOR_TEMP        0x17
#define DC_CURRENT            0x18
#define RESET_CMD             0x1A
#define DRIVER_SOFTHARD_VERSION  0x1B   //13���ֽ�
//˫Զ�ٷ�������ת�Ӱ�
#define GET_ADAPTER_INF          0x38
#define GET_GPS_DATA             0x3B
#define SWICTH_SLAVE             0x3C
//����ת�Ӱ�ظ�Զ����    �ظ����ݸ�

#define GPS_SPEED                0x48
#define GPS_NUMBEL               0x49
#define GPS_LONGITUDE            0x4A //����γ��
#define GPS_LATITUDE             0x4B
#define GPS_DIR_ANGLE            0x4C
#define GPS_RUNED_TIME           0x4D
#define GPS_RUNED_DISTANCE       0x4E
#define GPS_RUNNING_TIME         0x4F
#define GPS_RUNNING_DISTANCE     0x50
#define ADAPTER_VERSION_INF      0x5E

#define GPS_TIME                 0x60
//˫Զ�ٷ��͸�LCD
#define LCD_MOTOR_MSG            0xA1
#define LCD_ADAPTER_MSG          0xA2
#define LCD_BATTERY_MSG          0xA3
#define LCD_THROTTLE_MSG         0xA4
#define LCD_MOTOR_MSG1           0xA5
#define LCD_ADAPTER_MSG1         0xA6
#define LCD_BATTERY_MSG1         0xA7
////#define LCD_THROTTLE_MSG1        0xA8
//����LCD��������
#define LCD_POWER_LIMIT          0xB0



//β��
#define END                      0X29



//YD��ط��͵�����֡��ʽ
//��
#define BATTERY_HEADER           0X3A   //0X0A
#define BATTERY_ADDR             0X16

//������
#define BATTERY_TEMP             0X08
#define BATTERY_VOTAGE           0X09
#define BATTERY_SOC              0X0E  //��������
#define BATTERY_OPEN             0x00
#define BATTERY_EE               0X2E  //�澯��Ϣ


//������
#define BATTERY_END1             0X0D
#define BATTERY_END2             0X0A

//Nϵ�е�ط��͵�����֡��ʽ
//�Ի���ַ
#define N_BATTERY_ADDR           0X90

//������
#define N_FUNCYION               0X03
//������
#define N_COMMAND                0XA0
//���ݳ���
#define N_LENGTH                 7



typedef enum 
{
	State_Disconnected,
	State_Connected,
	State_Error,
}Communication_State_e;
typedef enum 
{
    CommunicationState_Open_Battery_Msg,
    CommunicationState_BatteryOpen_State_Msg,
	CommunicationState_Get_Driver_Msg,
    CommunicationState_Get_Adapter_Msg,
	CommunicationState_Driver_Power,
    CommunicationState_Adapter_GPS,
    CommunicationState_Battery,
    CommunicationState_LCD_Driver1,
    CommunicationState_LCD_Driver2,
    CommunicationState_LCD_GPS1,
    CommunicationState_LCD_GPS2,

    CommunicationState_LCD_Throttle,
    CommunicationState_LCD_Battery1,
    CommunicationState_LCD_Battery2,
}Communication_e;

typedef struct
{	
	u8  			         CommFailCnt;	        //+1 for every transmission and cleared at each decode	 
	u8                       RxBuf[RS485_RXBUFSIZE];
    u8                       TxBuf[RS485_TXBUFSIZE];
    u8                       RxHeadPoint;           //����ͷ
    u8                       RxTailPoint;           //����β
    u8                       TxHeadPoint;           //����ͷ
    u8                       TxTailPoint;           //����β
    u8                       Tx_Size;       
    u8                       Rx_Size; 
    u8                       TxCount;       
    u8                       RxCount;        
	uint16_t                 Tx_msg;                 //  
    uint16_t                 Rx_msg;                 // 
    
	Communication_State_e    State;                  //
    Communication_e      	 Comm_State;             //
   
    u8                       cmdStep;
    u8                       cmdPtr;
    u16                       ucCheckSum;
    u8                       ucCmdData[RS485_RXBUFSIZE];
}Communication_Handle_t;

extern Communication_Handle_t  RS485_Handle;   //�ұߵ�485ͨ�Ŵ���
extern Communication_Handle_t  LS485_Handle;   //��ߵ�485ͨ�Ŵ���
extern Communication_Handle_t  MS485_Handle;   //�м��485ͨ�Ŵ���

void USART1_Config(u32 bound);
void USART2_Config(u32 bound);
void USART3_Config(u32 bound);
void USART1_2_3_Init(u32 bound);

void UART1Test(void);
void UART2Test(void);
void Send_Mode(void);
void Recive_Mode(void);

void UART1_SendByte(uint8_t SendData);
void UART2_SendByte(uint8_t SendData);
void UART3_SendByte(uint8_t SendData);

void UART1_SendHalfWord(uint16_t data);
void UART2_SendHalfWord(uint16_t data);
void UART3_SendHalfWord(uint16_t data);

void UART1_SendArray(uint8_t *array ,uint8_t num);
void UART2_SendArray(uint8_t *array ,uint8_t num);
void UART3_SendArray(uint8_t *array ,uint8_t num);

void UART1_SendStr(uint8_t *str );
void UART2_SendStr(uint8_t *str );
void UART3_SendStr(uint8_t *str );

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

u8 Write_RxBuf(Communication_Handle_t* p,u8 ch);

u8 Put_TxBuf(Communication_Handle_t* p,u8 ch,u8 num);

u8 Read_TxBuf (Communication_Handle_t* p);
void Tx_USART1_Data(void);
void Tx_USART2_Data(void);
void Tx_USART3_Data(void);

void SendData_Filling(u8 cmdbyte,u8 length,u8 address);//����Э��Ĳ�ͬ��������ݺ���Ϊ����
void YD_BatteryData_Filling(u8 cmdbyte);
void N_BatteryData_Filling(u8 cmdbyte);

void Tx_USART1_Data(void);   //
void Tx_USART2_Data(void);  //
void Tx_USART3_Data(void);
void SendRL_Handle(void);

u8 Read_RxBuf(Communication_Handle_t* p,u8* ch,u8 num);
//u8 Read_RxBuf(Communication_Handle_t* p,u8 *ch);
u8 ReceiveData_Check(Communication_Handle_t* p);
unsigned char Receive_YD_Handle1(Communication_Handle_t* p);
unsigned char Receive_N_Handle1(Communication_Handle_t* p);
unsigned char Receive_R_Handle(void);
unsigned char Receive_L_Handle(void);
unsigned char Receive_YD_Handle2(Communication_Handle_t* p);
unsigned char Receive_N_Handle2(Communication_Handle_t* p);
void Receive_M_Handle(void);

#endif /* __USART1_H */


