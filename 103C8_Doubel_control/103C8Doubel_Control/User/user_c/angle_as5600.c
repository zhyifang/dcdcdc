/***************************************
 * 文件名  ：angle_as5600.c
 * 描述    ：配置角度传感器       
 * 实验平台：STM32 基于STM32F103C8T6
 * 硬件连接：
 * 库版本  ：ST3.0.0  
 
*****************************************/

#include "angle_as5600.h"

THROTTLE_Handle_t THROTTLE_Handle;
//右端的油门杆
u8 R_AS5600_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		
    R_IIC_Start();  
	R_IIC_Send_Byte(0X6C);     				//发送AS5600从设备地址  写
	R_IIC_Wait_Ack();                       
    R_IIC_Send_Byte(ReadAddr);				//发送寄存器地址
	R_IIC_Wait_Ack();	                    
	R_IIC_Start();  	 	                 
	R_IIC_Send_Byte(0X6D);                  //发送AS5600从设备地址  读			   
	R_IIC_Wait_Ack();	                    
    temp=R_IIC_Read_Byte(0);		        
    R_IIC_Stop();                           //产生一个停止条件	    
	return temp;                            
}

void R_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    R_IIC_Start();   
    R_IIC_Send_Byte(0X6C);            //发送AS5600设备地址写 
	R_IIC_Wait_Ack();	   
    R_IIC_Send_Byte(WriteAddr);       //发送寄存器地址
	R_IIC_Wait_Ack(); 	 										  		   
	R_IIC_Send_Byte(DataToWrite);     //发送字节							   
	R_IIC_Wait_Ack();  		    	   
    R_IIC_Stop();				      //产生一个停止条件 
	delay_ms(10);	 				  //等待AS5600擦写数据
}

u8 R_AS5600_Read()
{
	u8 temp0=0;
	R_IIC_Start();
	R_IIC_Send_Byte(0X6D);
	R_IIC_Wait_Ack();
	temp0=R_IIC_Read_Byte(0);
	R_IIC_NAck();
	R_IIC_Stop();
	return temp0;
}

uint8_t R_AS5600_GetStatus(void)
{
	return R_AS5600_ReadOneByte(STATUS) & 0x38;	
}

uint16_t R_AS5600_GetAngle()
{
	return (int)((float)(R_AS5600_ReadOneByte(ANGLE_L) + (R_AS5600_ReadOneByte(ANGLE_H) << 8))/4096*360);	
}

uint16_t R_AS5600_GetRawAngle()
{
	uint16_t AngleVal=R_AS5600_ReadOneByte(RAWANG_L) + (R_AS5600_ReadOneByte(RAWANG_H) << 8);
	return AngleVal;	
}

void R_AS5600_SetHystheresis(uint8_t Hyst)
{
	uint8_t TmpConfHigh=R_AS5600_ReadOneByte(CONF_H);
	TmpConfHigh&=0xf3;
	TmpConfHigh |= (HYST_MASK & Hyst);	
	R_AS5600_WriteOneByte(CONF_H,TmpConfHigh);
}

void R_AS5600_SetOutputStage(uint8_t OutStage)
{
	uint8_t TmpConfHigh=R_AS5600_ReadOneByte(CONF_L);
	TmpConfHigh&=0xcf;
	TmpConfHigh |= (OUT_STG_MASK & OutStage);
	R_AS5600_WriteOneByte(CONF_L,TmpConfHigh);
}

void R_AS5600_SetPWMFreq(uint8_t Freq)
{
	uint8_t TmpConfHigh=R_AS5600_ReadOneByte(CONF_H);
	TmpConfHigh&=0x3f;
	TmpConfHigh |= (PWMF_MASK & Freq);
	R_AS5600_WriteOneByte(CONF_H,TmpConfHigh);
}

void R_AS5600_SetPMMode(uint8_t mode)
{
	uint8_t TmpConfL=R_AS5600_ReadOneByte(CONF_L);
	TmpConfL&=0xfc;
	TmpConfL |= (PM_MASK & mode);
	R_AS5600_WriteOneByte(CONF_L,TmpConfL);
}

