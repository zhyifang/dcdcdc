#ifndef _BATTERY_H
#define _BATTERY_H

#include "stm32f10x.h"

typedef enum
{
	Battery_State_Start = 0,
	Battery_State_YD,
	Battery_State_N,
	Battery_State_NO_Bat,
	Battery_State_CommFailed,
	Battery_State_Error,
}Battery_State_e;
typedef enum
{
	Battery_Status_NonEpBatt = 0,
	Battery_Status_CommFailed,
	Battery_Status_FunctionalError,
	Battery_Status_On,
}Battery_Status_e;

typedef enum 
{
	Battery_TypeOwnBatt,
	BatteryType_LiOn,
	BatteryType_Pb,
	Battery_TypeLiFe,
	BatteryType_Max,
	Battery_TypeA,
	Battery_TypeB,
	Battery_TypeC,
	Battery_TypeD,
	Battery_TypeE
}BatteryType_e;
typedef enum 
{
	bat_soc_more,
	bat_soc_less
}Bat_SocState_e;
typedef   struct
{

	Bat_SocState_e    Bat_SocState;
	Battery_Status_e  Status;
	Battery_State_e   State1;
    Battery_State_e   State2;
    BatteryType_e     Battery_Type;          //ʹ�õ�ص�����
    
    uint8_t           Y_Battery_Openflag1;  
    uint8_t           Y_Battery_Data1;       //
	uint16_t          Y_Battery_Voltage1;    //��ص�ѹ
	uint16_t          Y_Battery_Soc1  ;      //��ص�socʣ�����
    
	uint16_t          Y_Battery_Power1;      //��ع���      
	uint16_t          Y_Battery_Current1;    //��ص���
    
    uint16_t          N_Battery_Voltage1;    //��ص�ѹ
	uint16_t          N_Battery_Soc1  ;      //��ص�socʣ�����
    
	uint16_t          N_Battery_Power1;      //��ع���      
	uint16_t          N_Battery_Current1;    //��ص���
    
    uint8_t           Y_Battery_Openflag2; 
    uint8_t           Y_Battery_Data2;       //
    uint16_t          Y_Battery_Voltage2;    //��ص�ѹ
	uint16_t          Y_Battery_Soc2  ;      //��ص�socʣ�����
    
	uint16_t          Y_Battery_Power2;      //��ع���      
	uint16_t          Y_Battery_Current2;    //��ص���
    
    uint16_t          N_Battery_Voltage2;    //��ص�ѹ
	uint16_t          N_Battery_Soc2  ;      //��ص�socʣ�����
    
	uint16_t          N_Battery_Power2;      //��ع���      
	uint16_t          N_Battery_Current2;    //��ص���
    
    uint8_t           HighestTemp;           //����¶�
	uint8_t           LowestTemp;            //
    uint8_t           Battery_Comm_count;      //��ص�ͨ�ż���
	uint16_t          Battery_Protocol_Flag;   //���Э��ı�־λ
	uint16_t          useBatRealCap;           //ʹ�õ�صı������
	
}BATTERY_Handle_t;

extern  BATTERY_Handle_t BATTERY_Handle;


void BATTERY_Init(BATTERY_Handle_t* handle);


#endif






