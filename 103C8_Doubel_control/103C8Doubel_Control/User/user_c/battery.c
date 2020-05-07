#include "battery.h"

BATTERY_Handle_t BATTERY_Handle;



void BATTERY_Init(BATTERY_Handle_t* handle)
{
    
    handle->Y_Battery_Data1=0;           //
	handle->Y_Battery_Voltage1=0;    //电池电压
	handle->Y_Battery_Soc1=0  ;      //电池的soc剩余电量
    
	handle->Y_Battery_Power1=0;      //电池功率      
	handle->Y_Battery_Current1=0;    //电池电流
    
    handle-> N_Battery_Voltage1=0;    //电池电压
	handle->N_Battery_Soc1 =0 ;      //电池的soc剩余电量
    
	handle->N_Battery_Power1=0;      //电池功率      
	handle->N_Battery_Current1=0;    //电池电流
    
    handle->Y_Battery_Data2=0;           //
    handle->Y_Battery_Voltage2=0;    //电池电压
	handle->Y_Battery_Soc2 =0 ;      //电池的soc剩余电量
    
	handle->Y_Battery_Power2=0;      //电池功率      
	handle->Y_Battery_Current2=0;    //电池电流
    
    handle->N_Battery_Voltage2=0;    //电池电压
	handle->N_Battery_Soc2 =0 ;      //电池的soc剩余电量
    
	handle-> N_Battery_Power2=0;      //电池功率      
	handle->N_Battery_Current2=0;    //电池电流
    
    handle->HighestTemp=0;           //电池温度
	handle->LowestTemp=0;            //
    handle->Battery_Comm_count=0;      //电池的通信计数
	handle->Battery_Protocol_Flag=0;   //电池协议的标志位
	handle->useBatRealCap=0;           //使用电池的标称容量
    
    
}