void R_AS5600_EnableWd(void)
{
	uint8_t TmpConfH=R_AS5600_ReadOneByte(CONF_H);
	TmpConfH &= 0xDF;
	TmpConfH |= 0X20;
	R_AS5600_WriteOneByte(CONF_H,TmpConfH);
}

void R_AS5600_DisableWd(void)
{
	uint8_t TmpConfH=R_AS5600_ReadOneByte(CONF_H);
	TmpConfH&=0xDF;
	TmpConfH |= 0X00;
	R_AS5600_WriteOneByte(CONF_H,TmpConfH);
}


uint8_t R_AS5600_GetAGC(void)
{
	return R_AS5600_ReadOneByte(AGC);	
}

uint16_t R_AS5600_ReadConf(void)
{
	uint16_t conf_tmp = 0;
	
	conf_tmp=R_AS5600_ReadOneByte(CONF_H);
	conf_tmp <<=8;
	conf_tmp=R_AS5600_ReadOneByte(CONF_L);
	return conf_tmp;
}
uint16_t R_AS5600_ReadMagitue(void)
{
	uint16_t conf_tmp = 0;
	conf_tmp=R_AS5600_ReadOneByte(MAGN_H);
	conf_tmp <<=8;
	conf_tmp=R_AS5600_ReadOneByte(MAGN_L);
	return conf_tmp;
}


//左端的油门杆
u8 L_AS5600_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		
    L_IIC_Start();  
	L_IIC_Send_Byte(0X6C);     				 //发送AS5600从设备地址写
	L_IIC_Wait_Ack(); 
    L_IIC_Send_Byte(ReadAddr);				 //发送寄存器地址
	L_IIC_Wait_Ack();	    
	L_IIC_Start();  	 	   
	L_IIC_Send_Byte(0X6D);                   //发送AS5600从设备地址读			   
	L_IIC_Wait_Ack();	 
    temp=L_IIC_Read_Byte(0);		   
    L_IIC_Stop();                            //产生一个停止条件	    
	return temp;
}

void L_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    L_IIC_Start();   
    L_IIC_Send_Byte(0X6C);            //发送AS5600设备地址写 
	L_IIC_Wait_Ack();	   
    L_IIC_Send_Byte(WriteAddr);       //发送寄存器地址
	L_IIC_Wait_Ack(); 	 										  		   
	L_IIC_Send_Byte(DataToWrite);     //发送字节							   
	L_IIC_Wait_Ack();  		    	   
    L_IIC_Stop();						//产生一个停止条件 
	delay_ms(10);	 				//等待AS5600擦写数据
}

u8 L_AS5600_Read()
{
	u8 temp0=0;
	L_IIC_Start();
	L_IIC_Send_Byte(0X6D);
	L_IIC_Wait_Ack();
	temp0=L_IIC_Read_Byte(0);
	L_IIC_NAck();
	L_IIC_Stop();
	return temp0;
}

uint8_t L_AS5600_GetStatus(void)
{
	return L_AS5600_ReadOneByte(STATUS) & 0x38;	
}

uint16_t L_AS5600_GetAngle()
{
	return (int)((float)(L_AS5600_ReadOneByte(ANGLE_L) + (L_AS5600_ReadOneByte(ANGLE_H) << 8))/4096*360);	
}


uint16_t L_AS5600_GetRawAngle()
{
	uint16_t AngleVal=L_AS5600_ReadOneByte(RAWANG_L) + (L_AS5600_ReadOneByte(RAWANG_H) << 8);
	return AngleVal;	
}

void L_AS5600_SetHystheresis(uint8_t Hyst)
{
	uint8_t TmpConfHigh=L_AS5600_ReadOneByte(CONF_H);
	TmpConfHigh&=0xf3;
	TmpConfHigh |= (HYST_MASK & Hyst);	
	L_AS5600_WriteOneByte(CONF_H,TmpConfHigh);
}

