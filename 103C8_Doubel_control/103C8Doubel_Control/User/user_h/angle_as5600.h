#ifndef __ANGGLE_AS5600_H
#define	__ANGGLE_AS5600_H

#include "stm32f10x.h"
#include "i2c.h"
/*****�궨��******/
/*****��غ궨��******/
#define AS5600_ADDR	    0x36	// �豸��ַ

//AS5600�ǷǽӴ�ʽ��λ�ô�����
//Configuration Registers
#define ZMCO		    0x00
#define ZPOS_H		    0x01		// start position
#define ZPOS_L		    0x02
#define MPOS_H		    0x03		// stop position
#define MPOS_L		    0x04
#define MANG_H		    0x05        // maximum angle
#define MANG_L		    0x06
#define CONF_L		    0x07        //CONF Register
#define CONF_H		    0x08

//Output Registers
#define	RAWANG_H	    0x0C		// 
#define RAWANG_L	    0x0D
#define ANGLE_H		    0x0E		// 
#define ANGLE_L		    0x0F

//Status Registers
#define STATUS		    0x0B
#define AGC			    0x1A
#define MAGN_H		    0x1B
#define MAGN_L		    0x1C
//Burn Commands
#define BURN		    0xFF

//CONF Register
#define	CONF_L_PM		0x03		// 	00 = NOM, 01 = LPM1, 10 = LPM2, 11 = LPM3
#define CONF_H_SF		0x03		//  00 = 16x (1); 01 = 8x; 10 = 4x; 11 = 2x
#define CONF_H_FTH	    0x1C		// 
#define CONF_H_WD		0x20		// Watchdog

//
#define	MAGNET_LOW	    0x10
#define MAGNET_HIGH	    0x08
#define MAGNET_NORM     0x20

#define HYST_MASK		0x0C
#define HYST_OFF		0x00
#define HYST_1LSB		0x04
#define HYST_2LSB		0x08
#define HYST_3LSB		0x0C

#define	OUT_STG_MASK			0x30
#define	OUT_STG_ANALOG			0x00
#define OUT_STG_ANALOG_RED	    0x10
#define	OUT_STG_PWM				0x20

#define PM_MASK					0x03
#define AS5600_PM_MODE_NOM      0x00
#define AS5600_PM_MODE_LPM1     0x01
#define AS5600_PM_MODE_LPM2     0x02
#define AS5600_PM_MODE_LPM3     0x03

#define PWMF_MASK				0xC0
#define PWMF_115HZ				0x00
#define	PWMF_230HZ				0x40
#define	PWMF_460HZ				0x80
#define PWMF_920HZ				0xC0

#define AS5600_STATUS_MH        0X08
#define AS5600_STATUS_ML        0X10
#define AS5600_STATUS_MD        0X20


//�궨�壬ǰ����ݸ˵ĽǶ�
#define FOEWARD_60    60
#define BACKWARD_40   35          //����35
#define DT             5          //5
//ö��״̬��
typedef enum
{
	THROTTLE_State_Off = 0,
	THROTTLE_State_NeedReset, //1  ��Ҫ������������״̬
	THROTTLE_State_Reseted,   //2  �����������״̬
	THROTTLE_State_Forward,   //3  ����
    THROTTLE_State_Zero,      //4  
	THROTTLE_State_Backward,  //5  �����״̬
	THROTTLE_State_Cal_Start, 
	THROTTLE_State_Cal_Stop,  
	THROTTLE_State_Failure,   
    THROTTLE_State_Success    
}THROTTLE_State_e;

typedef struct
{
    //У����־λ��У������
	uint8_t      R_Calibration_Flag;
    uint8_t      R_CalError_Flag;
    uint8_t      R_CalSuc_Flag;
    uint8_t      R_SetZero_Flag;
    
    uint8_t      L_Calibration_Flag;
    uint8_t      L_CalError_Flag;
    uint8_t      L_CalSuc_Flag;
    uint8_t      L_SetZero_Flag;
    
    uint8_t      Limit_TEMP;
	int16_t  	 Temp_Val;
    uint8_t  	 Saft_Key; 
	uint8_t      ForwardPercentMax;             //The Forward max power percent 0-127
	uint8_t      BackwardPercentMax;            //The Backward max power percent 0-127

    uint8_t      R_ForwardPercent;              //The Forward max power percent 0-127
	uint8_t      R_BackwardPercent;             //The Backward max power percent 0-127
     
    uint8_t      L_ForwardPercent;              //The Forward max power percent 0-127
	uint8_t      L_BackwardPercent;             //The Backward max power percent 0-127
    	
    int16_t	     R_PercentTemp;                 //485���͵�Temp
  	int16_t	     L_PercentTemp;                 //485���͵�
	int16_t	     R_Percent;					    //percent of the full throttle from 0~127 without direction
	int16_t	     L_Percent;					    //percent of the full throttle from 0~127 without direction
    
    int16_t      R_Direction;					//direction 1: forward 0: backward
    int16_t      L_Direction;					//direction 1: forward 0: backward  
    
    int32_t      R_Cal_ForwardAngle;
    int32_t      R_ZeroAngle;
	int32_t      R_Cal_BackwardAngle;
	
    int32_t      L_Cal_ForwardAngle;
    int32_t      L_ZeroAngle;
	int32_t      L_Cal_BackwardAngle;
    
	int32_t      R_angle;		               //  �ұ����Ÿ˽Ƕ�
    int32_t      L_angle;                      //  ������Ÿ˽Ƕ�

    int32_t      R_Pre_angle;		           //  �ұ����Ÿ˽Ƕ�
    int32_t      L_Pre_angle;                  //  ������Ÿ˽Ƕ�
    THROTTLE_State_e  State;				   
    								
    uint8_t      CAN_TxBuf[8];
    
                                               
}THROTTLE_Handle_t; 

extern THROTTLE_Handle_t THROTTLE_Handle;



//�Ҷ˵ĽǶȴ���������   ��λд��
u8 R_AS5600_ReadOneByte(u8 ReadAddr);
void R_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite);
u8 R_AS5600_Read(void);
uint8_t R_AS5600_GetAGC(void);
uint16_t R_AS5600_ReadConf(void);
uint16_t R_AS5600_ReadMagitue(void);
void R_AS5600_EnableWd(void);
void R_AS5600_SetPMMode(uint8_t mode);
uint8_t R_AS5600_GetStatus(void);
extern uint16_t R_AS5600_GetAngle(void);
extern void R_AS5600_SetOutputStage(uint8_t OutStage);


//��˵ĽǶȴ���������
u8 L_AS5600_ReadOneByte(u8 ReadAddr);
void L_AS5600_WriteOneByte(u8 WriteAddr,u8 DataToWrite);
u8 L_AS5600_Read(void);
uint8_t L_AS5600_GetAGC(void);
uint16_t L_AS5600_ReadConf(void);
uint16_t L_AS5600_ReadMagitue(void);
void L_AS5600_EnableWd(void);
void L_AS5600_SetPMMode(uint8_t mode);
uint8_t L_AS5600_GetStatus(void);
extern uint16_t L_AS5600_GetAngle(void);
extern void L_AS5600_SetOutputStage(uint8_t OutStage);


//�Ƕȴ洢���ж��Ƿ�У�������Ƿ�У���ɹ�
void THROTTLE_Init(THROTTLE_Handle_t* handle);
void THROTTLE_Calc(THROTTLE_Handle_t* handle);
void Throttle_Value_Conversion(THROTTLE_Handle_t* p);
#endif /* __ANGGLE_AS5600_H */



