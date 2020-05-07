#ifndef __GPIO_H
#define	__GPIO_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "buzzer.h"
#include "angle_as5600.h"
/*****�궨��******/
/*****KEY��غ궨��******/
#define KEY_GPIO    GPIOA
#define KEY1_GPIO   GPIO_Pin_7
#define KEY2_GPIO   GPIO_Pin_6
#define KEY3_GPIO   GPIO_Pin_5
#define KEY4_GPIO   GPIO_Pin_4

#define KEY1_STATUS          GPIO_ReadInputDataBit(KEY_GPIO,KEY1_GPIO) //����ȡIO��Ϊ0ʱ��Ϊ���£���ȡ��IO��Ϊ1ʱ��Ϊû�а���
#define KEY2_STATUS          GPIO_ReadInputDataBit(KEY_GPIO,KEY2_GPIO)
#define KEY3_STATUS          GPIO_ReadInputDataBit(KEY_GPIO,KEY3_GPIO)
#define KEY4_STATUS          GPIO_ReadInputDataBit(KEY_GPIO,KEY4_GPIO)
/*****LED��غ궨��******/
//#define K2_R_GPIO_PORT   GPIOB   //usart1��RE����DE����
//#define K1_R_GPIO_PORT   GPIOA   //Can����
#define HALL_GPIO_PORT   GPIOB   //Hall ����
#define K1_G_GPIO_PORT   GPIOB
#define K2_G_GPIO_PORT   GPIOB
#define K3_G_GPIO_PORT   GPIOC
#define K4_G_GPIO_PORT   GPIOC

#define HALL_GPIO_Pin  GPIO_Pin_3
#define K1_G_GPIO_Pin  GPIO_Pin_14
#define K2_G_GPIO_Pin  GPIO_Pin_15
#define K3_G_GPIO_Pin  GPIO_Pin_13
#define K4_G_GPIO_Pin  GPIO_Pin_14


/*****LED���ƿ�0�������궨��******/ //Ϩ��LED�� 0

#define K1G_OFF      GPIO_ResetBits(K1_G_GPIO_PORT , K1_G_GPIO_Pin)
#define K2G_OFF      GPIO_ResetBits(K2_G_GPIO_PORT , K2_G_GPIO_Pin)
#define K3G_OFF      GPIO_ResetBits(K3_G_GPIO_PORT , K3_G_GPIO_Pin) 
#define K4G_OFF      GPIO_ResetBits(K4_G_GPIO_PORT , K4_G_GPIO_Pin)

/*****LED���ƿ�1�������궨��******/ //����LED��1 

#define K1G_ON     GPIO_SetBits(K1_G_GPIO_PORT , K1_G_GPIO_Pin)  
#define K2G_ON     GPIO_SetBits(K2_G_GPIO_PORT , K2_G_GPIO_Pin) 
#define K3G_ON     GPIO_SetBits(K3_G_GPIO_PORT , K3_G_GPIO_Pin)  
#define K4G_ON     GPIO_SetBits(K4_G_GPIO_PORT , K4_G_GPIO_Pin) 



#define LED_TIME   5
typedef enum
{
	KEY_State_Released,     //�ͷ����
	KEY_State_Releasing,    //�ͷ���
	KEY_State_Pressed,      //��ѹ���
	KEY_State_Pressing      //��ѹ��
}KEY_State_e;

typedef enum
{
	KEY_Action_Push,       //������ѹ
	KEY_Action_Release     //�����ͷ�
}KEY_Action_e;

typedef enum
{
	KEY_1_Button    =  1,
	KEY_2_Button    =  2,
	KEY_3_Button    =  3,
	KEY_4_Button    =  4 
}KEY_Name_e;
typedef enum
{
	Touch_NO,            //����û�а�ѹ
	Touch_Short,         //�����̰�
	Touch_Mid,           //�����а�
	Touch_Long           //��������
}KEY_Touch_Type_e;
typedef struct
{
	uint8_t              Lock;
	KEY_Action_e		 Action;
	KEY_State_e	  	     State;
	KEY_Touch_Type_e     Touch_Type;
	uint16_t			 Press_Count;
}KEY_t;
typedef enum
{
	NORM_MODE,            //����û�а�ѹ
	SYNC_MODE,         //�����̰�
	DOCK_MODE,           //�����а�
}KEY_MODE_e;


typedef struct
{
	KEY_t    	Open_Close_Key;
	KEY_t		SYNC_Control_Key;
	KEY_t		DOCK_Key;
	KEY_t		NORM_Key;
    KEY_MODE_e  DISPALY_MODE;
    
}KEY_Handle_t;
typedef struct
{
	uint8_t LED_One_Trigger_flag;
    uint8_t LED_Key2_Trigger_flag;
    uint8_t LED_Key3_Trigger_flag;

	uint8_t LED_Key2Key3_Trigger_flag;
    uint8_t LED_Key1Key4Key3_Trigger_flag;
    uint8_t LED_Key1Key4_Trigger_flag;
    uint8_t LED_Key4_Trigger_flag; 
    uint8_t LED_Key2Key4_Trigger_flag;
    uint8_t LED_Key3Key4_Trigger_flag;
    uint8_t LED_Three_Trigger_flag;
    
    
    
	uint8_t LED_All_Trigger_flag;
 	uint8_t LED_Key1_flag;   
    uint8_t LED_Key2_flag;
    uint8_t LED_Key4_flag;
    uint8_t LED_Key2Key4_flag; 
    uint8_t LED_Key2Key1_flag;
    uint8_t LED_Key3Key4_flag;   
    uint8_t LED_Direction_flag;
    
	uint8_t LED_One_cnt;
	uint8_t LED_Two_cnt;
	uint8_t LED_Three_cnt;
    uint8_t LED_All_cnt;
    uint8_t LED_times;
}LED_Handle_t;



typedef enum 
{
	Open_Flag,
	Open_State,
    Close_Flag,
	Close_State,
}Open_Close_State_e;


typedef struct 
{
	Open_Close_State_e  SysPowerState;
	uint8_t             SysPowerCheckFlag; 
    
    uint8_t	            Open_Flag;
	uint8_t             Open_State;
    uint8_t             Close_Flag;
	uint8_t             Close_State;

	uint16_t            AutoPowerOffCount;    //�Զ��ػ�����

}Open_Close_Switch_Handle_t;

extern Open_Close_Switch_Handle_t  Open_Close_Switch_Handle;



extern LED_Handle_t LED_Handle;
extern KEY_Handle_t KEY_Handle;

void LED_GPIO_Init(void);
void Key_GPIO_Init(void);
void Open_Close_Switch_Init(void);
void Key_GPIO_Handle(void);

void KEY_Get_Touch_Type(KEY_t* p);
void KEY_Main_Handle(KEY_Handle_t* p);
void KEY_CountClear(KEY_t* p);

void LED_All_ON(void);
void LED_All_OFF(void);
void LED_All_G_OFF(void);

void LED_All_G_ON(void);

void LED_Three_G_ON(void);
void LED_Three_G_OFF(void);
void LED_Two_G_ON(void);
void LED_Two_G_OFF(void);
void LED_One_G_ON(void);
void LED_One_G_OFF(void);
void LED_eerhT_G_ON(void);
void LED_eerhT_G_OFF(void);
void LED_owT_G_ON(void);
void LED_owT_G_OFF(void);
void LED_enO_G_ON(void);
void LED_enO_G_OFF(void);

void LED_Work(void);

void All_Function_Handle(KEY_Handle_t* p);
#endif /* __GPIO_H */