void L_AS5600_SetOutputStage(uint8_t OutStage)
{
	uint8_t TmpConfHigh=L_AS5600_ReadOneByte(CONF_L);
	TmpConfHigh&=0xcf;
	TmpConfHigh |= (OUT_STG_MASK & OutStage);
	L_AS5600_WriteOneByte(CONF_L,TmpConfHigh);
}
void L_AS5600_SetPWMFreq(uint8_t Freq)
{
	uint8_t TmpConfHigh=L_AS5600_ReadOneByte(CONF_H);
	TmpConfHigh&=0x3f;
	TmpConfHigh |= (PWMF_MASK & Freq);
	L_AS5600_WriteOneByte(CONF_H,TmpConfHigh);
}
void L_AS5600_SetPMMode(uint8_t mode)
{
	uint8_t TmpConfL=L_AS5600_ReadOneByte(CONF_L);
	TmpConfL&=0xfc;
	TmpConfL |= (PM_MASK & mode);
	L_AS5600_WriteOneByte(CONF_L,TmpConfL);
}

void L_AS5600_EnableWd(void)
{
	uint8_t TmpConfH=L_AS5600_ReadOneByte(CONF_H);
	TmpConfH &= 0xDF;
	TmpConfH |= 0X20;
	L_AS5600_WriteOneByte(CONF_H,TmpConfH);
}

void L_AS5600_DisableWd(void)
{
	uint8_t TmpConfH=L_AS5600_ReadOneByte(CONF_H);
	TmpConfH&=0xDF;
	TmpConfH |= 0X00;
	L_AS5600_WriteOneByte(CONF_H,TmpConfH);
}
uint8_t L_AS5600_GetAGC(void)
{
	return L_AS5600_ReadOneByte(AGC);	
}

uint16_t L_AS5600_ReadConf(void)
{
	uint16_t conf_tmp = 0;
	
	conf_tmp=L_AS5600_ReadOneByte(CONF_H);
	conf_tmp <<=8;
	conf_tmp=L_AS5600_ReadOneByte(CONF_L);

	return conf_tmp;
}
uint16_t L_AS5600_ReadMagitue(void)
{
	uint16_t conf_tmp = 0;
	
	conf_tmp=L_AS5600_ReadOneByte(MAGN_H);
	conf_tmp <<=8;
	conf_tmp=L_AS5600_ReadOneByte(MAGN_L);

	return conf_tmp;
}



















//油门校正初始化
void THROTTLE_Init(THROTTLE_Handle_t* handle)
{	
    u8 count_buf=0;
    handle->Limit_TEMP = 127;
	handle->R_Calibration_Flag=0;
    handle->R_CalError_Flag=0;
    handle->R_CalSuc_Flag=0;
    handle->R_SetZero_Flag=0;
    
    handle->L_Calibration_Flag=0;
    handle->L_CalError_Flag=0;
    handle->L_CalSuc_Flag=0;
    handle->L_SetZero_Flag=0;
    
	handle->Temp_Val=0;
    handle->Saft_Key=0; 
	handle->ForwardPercentMax=0;             //The Forward max power percent 0-127
	handle-> BackwardPercentMax=0;            //The Backward max power percent 0-127

    handle->R_ForwardPercent=0;              //The Forward max power percent 0-127
	handle->R_BackwardPercent=0;             //The Backward max power percent 0-127
     
    handle->L_ForwardPercent=0;              //The Forward max power percent 0-127
	handle->L_BackwardPercent=0;             //The Backward max power percent 0-127
    	
    handle->R_PercentTemp=0;                 //485发送的Temp
  	handle->L_PercentTemp=0;                 //485发送的
	handle->R_Percent=0;					     //percent of the full throttle from 0~127 without direction
	handle->L_Percent=0;					     //percent of the full throttle from 0~127 without direction
    
    handle->R_Direction=0;					//direction 1: forward 0: backward
    handle->L_Direction=0;					//direction 1: forward 0: backward  
    
    handle->R_Cal_ForwardAngle=0;
    handle->R_ZeroAngle=0;
	handle->R_Cal_BackwardAngle=0;
	
    handle->L_Cal_ForwardAngle=0;
    handle->L_ZeroAngle=0;
	handle->L_Cal_BackwardAngle=0;
    
	handle->R_angle=0;		            //  右边油门杆角度
    handle->L_angle=0;                   //  左边油门杆角度

    handle->R_Pre_angle=0;		        //  右边油门杆角度
    handle->L_Pre_angle=0; 
    
    for(count_buf=0;count_buf<8;count_buf++)
    {   
      handle->CAN_TxBuf[count_buf]=0;
    }
}

