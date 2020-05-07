#include "flash.h"
#include "key_control_mode.h"
#include "angle_as5600.h"
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
} 
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{   
 	u16 i;    
	FLASH_Unlock();				 //解锁
	while(1) 
	{	
		FLASH_ErasePage((WriteAddr-STM32_FLASH_BASE)/1024*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
		for(i=0;i<NumToWrite;i++)//复制
		{
			STMFLASH_BUF[i]=pBuffer[i];	  
		}
		STMFLASH_Write_NoCheck((WriteAddr-STM32_FLASH_BASE)/1024*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,NumToWrite);//写入整个扇区  				   
		break;//写入结束了 
	};	
	FLASH_Lock();//上锁
}
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字 
}


FLASH_Handle_t FLASH_Handle;


//存储校正角度
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

//读取flash中存储的角度
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
        
        THROTTLE_Handle.State=THROTTLE_State_Cal_Stop;  //校正步骤成功状态
//      THROTTLE_Calc(&THROTTLE_Handle);//双远操判定校正是否正确 油门校正算法
    }
    else
    {
        
        THROTTLE_Handle.State=THROTTLE_State_NeedReset;  //需要校正

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


