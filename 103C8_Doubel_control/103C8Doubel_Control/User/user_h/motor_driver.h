#ifndef _MOTOR_DRIVER_H
#define _MOTOR_DRIVER_H

#include "stm32f10x.h"

typedef enum 
{  
    motor_nomal,
	motor_comm_fail,
	motor_over_vol,
	motor_under_vol,
	motor_over_cur,
	motor_idling,
	motor_stall,
	motor_ctrl_fail,
	Motor_CalErr,
	Motor_CalErred,
	motor_over_temp,
    motor_temp_warn,
}MOTOR_Status_e;
typedef  enum
{
    temp_nomal,
    temp_over,	
	temp_damage
}temp_state_e;
typedef  struct
{
    uint16_t         value;
	temp_state_e     temp_state;
	
}temp_t;

typedef  struct
{ 
	temp_t  				tMot;

	uint16_t                MotorPowerLimit;

    MOTOR_Status_e          motor_status;	
    
    uint8_t                 Motor_Status;	
	uint16_t 				power;	 
    uint16_t 				dc_votage;	
    uint16_t  				rpm;						
    uint16_t  				motor_current;
    uint16_t  				motor_dc_current;
    int16_t  				motor_temp;
    int16_t  				mos_temp;
    uint8_t                 product_type;
    uint8_t                 soft_version;
    uint16_t                sotf_year_month;
    uint8_t                 hard_version;
    uint16_t                hard_year_month;
    
    
	uint8_t                 Get_Driver_Flag;
 	uint8_t                 Get_Adapter_Flag;   
	uint16_t                MotorCheckNoTempCount;	
}MOTOR_Handle_t;

extern  MOTOR_Handle_t  MOTOR_Handle;
extern  MOTOR_Handle_t  MOTOR_Handle1;


void MOTOR_Init(MOTOR_Handle_t* handle);


#endif




