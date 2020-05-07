#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f10x.h"

//
#define STM32_FLASH_SIZE 32 	 		//��ѡSTM32��FLASH������С(��λΪK)

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE   0x08000000 	//STM32 FLASH����ʼ��ַ
#define FLASH_SYNC_ADDR    0x08007C00   //����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)32����
#define FlASH_R_ADC_ADDR   0x08007800   //�ұߵ����õĵ�ַ
#define FlASH_L_ADC_ADDR   0x08007400   //��ߵ����õĵ�ַ







u16 STMFLASH_ReadHalfWord(u32 faddr);	//��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������

void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼ ,д��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ ,����ָ�����ȵ�����

//����д��
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





