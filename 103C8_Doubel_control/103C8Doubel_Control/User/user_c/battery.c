#include "battery.h"

BATTERY_Handle_t BATTERY_Handle;



void BATTERY_Init(BATTERY_Handle_t* handle)
{
    
    handle->Y_Battery_Data1=0;           //
	handle->Y_Battery_Voltage1=0;    //��ص�ѹ
	handle->Y_Battery_Soc1=0  ;      //��ص�socʣ�����
    
	handle->Y_Battery_Power1=0;      //��ع���      
	handle->Y_Battery_Current1=0;    //��ص���
    
    handle-> N_Battery_Voltage1=0;    //��ص�ѹ
	handle->N_Battery_Soc1 =0 ;      //��ص�socʣ�����
    
	handle->N_Battery_Power1=0;      //��ع���      
	handle->N_Battery_Current1=0;    //��ص���
    
    handle->Y_Battery_Data2=0;           //
    handle->Y_Battery_Voltage2=0;    //��ص�ѹ
	handle->Y_Battery_Soc2 =0 ;      //��ص�socʣ�����
    
	handle->Y_Battery_Power2=0;      //��ع���      
	handle->Y_Battery_Current2=0;    //��ص���
    
    handle->N_Battery_Voltage2=0;    //��ص�ѹ
	handle->N_Battery_Soc2 =0 ;      //��ص�socʣ�����
    
	handle-> N_Battery_Power2=0;      //��ع���      
	handle->N_Battery_Current2=0;    //��ص���
    
    handle->HighestTemp=0;           //����¶�
	handle->LowestTemp=0;            //
    handle->Battery_Comm_count=0;      //��ص�ͨ�ż���
	handle->Battery_Protocol_Flag=0;   //���Э��ı�־λ
	handle->useBatRealCap=0;           //ʹ�õ�صı������
    
    
}










