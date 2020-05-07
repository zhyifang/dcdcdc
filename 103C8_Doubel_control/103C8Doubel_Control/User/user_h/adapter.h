#ifndef _ADAPTER_H
#define _ADAPTER_H

#include "stm32f10x.h"

typedef enum
{
	gps_Searching = 0,
	gps_Connected,
	gps_Disconnected,
}GPS_Status_e;

typedef struct
{

	GPS_Status_e    gps_status;
		
	uint8_t         speed;
    uint8_t         num_sv;
    
	uint16_t        latitude_H;
	uint16_t        latitude_L;
	uint16_t        longtitude_H;
	uint16_t        longtitude_L;
    uint16_t        traveled_distance;  //已运行距离
	uint16_t        traveled_time;      //已运行时间
	uint16_t        remain_distance;    //可运行距离
	uint16_t        remain_time;        //可运行时间
    
	uint16_t        driction_angle;     //方位角
    
	uint8_t         adapter_soft_version;
	uint16_t        soft_year_month;      
	uint8_t         adapter_hard_version;  
	uint16_t        hard_year_month;
	
	uint16_t        SaveTraveled_distance;
	uint16_t        SaveTraveled_time;
	uint32_t        SaveTime;
	uint16_t        TimeH;
	uint16_t        TimeL;
	uint32_t        Time;  //the time is the minute from 1980s to now 


}GPS_Handle_t;

extern  GPS_Handle_t  GPS_Handle;
extern  GPS_Handle_t  GPS_Handle1;
void GPS_Init(GPS_Handle_t* handle);

#endif




