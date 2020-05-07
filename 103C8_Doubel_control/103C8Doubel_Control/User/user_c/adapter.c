#include "adapter.h"

GPS_Handle_t  GPS_Handle;
GPS_Handle_t  GPS_Handle1;



void GPS_Init(GPS_Handle_t* handle)
{

    handle->speed=0;
    handle->num_sv=0;
    handle->latitude_H=0;
    
	handle->latitude_L=0;
	handle->longtitude_H=0;
	handle->longtitude_L=0;
    handle->traveled_distance=0;  //�����о���
	handle->traveled_time=0;      //������ʱ��
	handle->remain_distance=0;    //�����о���
	handle->remain_time=0;        //������ʱ��
    
	handle->driction_angle=0;     //��λ��
    handle->adapter_hard_version=0;

	handle->soft_year_month=0;      
	handle->adapter_hard_version=0;  
	handle->hard_year_month=0;
	
	handle->SaveTraveled_distance=0;
	handle->SaveTraveled_time=0;
	handle->SaveTime=0;
	handle->TimeH=0;
	handle->TimeL=0;
	handle->Time=0; 
}

