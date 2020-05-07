/***************************************
 * �ļ���  ��angle_as5600.c
 * ����    �����ýǶȴ�����       
 * ʵ��ƽ̨��STM32 ����STM32F103C8T6
 * Ӳ�����ӣ�
 * ��汾  ��ST3.0.0  
 
*****************************************/

#include "angle_as5600.h"

THROTTLE_Handle_t THROTTLE_Handle;
//�Ҷ˵����Ÿ�
u8 R_AS5600_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		
    R_IIC_Start();  
	R_IIC_Send_Byte(0X6C);     				//����AS5600���豸��ַ  д
	R_IIC_Wait_Ack();                       
    R_IIC_Send_Byte(ReadAddr);				//���ͼĴ�����ַ
	R_IIC_Wait_Ack();	                    
	R_IIC_Start();  	 	                 
	R_IIC_Send_Byte(0X6D);                  //����AS5600���豸��ַ  ��			   
	R_IIC_Wait_Ack();	                    
    temp=R_IIC_Read_Byte(0);		        
    R_IIC_Stop();                           //����һ��ֹͣ����	    
	return temp;                            
}

void R_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    R_IIC_Start();   
    R_IIC_Send_Byte(0X6C);            //����AS5600�豸��ַд 
	R_IIC_Wait_Ack();	   
    R_IIC_Send_Byte(WriteAddr);       //���ͼĴ�����ַ
	R_IIC_Wait_Ack(); 	 										  		   
	R_IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	R_IIC_Wait_Ack();  		    	   
    R_IIC_Stop();				      //����һ��ֹͣ���� 
	delay_ms(10);	 				  //�ȴ�AS5600��д����
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


//��˵����Ÿ�
u8 L_AS5600_ReadOneByte(u8 ReadAddr)
{				  
	u8 temp=0;		
    L_IIC_Start();  
	L_IIC_Send_Byte(0X6C);     				 //����AS5600���豸��ַд
	L_IIC_Wait_Ack(); 
    L_IIC_Send_Byte(ReadAddr);				 //���ͼĴ�����ַ
	L_IIC_Wait_Ack();	    
	L_IIC_Start();  	 	   
	L_IIC_Send_Byte(0X6D);                   //����AS5600���豸��ַ��			   
	L_IIC_Wait_Ack();	 
    temp=L_IIC_Read_Byte(0);		   
    L_IIC_Stop();                            //����һ��ֹͣ����	    
	return temp;
}

void L_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    L_IIC_Start();   
    L_IIC_Send_Byte(0X6C);            //����AS5600�豸��ַд 
	L_IIC_Wait_Ack();	   
    L_IIC_Send_Byte(WriteAddr);       //���ͼĴ�����ַ
	L_IIC_Wait_Ack(); 	 										  		   
	L_IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	L_IIC_Wait_Ack();  		    	   
    L_IIC_Stop();						//����һ��ֹͣ���� 
	delay_ms(10);	 				//�ȴ�AS5600��д����
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



















//����У����ʼ��
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
    	
    handle->R_PercentTemp=0;                 //485���͵�Temp
  	handle->L_PercentTemp=0;                 //485���͵�
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
    
	handle->R_angle=0;		            //  �ұ����Ÿ˽Ƕ�
    handle->L_angle=0;                   //  ������Ÿ˽Ƕ�

    handle->R_Pre_angle=0;		        //  �ұ����Ÿ˽Ƕ�
    handle->L_Pre_angle=0; 
    
    for(count_buf=0;count_buf<8;count_buf++)
    {   
      handle->CAN_TxBuf[count_buf]=0;
    }
}

