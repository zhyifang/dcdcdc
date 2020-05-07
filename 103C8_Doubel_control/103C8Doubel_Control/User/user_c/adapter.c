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
    handle->traveled_distance=0;  //已运行距离
	handle->traveled_time=0;      //已运行时间
	handle->remain_distance=0;    //可运行距离
	handle->remain_time=0;        //可运行时间
    
	handle->driction_angle=0;     //方位角
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

