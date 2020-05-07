#ifndef __KEY_CONTROL_MODE_H
#define	__KEY_CONTROL_MODE_H

#include "stm32f10x.h"

//枚举状态流
typedef enum
{
	ControlMode_State_0 = 0,
	ControlMode_State_R, //1  需要重新设置油门状态
	ControlMode_State_L, //2  完成设置油门状态
    Limit_State_Mid,
    Limit_State_LOW,
    
}KeyControlMode_State_e;

typedef struct
{
    //校正标志位，校正错误
	uint8_t      R_Calibration_Flag;
    uint8_t      R_CalError_Flag;
    uint8_t      R_CalSuc_Flag;
    uint8_t      R_SetZero_Flag;
    uint8_t      R_L_Change_Flag;
    KeyControlMode_State_e State;

											
}KeyControlMode_Handle_t;

extern KeyControlMode_Handle_t SYNC_ContorlMode;
extern KeyControlMode_Handle_t DOCK_ContorlMode;




#endif /* __KEY_CONTROL_MODE_H */

