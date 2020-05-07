#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x.h"

//
#define STM32_FLASH_SIZE 32 	 		//所选STM32的FLASH容量大小(单位为K)

//FLASH起始地址
#define STM32_FLASH_BASE   0x08000000 	//STM32 FLASH的起始地址
#define FLASH_SYNC_ADDR    0x08007C00   //设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)32扇区
#define FlASH_R_ADC_ADDR   0x08007800   //右边的设置的地址
#define FlASH_L_ADC_ADDR   0x08007400   //左边的设置的地址







u16 STMFLASH_ReadHalfWord(u32 faddr);	//读出半字  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//指定地址开始读取指定长度数据

void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始 ,写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始 ,读出指定长度的数据

//测试写入
void Test_Write(u32 WriteAddr,u16 WriteData);



typedef struct
{
    uint8_t      R_Flash_SaveSuc_Flag;
    uint8_t      L_Flash_SaveSuc_Flag;
    uint8_t      R_Flash_ReadSuc_Flag;
    uint8_t      L_Flash_ReadSuc_Flag;    
    
    u16          R_ADC_temp[5] ;
    u16          R_ADC[5];
    u16          L_ADC_temp[5]; 
    u16          L_ADC[5];   

    u16          SYNC_temp[3]; 
    u16          SYNC_KEY[3]; 
    uint8_t      SYNC_SaveSuc_Flag;
}FLASH_Handle_t;

extern FLASH_Handle_t FLASH_Handle;

void Flash_Save_Angle(FLASH_Handle_t* p);
void Flash_Read_Angle(FLASH_Handle_t* p);

void Flash_Save_SYNC(FLASH_Handle_t* p) ;
void Flash_Read_SYNC(FLASH_Handle_t* p) ;

#endif





