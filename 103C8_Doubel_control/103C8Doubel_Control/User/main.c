/********************************************************
 * �ļ���  ��main.c
 * ����    ��˫Զ����Ŀ����һ̨˫Զ��ʵ����̨������Ŀ���
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * ��汾  ��1.0 
 * ������  ��zyf 
*********************************************************/
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
//���������ͷ�ļ�
#include "usart1.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "i2c.h"
#include "iwdg.h"
#include "buzzer.h"
#include "angle_as5600.h"
#include "flash.h"
#include "key_control_mode.h"
#include "can.h"

void Disable_All(void)
{
    TIM_Cmd(TIM1, DISABLE);
    USART_Cmd(USART1, DISABLE);// USART1ʹ��
    USART_Cmd(USART2, DISABLE);// USART1ʹ��
}

void Enable_All(void)
{
    TIM_Cmd(TIM1, ENABLE);
    USART_Cmd(USART1, ENABLE);// USART1ʹ��
    USART_Cmd(USART2, ENABLE);// USART1ʹ��
}

KEY_Handle_t KEY_Handle;
void RCC_Configuration(void)
{
    //ErrorStatus HSEStartUpStatus;
    u32 tmpreg = 0;
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
    /* Enable HSI */
    RCC_HSICmd(ENABLE);      //ʹ�õ��ڲ�ʱ��
    /* HCLK = SYSCLK */
    RCC->CFGR &=0xFFFFFF0F;  //CFGR_HPRE_Reset_Mask;//
    /* PCLK2 = HCLK */
    RCC->CFGR &=0xFFFFC7FF;  //CFGR_PPRE2_Reset_Mask;//
    /* PCLK1 = HCLK/2 */
    tmpreg = RCC->CFGR;      //
    tmpreg &= 0xFFFFF8FF;    //CFGR_PPRE1_Reset_Mask;
    tmpreg |= 0x00000400;    //RCC_HCLK_Div2;
    RCC->CFGR = tmpreg;      //APB1 2��Ƶ���
    /* Flash 2 wait state */
    FLASH->ACR &=0x00000038;//ACR_LATENCY_Mask;//
    FLASH->ACR |= 0x00000002;//FLASH_Latency_2;//
    /* Enable Prefetch Buffer */
    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    FLASH->ACR &= 0xFFFFFFEF;//ACR_PRFTBE_Mask;//
    FLASH->ACR |=0x00000010;//FLASH_PrefetchBuffer_Enable;//
    /* PLLCLK = 8MHz/2 * 16 = 64 MHz */
    tmpreg = RCC->CFGR;//
    tmpreg &= 0xFFC0FFFF;//CFGR_PLL_Mask;
    tmpreg |= 0x00388000;//RCC_PLLMul_16
    RCC->CFGR = tmpreg;
    /* Enable PLL */
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    //asm("nop");//151126-1
    }
    /* Select PLL as system clock source */
    tmpreg = RCC->CFGR;//
    tmpreg &= 0xFFFFFFFC;//CFGR_SW_Mask;
    tmpreg |= 0x00000002;//RCC_SYSCLKSource_PLLCLK;
    RCC->CFGR = tmpreg;
    while(((u8)(RCC->CFGR & 0x0000000C)) != 0x08)//
    {
        //asm("nop");//151126-1
    } 
}