void THROTTLE_Calc(THROTTLE_Handle_t* p)
{	
    //使用校正获取的数据，进行判定是否合理校正。
    if(p->R_ZeroAngle>=60&&p->R_ZeroAngle<=299)
    {
        //按照增序列   下按照减系列来角度增加和减少
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))
        {
            if((((p->R_Cal_ForwardAngle-p->R_ZeroAngle)>=(FOEWARD_60-DT))&&((p->R_Cal_ForwardAngle-p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_ZeroAngle-p->R_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->R_ZeroAngle-p->R_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->R_CalError_Flag=1; //右边错误
                return;
            }
        }
        else if(((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)))
        {

            if((((p->R_ZeroAngle-p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle-p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                && (((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
              p->R_CalSuc_Flag=1;  //右边校正成功
              p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {
                p->R_CalError_Flag=1; //右边错误
                return;
            }
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
        
    }
    else if(p->R_ZeroAngle>=BACKWARD_40&&p->R_ZeroAngle<FOEWARD_60)
    {     
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
            if((((p->R_Cal_ForwardAngle-p->R_ZeroAngle)>=(FOEWARD_60-DT))&&((p->R_Cal_ForwardAngle-p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_ZeroAngle-p->R_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->R_ZeroAngle-p->R_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->R_CalError_Flag=1; //右边错误
                return;
            }    
        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle >p->R_Cal_BackwardAngle))
        {
            if((((360-(p->R_Cal_ForwardAngle)+p->R_ZeroAngle)>=(FOEWARD_60-DT))&&((360-(p->R_Cal_ForwardAngle)+p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->R_Cal_BackwardAngle - p->R_ZeroAngle<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle - p->R_ZeroAngle>=(BACKWARD_40-DT)))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {
                 p->R_CalError_Flag=1; //右边错误
                return;
            
            }
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
    }
    else if(p->R_ZeroAngle>299&&p->R_ZeroAngle<=(359-BACKWARD_40))
    {
         //qian 60
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
            if((((360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle))>=(FOEWARD_60-DT))&&((360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle))<=(FOEWARD_60+DT)))
                 &&((p->R_ZeroAngle - p->R_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->R_ZeroAngle-p->R_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->R_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle))
        {

             if((((p->R_ZeroAngle - p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->R_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
        
    }
    else if(p->R_ZeroAngle<360&&p->R_ZeroAngle>(359-BACKWARD_40))
    {
        //qian 60
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle>p->R_Cal_ForwardAngle))   
        {
            if((((360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle))>=(FOEWARD_60-DT))&&((360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle))<=(FOEWARD_60+DT)))
                 &&((p->R_ZeroAngle - p->R_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->R_ZeroAngle-p->R_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->R_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle <p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_Cal_ForwardAngle))
        {

             if((((p->R_ZeroAngle - p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle+360-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle+360-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->R_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
    }
    else
    {
        //qian 0-40
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_Cal_ForwardAngle))   
        {
            if(((((p->R_Cal_ForwardAngle)-p->R_ZeroAngle)>=(FOEWARD_60-DT))&&(((p->R_Cal_ForwardAngle)-p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->R_ZeroAngle +360- p->R_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->R_ZeroAngle+360-p->R_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->R_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle >p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle>p->R_Cal_BackwardAngle))
        {

             if((((p->R_ZeroAngle +360- p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle+360 - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //右边校正成功
                p->R_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->R_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
    }

    //左边使用校正获取的数据，进行判定是否合理校正。
    //使用校正获取的数据，进行判定是否合理校正。
    if(p->L_ZeroAngle>=60&&p->L_ZeroAngle<=299)
    {
        //按照增序列   下按照减系列来角度增加和减少
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))
        {
            if((((p->L_Cal_ForwardAngle-p->L_ZeroAngle)>=(FOEWARD_60-DT))&&((p->L_Cal_ForwardAngle-p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_ZeroAngle-p->L_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->L_ZeroAngle-p->L_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->L_CalError_Flag=1; //右边错误
                return;
            }
        }
        else if(((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)))
        {

            if((((p->L_ZeroAngle-p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle-p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                && (((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
              p->L_CalSuc_Flag=1;  //右边校正成功
              p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {
                p->L_CalError_Flag=1; //右边错误
                return;
            }
        }
        else
        { 
            p->L_CalError_Flag=1;
            return;
        }
        
    }
    else if(p->L_ZeroAngle>=BACKWARD_40&&p->L_ZeroAngle<FOEWARD_60)
    {     
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if((((p->L_Cal_ForwardAngle-p->L_ZeroAngle)>=(FOEWARD_60-DT))&&((p->L_Cal_ForwardAngle-p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_ZeroAngle-p->L_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->L_ZeroAngle-p->L_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->L_CalError_Flag=1; //右边错误
                return;
            }    
        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle >p->L_Cal_BackwardAngle))
        {
            if((((360-(p->L_Cal_ForwardAngle)+p->L_ZeroAngle)>=(FOEWARD_60-DT))&&((360-(p->L_Cal_ForwardAngle)+p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->L_Cal_BackwardAngle - p->L_ZeroAngle<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle - p->L_ZeroAngle>=(BACKWARD_40-DT)))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {
                 p->L_CalError_Flag=1; //右边错误
                return;
            
            }
        }
        else
        { 
            p->L_CalError_Flag=1;
            return;
        }
    }
    else if(p->L_ZeroAngle>299&&p->L_ZeroAngle<=(359-BACKWARD_40))
    {
         //qian 60
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if((((360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle))>=(FOEWARD_60-DT))&&((360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle))<=(FOEWARD_60+DT)))
                 &&((p->L_ZeroAngle - p->L_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->L_ZeroAngle-p->L_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->L_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle))
        {

             if((((p->L_ZeroAngle - p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->L_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->L_CalError_Flag=1;
            return;
        }
        
    }
    else if(p->L_ZeroAngle<360&&p->L_ZeroAngle>(359-BACKWARD_40))
    {
        //qian 60
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle>p->L_Cal_ForwardAngle))   
        {
            if((((360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle))>=(FOEWARD_60-DT))&&((360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle))<=(FOEWARD_60+DT)))
                 &&((p->L_ZeroAngle - p->L_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->L_ZeroAngle-p->L_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->L_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle <p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_Cal_ForwardAngle))
        {

             if((((p->L_ZeroAngle - p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle+360-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle+360-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->L_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->L_CalError_Flag=1;
            return;
        }
    }
    else
    {
        //qian 0-40
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_Cal_ForwardAngle))   
        {
            if(((((p->L_Cal_ForwardAngle)-p->L_ZeroAngle)>=(FOEWARD_60-DT))&&(((p->L_Cal_ForwardAngle)-p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->L_ZeroAngle +360- p->L_Cal_BackwardAngle<=(BACKWARD_40+DT))&&(( p->L_ZeroAngle+360-p->L_Cal_BackwardAngle>=(BACKWARD_40-DT)))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零

            }
            else
            {
                 p->L_CalError_Flag=1; //右边错误
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle >p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle>p->L_Cal_BackwardAngle))
        {

             if((((p->L_ZeroAngle +360- p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle+360 - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //右边校正成功
                p->L_CalError_Flag=0; //右边错误清零
            }
            else
            {   
                p->L_CalError_Flag=1; //右边错误
                return;
            }   
        }
        else
        { 
            p->L_CalError_Flag=1;
            return;
        }
    }
}

void Throttle_Value_Conversion(THROTTLE_Handle_t* p)
{
    
    //使用校正获取的数据，进行判定是否合理校正。
    if(p->R_ZeroAngle>=60&&p->R_ZeroAngle<=299)
    {
        //按照增序列   下按照角度增加和减少
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))
        {
            if(p->R_ZeroAngle>=299-10&&(p->R_ZeroAngle<=299))
            {
               if(p->R_angle<10)
               {
                   p->R_angle=359;
               }
            
            } 
            if(p->R_angle>=p->R_ZeroAngle&&(p->R_angle<=(p->R_ZeroAngle+FOEWARD_60+DT)))
            {  
                p->R_Direction=1;    //前推
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);
            }
            else
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_BackwardAngle);
            }
            
        }//减序列
        else if(((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)))
        {
            
            if(p->R_ZeroAngle>=60&&(p->R_ZeroAngle<=60+10))
            {
               if(p->R_angle>349)
               {
                   p->R_angle=0;
               }
            
            }
            
            if(p->R_angle<=p->R_ZeroAngle)
            {  
                p->R_Direction=1;    //前推
                
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle)/(p->R_Cal_BackwardAngle - p->R_ZeroAngle);
            }
        }
    }
    else if(p->R_ZeroAngle>=BACKWARD_40&&p->R_ZeroAngle<FOEWARD_60)
    {     
        //增序列  下减序列
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
            if(p->R_angle>=p->R_ZeroAngle)
            {  
                p->R_Direction=1;    //前推
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);
            }
            else
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_BackwardAngle);  //正确
            }
                
        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle >p->R_Cal_BackwardAngle))
        {
 
            if((p->R_angle >=359-(FOEWARD_60-BACKWARD_40))||(p->R_angle <= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //前推
                
                
                if((p->R_angle <= p->R_ZeroAngle))
                {
                    p->R_Percent =127L*(p->R_ZeroAngle - p->R_angle )/(360 - p->R_Cal_ForwardAngle + p->R_ZeroAngle);
                }
                else
                {
                    p->R_Percent =127L*(360 - p->R_angle + p->R_ZeroAngle)/(360 - p->R_Cal_ForwardAngle + p->R_ZeroAngle);

                }

            }
            else 
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle-p->R_ZeroAngle);
            }
           
        }

    }
    else if(p->R_ZeroAngle>299&&p->R_ZeroAngle<=(359-BACKWARD_40))
    {
         //增序列  下减序列
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
             if((p->R_angle <=(FOEWARD_60-BACKWARD_40+DT))||(p->R_angle >= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //前推
                
                
                if((p->R_angle >= p->R_ZeroAngle))
                {
                    p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle )/(360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle));
                }
                else
                {
                    p->R_Percent =127L*(360-p->R_ZeroAngle + p->R_angle)/(360 - p->R_ZeroAngle+ p->R_Cal_ForwardAngle);

                }

            }
            else 
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle - p->R_Cal_BackwardAngle);
            }
 
        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle))
        {

            if(p->R_angle<10)
            {
                
                p->R_angle=359;
            }
            
            
            if(p->R_angle<=p->R_ZeroAngle)
            {  
                p->R_Direction=1;    //前推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle-p->R_ZeroAngle);
            }
            
            
        }
    }
    else if(p->R_ZeroAngle<360&&p->R_ZeroAngle>(359-BACKWARD_40))
    {
        //增序列  下减序列
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle>p->R_Cal_ForwardAngle))   
        {
            
            if((p->R_angle <=(FOEWARD_60+DT))||(p->R_angle >= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //前推
                if((p->R_angle >= p->R_ZeroAngle))
                {
                    p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle )/(360-p->R_ZeroAngle+(p->R_Cal_ForwardAngle));
                }
                else
                {
                    p->R_Percent =127L*(360-p->R_ZeroAngle + p->R_angle)/(360-p->R_ZeroAngle+p->R_Cal_ForwardAngle );
                }
            }
            else 
            {
                p->R_Direction=0;   //后推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle - p->R_Cal_BackwardAngle);    //ok
                
            }
        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle <p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_Cal_ForwardAngle))
        {

            if((p->R_angle<=p->R_ZeroAngle)&&(p->R_angle>=(p->R_ZeroAngle-FOEWARD_60-DT)))
            {  
                p->R_Direction=1;    //前推
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else if((p->R_angle>p->R_ZeroAngle)||(p->R_angle <=(BACKWARD_40+DT)))
            {
                p->R_Direction=0;   //后推
                
                
                if(p->R_angle>=p->R_ZeroAngle)
                {  
                    p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle+360-p->R_ZeroAngle);    
                }
                else
                {
                    
                    p->R_Percent =127L*(p->R_angle+360-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle+360-p->R_ZeroAngle); //ok
                    
                }
            }
  
        }
    }
    else  //0<=X<40
    {
        //增序列  下减序列
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_Cal_ForwardAngle))   
        {    
  
            if((p->R_angle>=p->R_ZeroAngle)&&(p->R_angle<=(p->R_ZeroAngle+FOEWARD_60)))   //这里有bug
            {  
                p->R_Direction=1;    //前推
                p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);           //
            }
            else if((p->R_angle<p->R_ZeroAngle)||(p->R_angle >=(360-BACKWARD_40)))
            {
                p->R_Direction=0;   //后推

                if(p->R_angle<p->R_ZeroAngle)
                {  
                    p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(360-p->R_Cal_BackwardAngle+p->R_ZeroAngle);    
                }
                else
                {
                    p->R_Percent =127L*(360-p->R_angle+p->R_ZeroAngle)/(360-p->R_Cal_BackwardAngle+p->R_ZeroAngle); //0kl
                }
            }
        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle >p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle>p->R_Cal_BackwardAngle))
        { 
            if((p->R_angle<=p->R_ZeroAngle)||(p->R_angle >=(360-FOEWARD_60)))  
            {  
                p->R_Direction=1;    //前推

               if(p->R_angle<p->R_ZeroAngle)
                {  
                    p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(360 - p->R_Cal_ForwardAngle +p->R_ZeroAngle);    
                }
                else
                {
                    p->R_Percent =127L*(360-p->R_angle+p->R_ZeroAngle)/(360 - p->R_Cal_ForwardAngle+p->R_ZeroAngle); //0kl
                } 
            }
            else if(p->R_angle>p->R_ZeroAngle&&(p->R_angle <=(p->R_ZeroAngle+BACKWARD_40)))
            {
                p->R_Direction=0;   //后推

                p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle)/(p->R_Cal_BackwardAngle-p->R_ZeroAngle);           //
            }
        }

    }
    //
    if( p->R_Percent>125)
    {
         p->R_Percent=127;
    }
    else if( p->R_Percent<5)
    {
         p->R_Percent=0;
    }
    
    
    
    //左边的使用校正获取的数据，进行判定是否合理校正。
    if(p->L_ZeroAngle>=60&&p->L_ZeroAngle<=299)
    {
        
            //按照增序列   下按照减系列来角度增加和减少
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))
        {
            if((p->L_ZeroAngle>=(299-10))&&(p->L_ZeroAngle<=299))
            {
               if(p->L_angle<10)
               {
                   p->L_angle=359;
               }
            
            }
            
            if((p->L_angle>=p->L_ZeroAngle)&&(p->L_angle<=(p->L_ZeroAngle+FOEWARD_60+DT)))
            {  
                p->L_Direction=1;    //前推
                
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);
            }
            else
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_BackwardAngle);
            }
            
        }
        else if(((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)))
        {
            
            
            if((p->L_ZeroAngle>=60)&&(p->L_ZeroAngle<=(60+10)))
            {
               if(p->L_angle>349)
               {
                   p->L_angle=0;
               }
            }
            if(p->L_angle<=p->L_ZeroAngle)           //bug
            {  
                p->L_Direction=1;    //前推
                
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle)/(p->L_Cal_BackwardAngle - p->L_ZeroAngle);
            }
        }
        
        
    }
    else if(p->L_ZeroAngle>=BACKWARD_40&&p->L_ZeroAngle<FOEWARD_60)
    {
          //增序列  下减序列
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if(p->L_angle>=p->L_ZeroAngle)
            {  
                p->L_Direction=1;    //前推
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);
            }
            else
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_BackwardAngle);  //正确
            }
                
        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle >p->L_Cal_BackwardAngle))
        {
 
            if((p->L_angle >=359-(FOEWARD_60-BACKWARD_40))||(p->L_angle <= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //前推
                
                
                if((p->L_angle <= p->L_ZeroAngle))
                {
                    p->L_Percent =127L*(p->L_ZeroAngle - p->L_angle )/(360 - p->L_Cal_ForwardAngle + p->L_ZeroAngle);
                }
                else
                {
                    p->L_Percent =127L*(360 - p->L_angle + p->L_ZeroAngle)/(360 - p->L_Cal_ForwardAngle + p->L_ZeroAngle);

                }

            }
            else 
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle-p->L_ZeroAngle);
            }
           
        }
        
        
  
    }
    else if(p->L_ZeroAngle>299&&p->L_ZeroAngle<=(359-BACKWARD_40))
    {
        
        //增序列  下减序列
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if((p->L_angle <=(FOEWARD_60-BACKWARD_40+DT))||(p->L_angle >= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //前推
                if((p->L_angle >= p->L_ZeroAngle))
                {
                    p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle )/(360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle));
                }
                else
                {
                    p->L_Percent =127L*(360-p->L_ZeroAngle + p->L_angle)/(360 - p->L_ZeroAngle+ p->L_Cal_ForwardAngle);
                }
            }
            else 
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle - p->L_Cal_BackwardAngle);
            }
        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle))
        {
            if(p->L_angle<10)
            {        
                p->L_angle=359;
            }
            if(p->L_angle<=p->L_ZeroAngle)
            {  
                p->L_Direction=1;    //前推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle-p->L_ZeroAngle);
            }    
        }
    }
    else if((p->L_ZeroAngle<360&&p->L_ZeroAngle>(359-BACKWARD_40)))
    {
         //增序列  下减序列
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle>p->L_Cal_ForwardAngle))   
        {
            
            if((p->L_angle <=(FOEWARD_60+DT))||(p->L_angle >= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //前推
                if((p->L_angle >= p->L_ZeroAngle))
                {
                    p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle )/(360-p->L_ZeroAngle+(p->L_Cal_ForwardAngle));
                }
                else
                {
                    p->L_Percent =127L*(360-p->L_ZeroAngle + p->L_angle)/(360-p->L_ZeroAngle+p->L_Cal_ForwardAngle );
                }
            }
            else 
            {
                p->L_Direction=0;   //后推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle - p->L_Cal_BackwardAngle);    //ok
                
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle <p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_Cal_ForwardAngle))
        {

            if((p->L_angle<=p->L_ZeroAngle)&&(p->L_angle>=(p->L_ZeroAngle-FOEWARD_60-DT)))
            {  
                p->L_Direction=1;    //前推
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else if((p->L_angle>=p->L_ZeroAngle)||(p->L_angle <=(BACKWARD_40+DT)))
            {
                p->L_Direction=0;   //后推
                
                
                if(p->L_angle>=p->L_ZeroAngle)
                {  
                    p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle+360-p->L_ZeroAngle);    
                }
                else
                {
                    
                    p->L_Percent =127L*(p->L_angle+360-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle+360-p->L_ZeroAngle); //ok
                    
                }
            }
        }    
    }
    else //0<=X<40
    {
       //增序列  下减序列
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_Cal_ForwardAngle))   
        {    
  
            if((p->L_angle>=p->L_ZeroAngle)&&(p->L_angle<(p->L_ZeroAngle+FOEWARD_60)))
            {  
                p->L_Direction=1;    //前推
                p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);           //
            }
            else if((p->L_angle<p->L_ZeroAngle)||(p->L_angle >=(360-BACKWARD_40)))
            {
                p->L_Direction=0;   //后推

                if(p->L_angle<p->L_ZeroAngle)
                {  
                    p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(360-p->L_Cal_BackwardAngle+p->L_ZeroAngle);    
                }
                else
                {
                    p->L_Percent =127L*(360-p->L_angle+p->L_ZeroAngle)/(360-p->L_Cal_BackwardAngle+p->L_ZeroAngle); //0kl
                }
            }
        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle >p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle>p->L_Cal_BackwardAngle))
        { 
            if((p->L_angle<=p->L_ZeroAngle)||(p->L_angle >=(360-FOEWARD_60)))  
            {  
                p->L_Direction=1;    //前推

               if(p->L_angle<p->L_ZeroAngle)
                {  
                    p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(360 - p->L_Cal_ForwardAngle + p->L_ZeroAngle);    
                }
                else
                {
                    p->L_Percent =127L*(360-p->L_angle+p->L_ZeroAngle)/(360 - p->L_Cal_ForwardAngle+p->L_ZeroAngle); //0kl
                } 
            }
            else if((p->L_angle>p->L_ZeroAngle)&&(p->L_angle<=(p->L_ZeroAngle+BACKWARD_40))) //if((p->L_angle>=p->L_ZeroAngle)&&(p->L_angle<BACKWARD_40))
            {
                p->L_Direction=0;   //后推

                p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle)/(p->L_Cal_BackwardAngle-p->L_ZeroAngle);           //
            }
        } 
        
        
    
    
    }
    //
    if( p->L_Percent>125)
    {
         p->L_Percent=127;
    }
    else if( p->L_Percent<5)
    {
         p->L_Percent=0;
    }
    
    
}




