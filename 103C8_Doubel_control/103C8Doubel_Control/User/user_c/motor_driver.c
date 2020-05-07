#include "motor_driver.h"

MOTOR_Handle_t  MOTOR_Handle;
MOTOR_Handle_t  MOTOR_Handle1;

void MOTOR_Init(MOTOR_Handle_t* handle)
{

    handle->dc_votage=0;
    handle->Get_Adapter_Flag=0;
    handle->Get_Driver_Flag=0;
    handle->hard_version=0;
    handle->hard_year_month=0;
    handle->mos_temp=0;
    handle->MotorCheckNoTempCount=0;
    handle->MotorPowerLimit=0;
    handle->motor_current=0;
    handle->motor_dc_current=0;
    handle->Motor_Status=0;
//    handle->motor_status=0;
    handle->motor_temp=0;
    handle->power=0;
    handle->product_type=0;
    handle->rpm=0;
    handle->soft_version=0;
    handle->sotf_year_month=0;
}