//�����õı������壬��ɾ����
uint16_t RawAngle = 0;
uint16_t R_Angle  = 0,R_gAngle = 0;
uint16_t res      = 0;
uint8_t  gAGC     = 0;
uint16_t Config   = 0;
uint16_t L_Angle  = 0,L_gAngle =0;
uint16_t R_Magitue = 0;
uint16_t L_Magitue = 0;
uint8_t  temp;
uint8_t  temp_flag;
uint16_t CC = 0;
int main(void)
{  
    delay_init();                                // ��ʱ������ʼ��
    delay_ms(100);                               // �ȴ����Ԫ�����ϵ粢�ҹ�������
    RCC_Configuration();                         // 64M��Ƶ
    USART1_2_3_Init(38400);                       
    CAN1_Configuration();                         
    Key_GPIO_Init();                             
    LED_GPIO_Init();                             
    MOTOR_Init(&MOTOR_Handle);                     
    MOTOR_Init(&MOTOR_Handle1);                   
    GPS_Init(&GPS_Handle);                      
    GPS_Init(&GPS_Handle1);                       
    BATTERY_Init(&BATTERY_Handle);               
    THROTTLE_Init(&THROTTLE_Handle);             
    Open_Close_Switch_Init();                    //��ʼ����״̬��ʼ��                         
    TIM1_Init();
//  ADC123_Init();
    IIC_Init();
    Flash_Read_Angle(&FLASH_Handle);             //flash��ȡ�洢ֵ
//  as5600�Ƕȴ�����
    R_AS5600_SetOutputStage(OUT_STG_ANALOG);     //
    L_AS5600_SetOutputStage(OUT_STG_ANALOG);     //
	R_AS5600_SetPMMode(AS5600_PM_MODE_LPM2);     //power lpm2
	L_AS5600_SetPMMode(AS5600_PM_MODE_LPM2);     //power lpm2
	R_AS5600_EnableWd();                         //ʹ��оƬ���Ź�д��
	L_AS5600_EnableWd();                         //ʹ��оƬ���Ź�д��
    IWDG_Init(4,1200);     
    MS485_Handle.Comm_State=CommunicationState_LCD_Driver1;
    while (1)
    {	 
        if(counter_1ms_flag==1)
        {  
          counter_1ms_flag=0;
          THROTTLE_Handle.R_angle=R_AS5600_GetAngle();     //���Ե�����������ֵ
          THROTTLE_Handle.L_angle=L_AS5600_GetAngle();    
        }
        if(counter_10ms_flag==1)
        {  
          counter_10ms_flag=0;   
          THROTTLE_Handle.Temp_Val++; 
          #if 0   
          THROTTLE_Handle.CAN_TxBuf[0]= 100;
          THROTTLE_Handle.CAN_TxBuf[1]= 1;
          THROTTLE_Handle.CAN_TxBuf[2]= 100;
          THROTTLE_Handle.CAN_TxBuf[3]= 0;
          CAN_Send_Msg(THROTTLE_Handle.CAN_TxBuf,4);
          #endif 
          #if 1  
          BUZZER_Work();  //����������   
          if(Open_Close_Switch_Handle.Open_Flag==1)
          {
               //�ж�У���Ƕȣ��Ƿ���ȷ
              if(THROTTLE_Handle.State == THROTTLE_State_Cal_Stop)
              {
                  //if(THROTTLE_Handle.angle > 360)   return 0;     
                  THROTTLE_Calc(&THROTTLE_Handle);//˫Զ���ж�У���Ƿ���ȷ ����У���㷨 
                  if(THROTTLE_Handle.L_CalSuc_Flag==1&&THROTTLE_Handle.R_CalSuc_Flag==1)
                  { 
                      //flash���ݴ洢    
                      Flash_Save_Angle(&FLASH_Handle);
                      if(FLASH_Handle.L_Flash_SaveSuc_Flag==1&&FLASH_Handle.R_Flash_SaveSuc_Flag==1)      
                      {
                          THROTTLE_Handle.State=THROTTLE_State_Success;    //У��������ɣ�У���� 
                      }
                      else
                      {
                          THROTTLE_Handle.State=THROTTLE_State_NeedReset;  //У��������ɣ���ҪУ����             
                      }
                  } 
                  else
                  {
                      THROTTLE_Handle.State=THROTTLE_State_NeedReset;      //У��������ɣ���ҪУ����
                  }
              } 
              else if(THROTTLE_Handle.State == THROTTLE_State_Success)
              { 
                  //�����������Ͻ�У�������ݣ�ת��0��127����ֵ
                  Throttle_Value_Conversion(&THROTTLE_Handle); 
                  //10ms��һ�Σ�458ͨ������,         
                  //������У������458ͨ�����ݲ��������ź�ok
                  //�ж��Ƿ����Ż���  ���ػ�����  hallԿ�׶Ͽ�����û���ã�����һ�����Ż�������λ 
                  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))
                  {
                      THROTTLE_Handle.L_SetZero_Flag=0; 
                      THROTTLE_Handle.R_SetZero_Flag=0;
                      THROTTLE_Handle.Saft_Key=0;
                  }
                  else /*��ȫ�����Ѿ�����*/
                  {      
                      THROTTLE_Handle.Saft_Key=1;                      
                      if(THROTTLE_Handle.L_Percent==0&&THROTTLE_Handle.R_Percent==0)
                      {
                          THROTTLE_Handle.L_SetZero_Flag=1; 
                          THROTTLE_Handle.R_SetZero_Flag=1;
                      }
                  }
                  if(THROTTLE_Handle.L_SetZero_Flag==1&&THROTTLE_Handle.R_SetZero_Flag==1)
                  {
                      THROTTLE_Handle.R_PercentTemp=THROTTLE_Handle.R_Percent*THROTTLE_Handle.Limit_TEMP/127;
                      
                      THROTTLE_Handle.L_PercentTemp=THROTTLE_Handle.L_Percent*THROTTLE_Handle.Limit_TEMP/127;  //THROTTLE_Handle.L_PercentTemp=THROTTLE_Handle.L_Percent*THROTTLE_Handle.Limit_TEMP/127;
                  }
                  else
                  {
                      THROTTLE_Handle.R_PercentTemp=0;
                      THROTTLE_Handle.L_PercentTemp=0;
                  }  
                  /* CANЭ�鷢������10ms */ 
                  #if 1  
                  if(DOCK_ContorlMode.R_L_Change_Flag==1)
                  {
                    THROTTLE_Handle.CAN_TxBuf[0]= THROTTLE_Handle.R_PercentTemp*100/127;
                    THROTTLE_Handle.CAN_TxBuf[1]= THROTTLE_Handle.R_Direction;
                    THROTTLE_Handle.CAN_TxBuf[2]= THROTTLE_Handle.L_PercentTemp*100/127;
                    THROTTLE_Handle.CAN_TxBuf[3]= THROTTLE_Handle.L_Direction;
                  }
                  else
                  {
                    THROTTLE_Handle.CAN_TxBuf[0]= THROTTLE_Handle.L_PercentTemp*100/127;
                    THROTTLE_Handle.CAN_TxBuf[1]= THROTTLE_Handle.L_Direction;
                    THROTTLE_Handle.CAN_TxBuf[2]= THROTTLE_Handle.R_PercentTemp*100/127;
                    THROTTLE_Handle.CAN_TxBuf[3]= THROTTLE_Handle.R_Direction;
                  }                   
                  CAN_Send_Msg(THROTTLE_Handle.CAN_TxBuf,8);
                  #endif    
                  //ͨ��485�������� 50ms  
                  if(THROTTLE_Handle.Temp_Val%5==0)
                  {
                      switch(RS485_Handle.Comm_State)
                      {
                          //���͵�ش�ĸ�ߵ�����
                          case CommunicationState_Open_Battery_Msg: 
                              YD_BatteryData_Filling(BATTERY_OPEN); 

                              USART1_SEND_DATA();
                              UART1_SendArray(LS485_Handle.TxBuf,11);
                              USART1_RECIVE_DATA();
                          
                              USART2_SEND_DATA();
                              UART2_SendArray(RS485_Handle.TxBuf,11);
                              USART2_RECIVE_DATA();
                              if( MOTOR_Handle.Get_Driver_Flag==1&&MOTOR_Handle1.Get_Driver_Flag==1&&BATTERY_Handle.Y_Battery_Openflag1==4&&BATTERY_Handle.Y_Battery_Openflag2==4)
                              {
                                RS485_Handle.Comm_State=CommunicationState_Driver_Power;
                              }
                              else 
                                RS485_Handle.Comm_State=CommunicationState_BatteryOpen_State_Msg;
                           break;
                          case CommunicationState_BatteryOpen_State_Msg: 
                              YD_BatteryData_Filling(BATTERY_EE); 
                              USART1_SEND_DATA();
                              UART1_SendArray(LS485_Handle.TxBuf,9);
                              USART1_RECIVE_DATA();
                          
                              USART2_SEND_DATA();
                              UART2_SendArray(RS485_Handle.TxBuf,9);
                              USART2_RECIVE_DATA();
                              RS485_Handle.Comm_State=CommunicationState_Get_Driver_Msg;
                           break; 

                          case CommunicationState_Get_Driver_Msg:
                             SendData_Filling(DRIVER_VERSION,DATA_LENG_1,DRIVE_ADDRESS);

                             USART1_SEND_DATA();
                             UART1_SendArray(LS485_Handle.TxBuf ,DATA_LENG_1+5);
                             USART1_RECIVE_DATA();  
                          
                             USART2_SEND_DATA();
                             UART2_SendArray(RS485_Handle.TxBuf ,DATA_LENG_1+5); 
                             USART2_RECIVE_DATA();

                             RS485_Handle.Comm_State=CommunicationState_Open_Battery_Msg;  
                                
                          break;
                          case CommunicationState_Get_Adapter_Msg:
                             SendData_Filling(ADAPTER_VERSION,DATA_LENG_1,ADAPTER_ADDRESS);

                             USART1_SEND_DATA();
                             UART1_SendArray(LS485_Handle.TxBuf ,DATA_LENG_1+5);
                             USART1_RECIVE_DATA();
                          
                             USART2_SEND_DATA();
                             UART2_SendArray(RS485_Handle.TxBuf ,DATA_LENG_1+5);
                             USART2_RECIVE_DATA();
                             RS485_Handle.Comm_State=CommunicationState_Get_Driver_Msg;  

                          break; 
                          case CommunicationState_Driver_Power:
                              if(SYNC_ContorlMode.State==ControlMode_State_0)
                              {           
                                  SendData_Filling(R_POWER,DATA_LENG_3,DRIVE_ADDRESS); 
                                  SendData_Filling(L_POWER,DATA_LENG_3,DRIVE_ADDRESS);                     
                              }             
                              else if(SYNC_ContorlMode.State==ControlMode_State_R)
                              {
                                  SendData_Filling(Only_R_POWER,DATA_LENG_3,DRIVE_ADDRESS); 
                              }
                              else if(SYNC_ContorlMode.State==ControlMode_State_L)  
                              {   
                                  SendData_Filling(Only_L_POWER,DATA_LENG_3,DRIVE_ADDRESS); 
                              }          
                              USART1_SEND_DATA();
                              UART1_SendArray(LS485_Handle.TxBuf ,DATA_LENG_3+5);
                              USART1_RECIVE_DATA();
                              USART2_SEND_DATA();
                              UART2_SendArray(RS485_Handle.TxBuf ,DATA_LENG_3+5);
                              USART2_RECIVE_DATA();    
                              RS485_Handle.Comm_State=CommunicationState_Battery;
                          break;
                          case CommunicationState_Adapter_GPS:
                              SendData_Filling(ADAPTER_GPS,DATA_LENG_1,ADAPTER_ADDRESS);  
                              USART1_SEND_DATA();
                              UART1_SendArray(LS485_Handle.TxBuf ,DATA_LENG_1+5);
                              USART1_RECIVE_DATA();
                          
                              USART2_SEND_DATA();
                              UART2_SendArray(RS485_Handle.TxBuf ,DATA_LENG_1+5);
                              USART2_RECIVE_DATA();  
                              RS485_Handle.Comm_State=CommunicationState_Battery;
                          break;
                          case CommunicationState_Battery:
                              //���Ͳ�ͬ���Э�������֡ 
                              //YD�ĵ��Э��     
                            if(BATTERY_Handle.State1==Battery_State_YD)    
                            {   
//                                if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                               {                     
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE);                                
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;
//                               }
//                               else                                 
//                               {
//                                      BATTERY_Handle.Battery_Protocol_Flag=0;
//                                      YD_BatteryData_Filling(BATTERY_SOC); 
//                               }
                                 YD_BatteryData_Filling(BATTERY_SOC); 
                                 USART1_SEND_DATA();
                                 UART1_SendArray(LS485_Handle.TxBuf ,9);
                                 USART1_RECIVE_DATA();         
                            }
                          
                            if(BATTERY_Handle.State2==Battery_State_YD)    
                            {   
//                                if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                               {                     
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE);                                
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;
//                               }
//                               else                                 
//                               {
//                                      BATTERY_Handle.Battery_Protocol_Flag=0;
//                                      YD_BatteryData_Filling(BATTERY_SOC); 
//                                }
                                 YD_BatteryData_Filling(BATTERY_SOC); 

                                USART2_SEND_DATA();
                                UART2_SendArray(RS485_Handle.TxBuf ,9);
                                USART2_RECIVE_DATA();
                            }
                            RS485_Handle.Comm_State=CommunicationState_Driver_Power; 
                            
//                              if(BATTERY_Handle.State1==Battery_State_Start)    
//                              {                                      
//                                  if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                                  {
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE); 
//                                      Send_Mode();
//                                      UART1_SendArray(LS485_Handle.TxBuf ,9);
//                                      UART2_SendArray(RS485_Handle.TxBuf ,9);
//                                      Recive_Mode();
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;                         
//                                  }
//                                  else                                 
//                                  {        

//                                      N_BatteryData_Filling(N_COMMAND);
//                                      Send_Mode();
//                                      UART1_SendArray(LS485_Handle.TxBuf ,6);
//                                      UART2_SendArray(RS485_Handle.TxBuf ,6);  
//                                      Recive_Mode();
//                                      BATTERY_Handle.Battery_Protocol_Flag=0; 
//                                  }
//                              }
//                               if(BATTERY_Handle.State2==Battery_State_Start)    
//                              {                                      
//                                  if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                                  {
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE); 
//                                      Send_Mode();
////                                    UART1_SendArray(LS485_Handle.TxBuf ,9);
//                                      UART2_SendArray(RS485_Handle.TxBuf ,9);
//                                      Recive_Mode();
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;                         
//                                  }
//                                  else                                 
//                                  {        

//                                      N_BatteryData_Filling(N_COMMAND);
//                                      Send_Mode();
////                                      UART1_SendArray(LS485_Handle.TxBuf ,6);
//                                      UART2_SendArray(RS485_Handle.TxBuf ,6);  
//                                      Recive_Mode();
//                                      BATTERY_Handle.Battery_Protocol_Flag=0; 
//                                  }
//                              }
//                              else if(BATTERY_Handle.State1==Battery_State_YD&&BATTERY_Handle.State2==Battery_State_YD)
//                              {
//                                  if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                                  {                     
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE);                                
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;
//                                  }
//                                  else                                 
//                                  {
//                                      BATTERY_Handle.Battery_Protocol_Flag=0;
//                                      YD_BatteryData_Filling(BATTERY_SOC); 
//                                  }
//                                  Send_Mode();
//                                  UART1_SendArray(LS485_Handle.TxBuf ,9);
//                                  UART2_SendArray(RS485_Handle.TxBuf ,9);
//                                  Recive_Mode();
//   
//                                  
//                              }
//                              else if(BATTERY_Handle.State1==Battery_State_N&&BATTERY_Handle.State2==Battery_State_N) 
//                              {
//                                  N_BatteryData_Filling(N_COMMAND); //Nϵ�еĵ��Э�������֡
//                                  Send_Mode();
//                                  UART1_SendArray(LS485_Handle.TxBuf ,6);
//                                  UART2_SendArray(RS485_Handle.TxBuf ,6);
//                                  Recive_Mode();
//                              } 
//                              else if(BATTERY_Handle.State1==Battery_State_N&&BATTERY_Handle.State2==Battery_State_NO_Bat) 
//                              {
//                                  N_BatteryData_Filling(N_COMMAND); //Nϵ�еĵ��Э�������֡
//                                  USART1_SEND_DATA();
//                                  UART1_SendArray(LS485_Handle.TxBuf ,6);
//                                  USART1_RECIVE_DATA();
//                                  
//   
//                              }
//                              else if(BATTERY_Handle.State1==Battery_State_NO_Bat&&BATTERY_Handle.State2==Battery_State_N) 
//                              {
//                                  N_BatteryData_Filling(N_COMMAND); //Nϵ�еĵ��Э�������֡
//                                  USART2_SEND_DATA();
//                                  UART2_SendArray(RS485_Handle.TxBuf ,6);
//   
//                                  USART2_RECIVE_DATA();
//                              }
//                              else if(BATTERY_Handle.State1==Battery_State_YD&&BATTERY_Handle.State2==Battery_State_NO_Bat) 
//                              {
//                                  if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                                  {                     
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE);                                
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;
//                                  }
//                                  else                                 
//                                  {
//                                      BATTERY_Handle.Battery_Protocol_Flag=0;
//                                      YD_BatteryData_Filling(BATTERY_SOC); 
//                                  }
//                                  USART1_SEND_DATA();
//                                  UART1_SendArray(LS485_Handle.TxBuf ,9);
//                                  USART1_RECIVE_DATA();
//   
//                              }
//                              else if(BATTERY_Handle.State1==Battery_State_NO_Bat&&BATTERY_Handle.State2==Battery_State_YD) 
//                              {
//                                  if(BATTERY_Handle.Battery_Protocol_Flag==0)                     
//                                  {                     
//                                      YD_BatteryData_Filling(BATTERY_VOTAGE);                                
//                                      BATTERY_Handle.Battery_Protocol_Flag=1;
//                                  }
//                                  else                                 
//                                  {
//                                      BATTERY_Handle.Battery_Protocol_Flag=0;
//                                      YD_BatteryData_Filling(BATTERY_SOC); 
//                                  }
//                                  USART2_SEND_DATA();
//                                  UART2_SendArray(RS485_Handle.TxBuf ,9);
//   
//                                  USART2_RECIVE_DATA();
//   
//                              }                              
//                              else if(BATTERY_Handle.State1==Battery_State_NO_Bat&&BATTERY_Handle.State2==Battery_State_NO_Bat)                        
//                              {
// 
//                              }  

                          break;                           
                          default:
                          break;                
                      }   
                  }

                  
              }
              else if(THROTTLE_Handle.State == THROTTLE_State_NeedReset)
              {
                  LED_Handle.LED_Key4_Trigger_flag = 1;   //LED
                  Buzzer_Handle.Buzzer_one_flag=1;        //������  
              }
          }
          else
          {
              //�ػ�һЩ������Ҫ��ʼ�����ã�������Key2
              THROTTLE_Handle.L_SetZero_Flag=0; 
              THROTTLE_Handle.R_SetZero_Flag=0;
              THROTTLE_Handle.R_PercentTemp=0;
              THROTTLE_Handle.L_PercentTemp=0;
              MOTOR_Init(&MOTOR_Handle);
              MOTOR_Init(&MOTOR_Handle1);
              GPS_Init(&GPS_Handle);
              GPS_Init(&GPS_Handle1);
              Flash_Read_Angle(&FLASH_Handle);            //flash��ȡ�洢ֵ
              RS485_Handle.Comm_State=CommunicationState_Open_Battery_Msg;
          }
          #endif
          //�����õı����鿴
          #if 1
            R_Angle &=0x00;
			R_Angle |=R_AS5600_ReadOneByte(ANGLE_H);
			R_Angle<<=8;
			R_Angle |=R_AS5600_ReadOneByte(ANGLE_L);
			R_gAngle = (float)R_Angle/4096*360;
            
            L_Angle &=0x00;
			L_Angle |=L_AS5600_ReadOneByte(ANGLE_H);
			L_Angle<<=8;
			L_Angle |=L_AS5600_ReadOneByte(ANGLE_L);
			L_gAngle = (float)L_Angle/4096*360;  