void THROTTLE_Calc(THROTTLE_Handle_t* p)
{	
    //ʹ��У����ȡ�����ݣ������ж��Ƿ����У����
    if(p->R_ZeroAngle>=60&&p->R_ZeroAngle<=299)
    {
        //����������   �°��ռ�ϵ�����Ƕ����Ӻͼ���
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))
        {
            if((((p->R_Cal_ForwardAngle-p->R_ZeroAngle)>=(FOEWARD_60-DT))&&((p->R_Cal_ForwardAngle-p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_ZeroAngle-p->R_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->R_ZeroAngle-p->R_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->R_CalError_Flag=1; //�ұߴ���
                return;
            }
        }
        else if(((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)))
        {

            if((((p->R_ZeroAngle-p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle-p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                && (((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
              p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
              p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {
                p->R_CalError_Flag=1; //�ұߴ���
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
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->R_CalError_Flag=1; //�ұߴ���
                return;
            }    
        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle >p->R_Cal_BackwardAngle))
        {
            if((((360-(p->R_Cal_ForwardAngle)+p->R_ZeroAngle)>=(FOEWARD_60-DT))&&((360-(p->R_Cal_ForwardAngle)+p->R_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->R_Cal_BackwardAngle - p->R_ZeroAngle<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle - p->R_ZeroAngle>=(BACKWARD_40-DT)))))
            {
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {
                 p->R_CalError_Flag=1; //�ұߴ���
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
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->R_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle))
        {

             if((((p->R_ZeroAngle - p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->R_CalError_Flag=1; //�ұߴ���
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
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->R_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle <p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_Cal_ForwardAngle))
        {

             if((((p->R_ZeroAngle - p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle+360-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle+360-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->R_CalError_Flag=1; //�ұߴ���
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
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->R_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle >p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle>p->R_Cal_BackwardAngle))
        {

             if((((p->R_ZeroAngle +360- p->R_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->R_ZeroAngle+360 - p->R_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->R_Cal_BackwardAngle-p->R_ZeroAngle)<=(BACKWARD_40+DT))&&((p->R_Cal_BackwardAngle-p->R_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->R_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->R_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->R_CalError_Flag=1; //�ұߴ���
                return;
            }   
        }
        else
        { 
            p->R_CalError_Flag=1;
            return;
        }
    }

    //���ʹ��У����ȡ�����ݣ������ж��Ƿ����У����
    //ʹ��У����ȡ�����ݣ������ж��Ƿ����У����
    if(p->L_ZeroAngle>=60&&p->L_ZeroAngle<=299)
    {
        //����������   �°��ռ�ϵ�����Ƕ����Ӻͼ���
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))
        {
            if((((p->L_Cal_ForwardAngle-p->L_ZeroAngle)>=(FOEWARD_60-DT))&&((p->L_Cal_ForwardAngle-p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_ZeroAngle-p->L_Cal_BackwardAngle)<=(BACKWARD_40+DT))&&((p->L_ZeroAngle-p->L_Cal_BackwardAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->L_CalError_Flag=1; //�ұߴ���
                return;
            }
        }
        else if(((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)))
        {

            if((((p->L_ZeroAngle-p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle-p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                && (((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
              p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
              p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {
                p->L_CalError_Flag=1; //�ұߴ���
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
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->L_CalError_Flag=1; //�ұߴ���
                return;
            }    
        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle >p->L_Cal_BackwardAngle))
        {
            if((((360-(p->L_Cal_ForwardAngle)+p->L_ZeroAngle)>=(FOEWARD_60-DT))&&((360-(p->L_Cal_ForwardAngle)+p->L_ZeroAngle)<=(FOEWARD_60+DT)))
                 &&((p->L_Cal_BackwardAngle - p->L_ZeroAngle<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle - p->L_ZeroAngle>=(BACKWARD_40-DT)))))
            {
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {
                 p->L_CalError_Flag=1; //�ұߴ���
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
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->L_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle))
        {

             if((((p->L_ZeroAngle - p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->L_CalError_Flag=1; //�ұߴ���
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
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->L_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle <p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_Cal_ForwardAngle))
        {

             if((((p->L_ZeroAngle - p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle+360-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle+360-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->L_CalError_Flag=1; //�ұߴ���
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
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������

            }
            else
            {
                 p->L_CalError_Flag=1; //�ұߴ���
                return;
            
            }

        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle >p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle>p->L_Cal_BackwardAngle))
        {

             if((((p->L_ZeroAngle +360- p->L_Cal_ForwardAngle)>=(FOEWARD_60-DT))&&((p->L_ZeroAngle+360 - p->L_Cal_ForwardAngle)<=(FOEWARD_60+DT)))
                &&(((p->L_Cal_BackwardAngle-p->L_ZeroAngle)<=(BACKWARD_40+DT))&&((p->L_Cal_BackwardAngle-p->L_ZeroAngle)>=(BACKWARD_40-DT))))
            {
                p->L_CalSuc_Flag=1;  //�ұ�У���ɹ�
                p->L_CalError_Flag=0; //�ұߴ�������
            }
            else
            {   
                p->L_CalError_Flag=1; //�ұߴ���
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
    
    //ʹ��У����ȡ�����ݣ������ж��Ƿ����У����
    if(p->R_ZeroAngle>=60&&p->R_ZeroAngle<=299)
    {
        //����������   �°��սǶ����Ӻͼ���
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
                p->R_Direction=1;    //ǰ��
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);
            }
            else
            {
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_BackwardAngle);
            }
            
        }//������
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
                p->R_Direction=1;    //ǰ��
                
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else
            {
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle)/(p->R_Cal_BackwardAngle - p->R_ZeroAngle);
            }
        }
    }
    else if(p->R_ZeroAngle>=BACKWARD_40&&p->R_ZeroAngle<FOEWARD_60)
    {     
        //������  �¼�����
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
            if(p->R_angle>=p->R_ZeroAngle)
            {  
                p->R_Direction=1;    //ǰ��
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);
            }
            else
            {
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_BackwardAngle);  //��ȷ
            }
                
        }
        else if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_ForwardAngle >p->R_Cal_BackwardAngle))
        {
 
            if((p->R_angle >=359-(FOEWARD_60-BACKWARD_40))||(p->R_angle <= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //ǰ��
                
                
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
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle-p->R_ZeroAngle);
            }
           
        }

    }
    else if(p->R_ZeroAngle>299&&p->R_ZeroAngle<=(359-BACKWARD_40))
    {
         //������  �¼�����
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle))   
        {
             if((p->R_angle <=(FOEWARD_60-BACKWARD_40+DT))||(p->R_angle >= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //ǰ��
                
                
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
                p->R_Direction=0;   //����
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
                p->R_Direction=1;    //ǰ��
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else
            {
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_angle-p->R_ZeroAngle)/(p->R_Cal_BackwardAngle-p->R_ZeroAngle);
            }
            
            
        }
    }
    else if(p->R_ZeroAngle<360&&p->R_ZeroAngle>(359-BACKWARD_40))
    {
        //������  �¼�����
        if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle>p->R_Cal_ForwardAngle))   
        {
            
            if((p->R_angle <=(FOEWARD_60+DT))||(p->R_angle >= p->R_ZeroAngle))
            {  
                p->R_Direction=1;    //ǰ��
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
                p->R_Direction=0;   //����
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle - p->R_Cal_BackwardAngle);    //ok
                
            }
        }
        else if((p->R_Cal_ForwardAngle < p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle <p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle < p->R_Cal_ForwardAngle))
        {

            if((p->R_angle<=p->R_ZeroAngle)&&(p->R_angle>=(p->R_ZeroAngle-FOEWARD_60-DT)))
            {  
                p->R_Direction=1;    //ǰ��
                p->R_Percent =127L*(p->R_ZeroAngle-p->R_angle)/(p->R_ZeroAngle-p->R_Cal_ForwardAngle);
            }
            else if((p->R_angle>p->R_ZeroAngle)||(p->R_angle <=(BACKWARD_40+DT)))
            {
                p->R_Direction=0;   //����
                
                
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
        //������  �¼�����
        if((p->R_Cal_ForwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_ZeroAngle)&&(p->R_Cal_BackwardAngle > p->R_Cal_ForwardAngle))   
        {    
  
            if((p->R_angle>=p->R_ZeroAngle)&&(p->R_angle<=(p->R_ZeroAngle+FOEWARD_60)))   //������bug
            {  
                p->R_Direction=1;    //ǰ��
                p->R_Percent =127L*(p->R_angle - p->R_ZeroAngle)/(p->R_Cal_ForwardAngle-p->R_ZeroAngle);           //
            }
            else if((p->R_angle<p->R_ZeroAngle)||(p->R_angle >=(360-BACKWARD_40)))
            {
                p->R_Direction=0;   //����

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
                p->R_Direction=1;    //ǰ��

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
                p->R_Direction=0;   //����

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
    
    
    
    //��ߵ�ʹ��У����ȡ�����ݣ������ж��Ƿ����У����
    if(p->L_ZeroAngle>=60&&p->L_ZeroAngle<=299)
    {
        
            //����������   �°��ռ�ϵ�����Ƕ����Ӻͼ���
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
                p->L_Direction=1;    //ǰ��
                
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);
            }
            else
            {
                p->L_Direction=0;   //����
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
                p->L_Direction=1;    //ǰ��
                
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else
            {
                p->L_Direction=0;   //����
                p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle)/(p->L_Cal_BackwardAngle - p->L_ZeroAngle);
            }
        }
        
        
    }
    else if(p->L_ZeroAngle>=BACKWARD_40&&p->L_ZeroAngle<FOEWARD_60)
    {
          //������  �¼�����
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if(p->L_angle>=p->L_ZeroAngle)
            {  
                p->L_Direction=1;    //ǰ��
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);
            }
            else
            {
                p->L_Direction=0;   //����
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_BackwardAngle);  //��ȷ
            }
                
        }
        else if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_ForwardAngle >p->L_Cal_BackwardAngle))
        {
 
            if((p->L_angle >=359-(FOEWARD_60-BACKWARD_40))||(p->L_angle <= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //ǰ��
                
                
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
                p->L_Direction=0;   //����
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle-p->L_ZeroAngle);
            }
           
        }
        
        
  
    }
    else if(p->L_ZeroAngle>299&&p->L_ZeroAngle<=(359-BACKWARD_40))
    {
        
        //������  �¼�����
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle))   
        {
            if((p->L_angle <=(FOEWARD_60-BACKWARD_40+DT))||(p->L_angle >= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //ǰ��
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
                p->L_Direction=0;   //����
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
                p->L_Direction=1;    //ǰ��
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else
            {
                p->L_Direction=0;   //����
                p->L_Percent =127L*(p->L_angle-p->L_ZeroAngle)/(p->L_Cal_BackwardAngle-p->L_ZeroAngle);
            }    
        }
    }
    else if((p->L_ZeroAngle<360&&p->L_ZeroAngle>(359-BACKWARD_40)))
    {
         //������  �¼�����
        if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle>p->L_Cal_ForwardAngle))   
        {
            
            if((p->L_angle <=(FOEWARD_60+DT))||(p->L_angle >= p->L_ZeroAngle))
            {  
                p->L_Direction=1;    //ǰ��
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
                p->L_Direction=0;   //����
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle - p->L_Cal_BackwardAngle);    //ok
                
            }

        }
        else if((p->L_Cal_ForwardAngle < p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle <p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle < p->L_Cal_ForwardAngle))
        {

            if((p->L_angle<=p->L_ZeroAngle)&&(p->L_angle>=(p->L_ZeroAngle-FOEWARD_60-DT)))
            {  
                p->L_Direction=1;    //ǰ��
                p->L_Percent =127L*(p->L_ZeroAngle-p->L_angle)/(p->L_ZeroAngle-p->L_Cal_ForwardAngle);
            }
            else if((p->L_angle>=p->L_ZeroAngle)||(p->L_angle <=(BACKWARD_40+DT)))
            {
                p->L_Direction=0;   //����
                
                
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
       //������  �¼�����
        if((p->L_Cal_ForwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_ZeroAngle)&&(p->L_Cal_BackwardAngle > p->L_Cal_ForwardAngle))   
        {    
  
            if((p->L_angle>=p->L_ZeroAngle)&&(p->L_angle<(p->L_ZeroAngle+FOEWARD_60)))
            {  
                p->L_Direction=1;    //ǰ��
                p->L_Percent =127L*(p->L_angle - p->L_ZeroAngle)/(p->L_Cal_ForwardAngle-p->L_ZeroAngle);           //
            }
            else if((p->L_angle<p->L_ZeroAngle)||(p->L_angle >=(360-BACKWARD_40)))
            {
                p->L_Direction=0;   //����

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
                p->L_Direction=1;    //ǰ��

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
                p->L_Direction=0;   //����

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




