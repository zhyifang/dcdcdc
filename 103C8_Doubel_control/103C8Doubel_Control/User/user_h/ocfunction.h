#ifndef __OCFUNCTION_H
#define	__OCFUNCTION_H

#include "stm32f10x.h"

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
	uint16_t            AutoPowerOffCount;    //自动关机计数

}Open_Close_Switch_Handle_t;

extern  Open_Close_Switch_Handle_t  Open_Close_Switch_Handle;



#endif /* __OCFUNCTION_H */