//			res = R_AS5600_GetStatus();
//			RawAngle = R_AS5600_GetRawAngle();
			Config = R_AS5600_ReadConf();
			gAGC = R_AS5600_GetAGC();
			R_Magitue = R_AS5600_ReadMagitue(); 
            L_Magitue = L_AS5600_ReadMagitue(); 
            #endif
            
          #if 0
            switch(MS485_Handle.Comm_State)
            { 
                case CommunicationState_LCD_Driver1:
                    SendData_Filling(LCD_MOTOR_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Driver2;
                   break;
                
                case CommunicationState_LCD_Driver2:
                    SendData_Filling(LCD_MOTOR_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_GPS1;
                   break;
                
                case CommunicationState_LCD_GPS1:
                    SendData_Filling(LCD_ADAPTER_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_GPS2;
                   break;
                
                case CommunicationState_LCD_GPS2:
                    SendData_Filling(LCD_ADAPTER_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Throttle;
                   break;
               
                 case CommunicationState_LCD_Throttle:
                    SendData_Filling(LCD_THROTTLE_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Battery1;
                   break; 
                 
                 case CommunicationState_LCD_Battery1:
                    SendData_Filling(LCD_BATTERY_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Battery2;
                   break;
                
                case CommunicationState_LCD_Battery2:
                    SendData_Filling(LCD_BATTERY_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Driver1;
                   break;
                
                default:
                   break; 
            } 
            USART3_SEND_DATA();               
            UART3_SendArray(MS485_Handle.TxBuf,12);
            USART3_RECIVE_DATA();
          #endif
    
        }
        if(counter_100ms_flag==1)
        {
          counter_100ms_flag=0;
          LED_Work();
          KEY_Main_Handle(&KEY_Handle);  
         
          if(Open_Close_Switch_Handle.Open_Flag==1)
          {    
             if(KEY_Handle.DISPALY_MODE==NORM_MODE)
             {   
               K4G_ON;
               THROTTLE_Handle.Limit_TEMP=127;
             }
             else if(KEY_Handle.DISPALY_MODE==SYNC_MODE)
             {
               K3G_ON;
             }
             else if(KEY_Handle.DISPALY_MODE==DOCK_MODE)
             {
               K2G_ON;
             }
           }
            #if 1
            switch(MS485_Handle.Comm_State)
            {
                case CommunicationState_LCD_Driver1:
                    SendData_Filling(LCD_MOTOR_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Driver2;
                   break;
                
                case CommunicationState_LCD_Driver2:
                    SendData_Filling(LCD_MOTOR_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_GPS1;
                   break;
                
                case CommunicationState_LCD_GPS1:
                    SendData_Filling(LCD_ADAPTER_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_GPS2;
                   break;
                
                case CommunicationState_LCD_GPS2:
                    SendData_Filling(LCD_ADAPTER_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Throttle;
                   break;
               
                 case CommunicationState_LCD_Throttle:
                    SendData_Filling(LCD_THROTTLE_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Battery1;
                   break; 
                 
                 case CommunicationState_LCD_Battery1:
                    SendData_Filling(LCD_BATTERY_MSG,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Battery2;
                   break;
                
                case CommunicationState_LCD_Battery2:
                    SendData_Filling(LCD_BATTERY_MSG1,DATA_LENG_7,LCD_ADDRESS);
                    MS485_Handle.Comm_State=CommunicationState_LCD_Driver1;
                   break;
                
                default:
                   break; 
            } 
            USART3_SEND_DATA();               
            UART3_SendArray(MS485_Handle.TxBuf,12);
            USART3_RECIVE_DATA();
            #endif    
        }  
        IWDG_ReloadCounter(); 
        
        
        /*�͹���ģʽ*/
        __WFI();
    }
}



