#include "flash.h"
#include "key_control_mode.h"
#include "angle_as5600.h"
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�
//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{   
 	u16 i;    
	FLASH_Unlock();				 //����
	while(1) 
	{	
		FLASH_ErasePage((WriteAddr-STM32_FLASH_BASE)/1024*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
		for(i=0;i<NumToWrite;i++)//����
		{
			STMFLASH_BUF[i]=pBuffer[i];	  
		}
		STMFLASH_Write_NoCheck((WriteAddr-STM32_FLASH_BASE)/1024*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,NumToWrite);//д����������  				   
		break;//д������� 
	};	
	FLASH_Lock();//����
}
//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//WriteAddr:��ʼ��ַ
//WriteData:Ҫд�������
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}


FLASH_Handle_t FLASH_Handle;


//�洢У���Ƕ�
void Flash_Save_Angle(FLASH_Handle_t* p) 
{
    p->L_ADC_temp[0]=THROTTLE_Handle.L_ZeroAngle;
    p->L_ADC_temp[1]=THROTTLE_Handle.L_Cal_ForwardAngle;
    p->L_ADC_temp[2]=THROTTLE_Handle.L_Cal_BackwardAngle;
    p->L_ADC_temp[3]=THROTTLE_Handle.L_ZeroAngle^THROTTLE_Handle.L_Cal_ForwardAngle^THROTTLE_Handle.L_Cal_BackwardAngle;
    p->L_ADC_temp[4]=0xaaaa;    
     
    p->R_ADC_temp[0]=THROTTLE_Handle.R_ZeroAngle;
    p->R_ADC_temp[1]=THROTTLE_Handle.R_Cal_ForwardAngle;
    p->R_ADC_temp[2]=THROTTLE_Handle.R_Cal_BackwardAngle;
    p->R_ADC_temp[3]=THROTTLE_Handle.R_ZeroAngle^THROTTLE_Handle.R_Cal_ForwardAngle^THROTTLE_Handle.R_Cal_BackwardAngle;
    p->R_ADC_temp[4]=0xaaaa;
    
    STMFLASH_Write(FlASH_R_ADC_ADDR,(u16*)p->R_ADC_temp,5);     //flash
    STMFLASH_Write(FlASH_L_ADC_ADDR,(u16*)p->L_ADC_temp,5);     //flash
    STMFLASH_Read(FlASH_R_ADC_ADDR,(u16*)p->R_ADC,5);
    STMFLASH_Read(FlASH_L_ADC_ADDR,(u16*)p->L_ADC,5);

   if((p->R_ADC[3]==p->R_ADC_temp[3])&&(p->R_ADC[4]==p->R_ADC_temp[4]))
   {
       p->R_Flash_SaveSuc_Flag=1;
   }
   if((p->L_ADC[3]==p->L_ADC_temp[3])&&(p->L_ADC[4]==p->L_ADC_temp[4]))
   {
       p->L_Flash_SaveSuc_Flag=1;
   }		
}

//��ȡflash�д洢�ĽǶ�
void Flash_Read_Angle(FLASH_Handle_t* p) 
{
    STMFLASH_Read(FlASH_R_ADC_ADDR,(u16*)p->R_ADC,5);
    STMFLASH_Read(FlASH_L_ADC_ADDR,(u16*)p->L_ADC,5);
    if((p->R_ADC[4]==0xaaaa)&&(p->R_ADC[3]==(p->R_ADC[0]^p->R_ADC[1]^p->R_ADC[2])))
    {
        THROTTLE_Handle.R_ZeroAngle        = p->R_ADC[0];
        THROTTLE_Handle.R_Cal_ForwardAngle = p->R_ADC[1];
        THROTTLE_Handle.R_Cal_BackwardAngle= p->R_ADC[2];
        p->R_Flash_ReadSuc_Flag=1;
    }
    else
    {
        THROTTLE_Handle.R_ZeroAngle        = 0;
        THROTTLE_Handle.R_Cal_ForwardAngle = 0;
        THROTTLE_Handle.R_Cal_BackwardAngle= 0;
        p->R_Flash_ReadSuc_Flag=0;
    }
          
    if((p->L_ADC[4]==0xaaaa)&&(p->L_ADC[3]==(p->L_ADC[0]^p->L_ADC[1]^p->L_ADC[2])))
    {
        THROTTLE_Handle.L_ZeroAngle        = p->L_ADC[0];
        THROTTLE_Handle.L_Cal_ForwardAngle = p->L_ADC[1];
        THROTTLE_Handle.L_Cal_BackwardAngle= p->L_ADC[2];
        p->L_Flash_ReadSuc_Flag=1;
    }
    else
    {
        THROTTLE_Handle.L_ZeroAngle        = 0;
        THROTTLE_Handle.L_Cal_ForwardAngle = 0;
        THROTTLE_Handle.L_Cal_BackwardAngle= 0;
        p->L_Flash_ReadSuc_Flag=0;
    }          
    
    if( p->R_Flash_ReadSuc_Flag==1&&p->L_Flash_ReadSuc_Flag==1)
    {
        
        THROTTLE_Handle.State=THROTTLE_State_Cal_Stop;  //У������ɹ�״̬
//      THROTTLE_Calc(&THROTTLE_Handle);//˫Զ���ж�У���Ƿ���ȷ ����У���㷨
    }
    else
    {
        
        THROTTLE_Handle.State=THROTTLE_State_NeedReset;  //��ҪУ��

    }
    
}
void Flash_Save_SYNC(FLASH_Handle_t* p) 
{

    p->SYNC_temp[0]=SYNC_ContorlMode.State;
    p->SYNC_temp[1]=0xaaaa;
    p->SYNC_temp[2]=SYNC_ContorlMode.State^0xaaaa;

    STMFLASH_Write(FLASH_SYNC_ADDR,(u16*)p->SYNC_temp,3);     //flash
    STMFLASH_Read(FLASH_SYNC_ADDR,(u16*)p->SYNC_KEY,3);

   if((p->SYNC_KEY[0]==p->SYNC_temp[0])&&(p->SYNC_KEY[2]==p->SYNC_temp[2]))
   {
       p->SYNC_SaveSuc_Flag=1;
   }
   else
   {
      p->SYNC_SaveSuc_Flag=0;
   }
}
void Flash_Read_SYNC(FLASH_Handle_t* p) 
{
    
    STMFLASH_Read(FLASH_SYNC_ADDR,(u16*)p->SYNC_KEY,3);

    if((p->SYNC_KEY[1]==0xaaaa)&&(p->SYNC_KEY[2]==(p->SYNC_KEY[0]^p->SYNC_KEY[1])))
    {
        SYNC_ContorlMode.State=p->SYNC_KEY[0];
    }
    else
    {
         SYNC_ContorlMode.State=ControlMode_State_R;
    }
}


