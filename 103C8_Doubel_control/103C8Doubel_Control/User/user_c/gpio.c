/***************************************
 * 文件名  ：gpio.c
 * 描述    ：配置GPIO 按键以及LED灯        
 * 实验平台：STM32 基于STM32F103C8T6
 * 硬件连接：
 * 库版本  ：ST3.0.0  
******************************************/
#include "gpio.h"
#include "key_control_mode.h"
#include "flash.h"
Open_Close_Switch_Handle_t  Open_Close_Switch_Handle;
LED_Handle_t LED_Handle;
void Key_GPIO_Init(void){
    
    GPIO_InitTypeDef         GPIO_InitStructre;
//    GPIO_DeInit(GPIOA);
//    GPIO_DeInit(GPIOB); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOB的时钟
    /********KEY_GPIO的配置******************/
    GPIO_InitStructre.GPIO_Pin   = KEY1_GPIO|KEY2_GPIO|KEY3_GPIO|KEY4_GPIO;  
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_IPU ;
    GPIO_Init(KEY_GPIO, &GPIO_InitStructre);
 
}

void LED_GPIO_Init(void){
    
    GPIO_InitTypeDef         GPIO_InitStructre;
    /********GPIO_LED的配置******************/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);//开启GPIOB的时钟 
    //开启AFIO 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    // 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    GPIO_InitStructre.GPIO_Pin   = HALL_GPIO_Pin;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructre);
    

    
    
    GPIO_InitStructre.GPIO_Pin   = K1_G_GPIO_Pin|K2_G_GPIO_Pin;  //try
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructre);
    
    GPIO_InitStructre.GPIO_Pin   = K3_G_GPIO_Pin|K4_G_GPIO_Pin;
    GPIO_InitStructre.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;
  
    GPIO_Init(GPIOC, &GPIO_InitStructre);
  
}
void Open_Close_Switch_Init(void)
{
    Open_Close_Switch_Handle.Close_Flag=1;
    Open_Close_Switch_Handle.Close_State=1;
    Open_Close_Switch_Handle.Open_Flag=0;
    Open_Close_Switch_Handle.Open_State=0;
}


//按键计数清零
void KEY_CountClear(KEY_t* p)
{
	if(p->Action == KEY_Action_Release)
	{
		p->Press_Count = 0;
		p->Lock = 0;
	}  
}




//获取按键的按键的短按、中按和长按类型
void KEY_Get_Touch_Type(KEY_t* p)
{
    if((p->Press_Count > 1) &&(p->Press_Count <= 8))             //8*100ms=0.8s
	{ 
		if(p->Action == KEY_Action_Release)
		{
			p->Touch_Type = Touch_Short;                          //短按标志
		}
	}
	else if((p->Press_Count >8) && (p->Press_Count <= 18))       //18*100ms=1.8s
	{
		if(p->Action == KEY_Action_Release)
		{
			p->Touch_Type = Touch_Mid;                            
		}
	}
	else if((p->Press_Count > 18)&&(p->Press_Count < 200)&&(p->Lock == 0) )//时间从1.8s到20s
	{		
		p->Lock = 1;
		p->Touch_Type = Touch_Long;                      //长按标志
	}
//  LED_Next_Menu_Config(&LED_Handle);                   //下一个界面这个部分根据以上的短按、中按和长按，进行相应为的设置。
    All_Function_Handle(&KEY_Handle);                    //校正好了，油门为零时候函数开放
    //release 清除计数
    if(p->Action == KEY_Action_Release)
	{
		p->Press_Count = 0;
		p->Lock = 0;
	}  
	p->Touch_Type = Touch_NO;                            //没有按下  
} 
void All_Function_Handle(KEY_Handle_t* p)               //按键菜单处理函数，
{
    if(p->Open_Close_Key.Touch_Type==Touch_Long)        //key1开关机长按控制键
    {
        if(Open_Close_Switch_Handle.Open_Flag==0)
        {
            Buzzer_Handle.Buzzer_long_flag=1;
            Open_Close_Switch_Handle.Close_Flag=0;
             Open_Close_Switch_Handle.Close_State=0;
            Open_Close_Switch_Handle.Open_Flag =1;
            Open_Close_Switch_Handle.Open_State=1;
            LED_Handle.LED_All_Trigger_flag    =1;
            //正常模式          
            p->DISPALY_MODE=NORM_MODE; 
            
            K3G_OFF;
            K2G_OFF;
            FLASH_Handle.L_Flash_SaveSuc_Flag=0;
            FLASH_Handle.R_Flash_SaveSuc_Flag=0;    
        }
        else
        {
            Buzzer_Handle.Buzzer_long_flag=1;
            Open_Close_Switch_Handle.Close_Flag=1;
            Open_Close_Switch_Handle.Close_State=1;
            Open_Close_Switch_Handle.Open_Flag =0;
            Open_Close_Switch_Handle.Open_State=0;
            BUZZER_Off();
            LED_Handle.LED_Key4_Trigger_flag = 0;   //LED
            LED_All_OFF();
        }
    }
    else if(p->Open_Close_Key.Touch_Type==Touch_Mid)  //开关机中按按控制键
    {
    
    
    }
    else if(p->Open_Close_Key.Touch_Type==Touch_Short)
    {
    
    
    }
    
    if(p->SYNC_Control_Key.Touch_Type==Touch_Long)    //key2同步模式长按控制键
    {
         Buzzer_Handle.Buzzer_long_flag=1;
         p->DISPALY_MODE=SYNC_MODE;  
         K4G_OFF;  
         K2G_OFF; 
        
//        if(SYNC_ContorlMode.State==ControlMode_State_0)
//        {
//            SYNC_ContorlMode.State=ControlMode_State_R; 
//            LED_Handle.LED_Key2_flag    =1;      
//        }
        if(SYNC_ContorlMode.State==ControlMode_State_R)
        {  
            SYNC_ContorlMode.State=ControlMode_State_L;
            LED_Handle.LED_Key4_flag    =1;
            Flash_Save_SYNC(&FLASH_Handle);           
            
        }
        else if(SYNC_ContorlMode.State==ControlMode_State_L)
        {
            SYNC_ContorlMode.State=ControlMode_State_R;
            LED_Handle.LED_Key2_flag    =1;
            Flash_Save_SYNC(&FLASH_Handle);           
        }
    }
    else if(p->SYNC_Control_Key.Touch_Type==Touch_Mid)
    {
    
    
    }
    else if(p->SYNC_Control_Key.Touch_Type==Touch_Short)
    {
        //SYNC模式限制功率
         Buzzer_Handle.Buzzer_one_flag=1;
         p->DISPALY_MODE =SYNC_MODE; 
         K4G_OFF;  
         K2G_OFF;
         Flash_Read_SYNC(&FLASH_Handle);            //flash读取存储值
    }
    
    if(p->DOCK_Key.Touch_Type==Touch_Long)            //key3  DOCK长按控制键
    {
        Buzzer_Handle.Buzzer_long_flag=1;
       if(DOCK_ContorlMode.R_L_Change_Flag==1)
       {
        DOCK_ContorlMode.R_L_Change_Flag=0;
       
       }
       else 
       {
         DOCK_ContorlMode.R_L_Change_Flag=1;

       }
 
    }
    else if(p->DOCK_Key.Touch_Type==Touch_Mid)
    {
    
    
    }
    else if(p->DOCK_Key.Touch_Type==Touch_Short)
    {
        //dock模式限制功率
        Buzzer_Handle.Buzzer_one_flag=1;
        p->DISPALY_MODE=DOCK_MODE; 
        K4G_OFF;  
        K3G_OFF;
        K2G_ON;
        
        
        if(DOCK_ContorlMode.State==ControlMode_State_0)
        { 
            THROTTLE_Handle.Limit_TEMP=30;      
        }
      
    }
    if(p->NORM_Key.Touch_Type==Touch_Long)            //NORM模式下长按控制键
    { 
      //长按进入校正模式
      Buzzer_Handle.Buzzer_long_flag=1;
      THROTTLE_Handle.R_Calibration_Flag=1;
      THROTTLE_Handle.L_Calibration_Flag=1;
      THROTTLE_Handle.L_CalSuc_Flag=0;
      THROTTLE_Handle.R_CalSuc_Flag=0;
      THROTTLE_Handle.State=THROTTLE_State_Forward;
     
      K4G_OFF;
      LED_Handle.LED_Key4_Trigger_flag = 0;   //LED

    }
    else if(p->NORM_Key.Touch_Type==Touch_Mid)
    {
    
    
    }
    else if(p->NORM_Key.Touch_Type==Touch_Short)
    {
       Buzzer_Handle.Buzzer_one_flag=1;
       KEY_Handle.DISPALY_MODE=NORM_MODE;
       K2G_OFF;
       K3G_OFF;
        
       SYNC_ContorlMode.State=ControlMode_State_0;
       DOCK_ContorlMode.State=ControlMode_State_0;

       if(THROTTLE_Handle.R_Calibration_Flag==1&&THROTTLE_Handle.L_Calibration_Flag==1)
       {   
            if(THROTTLE_Handle.State==THROTTLE_State_Zero)     
            {
                K3G_ON;
                THROTTLE_Handle.State=THROTTLE_State_Backward;  //THROTTLE_State_Backward
                THROTTLE_Handle.R_ZeroAngle=THROTTLE_Handle.R_angle;
                THROTTLE_Handle.L_ZeroAngle=THROTTLE_Handle.L_angle;
            }
            else if(THROTTLE_Handle.State==THROTTLE_State_Forward)
            {
                K1G_ON;
                THROTTLE_Handle.State=THROTTLE_State_Zero;       //THROTTLE_State_Zero
                THROTTLE_Handle.R_Cal_ForwardAngle=THROTTLE_Handle.R_angle;
                THROTTLE_Handle.L_Cal_ForwardAngle=THROTTLE_Handle.L_angle;

            }
            else if(THROTTLE_Handle.State==THROTTLE_State_Backward)
            {
                LED_Handle.LED_All_Trigger_flag = 1;   //LED
                THROTTLE_Handle.R_Cal_BackwardAngle=THROTTLE_Handle.R_angle;
                THROTTLE_Handle.L_Cal_BackwardAngle=THROTTLE_Handle.L_angle;
                THROTTLE_Handle.State=THROTTLE_State_Cal_Stop;
                /*需要油门回零位*/
                THROTTLE_Handle.L_SetZero_Flag=0; 
                THROTTLE_Handle.R_SetZero_Flag=0;
                
            }  
        }
    } 
}
    
void KEY_Main_Handle(KEY_Handle_t* p)
{
    if(KEY1_STATUS == 0)
	{
        p->Open_Close_Key.Press_Count++; 
        p->Open_Close_Key.Action = KEY_Action_Push;
        p->Open_Close_Key.State = KEY_State_Pressing;  //按压中 
	}
    else
    {
        p->Open_Close_Key.Action = KEY_Action_Release; //释放了动作
        p->Open_Close_Key.State = KEY_State_Released;  //释放了状态
    }
	KEY_Get_Touch_Type(&p->Open_Close_Key);
    
    //开关机按键，按下开机
    if(Open_Close_Switch_Handle.Close_State==0)
    {
        if(THROTTLE_Handle.State == THROTTLE_State_Success&&THROTTLE_Handle.R_Percent==0&&THROTTLE_Handle.L_Percent==0)
        {   
             if(KEY3_STATUS == 0)
               {
                p->SYNC_Control_Key.Press_Count++; 
                p->SYNC_Control_Key.Action = KEY_Action_Push;
                p->SYNC_Control_Key.State  = KEY_State_Pressing;   //按压中

               }
               else
               {
                p->SYNC_Control_Key.Action = KEY_Action_Release;   //释放了动作
                p->SYNC_Control_Key.State  = KEY_State_Released;   //释放了

               }
               KEY_Get_Touch_Type(&p->SYNC_Control_Key);
            
               if(KEY2_STATUS == 0)
               {  
                p->DOCK_Key.Press_Count++; 
                p->DOCK_Key.Action = KEY_Action_Push;
                p->DOCK_Key.State=KEY_State_Pressing;  //按压中
               }
               else
               {
                p->DOCK_Key.Action = KEY_Action_Release;
                p->DOCK_Key.State=KEY_State_Released;  //释放了
               }
               KEY_Get_Touch_Type(&p->DOCK_Key);
            
               if(KEY4_STATUS == 0)
               {
                p->NORM_Key.Press_Count++; 
                p->NORM_Key.Action = KEY_Action_Push;
                p->NORM_Key.State=KEY_State_Pressing;  //按压中
               }
               else
               {
                p->NORM_Key.Action = KEY_Action_Release;
                p->NORM_Key.State=KEY_State_Released;  //释放了
               }
               KEY_Get_Touch_Type(&p->NORM_Key);  
        }
        else if(THROTTLE_Handle.State == THROTTLE_State_NeedReset||THROTTLE_Handle.State==THROTTLE_State_Zero||THROTTLE_Handle.State==THROTTLE_State_Forward||THROTTLE_Handle.State==THROTTLE_State_Backward)
        {
            if(KEY4_STATUS == 0)
           {
            p->NORM_Key.Press_Count++; 
            p->NORM_Key.Action = KEY_Action_Push;
            p->NORM_Key.State=KEY_State_Pressing;  //按压中
           }
           else
           {
            p->NORM_Key.Action = KEY_Action_Release;
            p->NORM_Key.State=KEY_State_Released;  //释放了

           }
           KEY_Get_Touch_Type(&p->NORM_Key);    
        }
    } 
}

//LED的函数
void LED_All_ON(void)
{      
   K1G_ON;     //ok       
   K2G_ON;     //ok
   K3G_ON;     //ok  
   K4G_ON;     //ok
}

void LED_All_OFF(void)
{     
   K1G_OFF;     //ok     
   K2G_OFF;     //ok
   K3G_OFF;     //ok  
   K4G_OFF;     //ok
}

void LED_All_G_OFF(void)
{
    K1G_OFF;     //ok
    K2G_OFF;     //ok
    K3G_OFF;     //ok
    K4G_OFF;     //ok
}


void LED_All_G_ON(void)
{
    K1G_ON;     //ok
    K2G_ON;     //ok
    K3G_ON;     //ok
    K4G_ON;     //ok
}


void LED_Three_G_ON(void)
{
    K1G_ON;       //ok
    K2G_ON;       //ok
    K3G_ON;       //ok
//    K4R_ON;     //ok
}
void LED_Three_G_OFF(void)
{
    K1G_OFF;      //ok
    K2G_OFF;      //ok
    K3G_OFF;      //ok
//    K4R_ON;     //ok
}

void LED_Two_G_ON(void)
{
    K1G_ON;       //ok
    K2G_ON;       //ok
//    K3R_ON;     //ok
//    K4R_ON;     //ok
}
void LED_Two_G_OFF(void)
{
    K1G_OFF;       //ok
    K2G_OFF;       //ok
//    K3R_OFF;     //ok
//    K4R_ON;      //ok
}
void LED_One_G_ON(void)
{
    K1G_ON;       //ok
//    K2R_ON;     //ok
//    K3R_ON;     //ok
//    K4R_ON;     //ok
}
void LED_One_G_OFF(void)
{
    K1G_OFF;       //ok
//    K2R_OFF;     //ok
//    K3R_OFF;     //ok
//    K4R_ON;     //ok
}

void LED_eerhT_G_ON(void)
{
//    K1R_ON;      //ok
    K2G_ON;        //ok
    K3G_ON;        //ok
    K4G_ON;        //ok
}
void LED_eerhT_G_OFF(void)
{
//    K1R_OFF;      //ok
    K2G_OFF;      //ok
    K3G_OFF;      //ok
    K4G_OFF;     //ok
}

void LED_owT_G_ON(void)
{
//    K1R_ON;       //ok
//    K2R_ON;       //ok
    K3G_ON;     //ok
    K4G_ON;     //ok
}
void LED_owT_G_OFF(void)
{
//    K1R_OFF;       //ok
//    K2R_OFF;       //ok
    K3G_OFF;     //ok
    K4G_OFF;      //ok
}
void LED_enO_G_ON(void)
{
//    K1R_ON;       //ok
//    K2R_ON;     //ok
//    K3R_ON;     //ok
    K4G_ON;     //ok
}
void LED_enO_G_OFF(void)
{
//    K1R_OFF;       //ok
//    K2R_OFF;     //ok
//    K3R_OFF;     //ok
    K4G_OFF;     //ok
}

void LED_Work(void)
{
    if(LED_Handle.LED_All_Trigger_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			LED_All_ON();
            if(++LED_Handle.LED_times>=2)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_All_Trigger_flag=0;
                LED_All_OFF();
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
           LED_All_OFF();
        }
		else
		{
			LED_All_ON();
		}
	}
    else if(LED_Handle.LED_Key1_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K1G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key1_flag=0;
                K1G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K1G_OFF;     //ok
        }
		else
		{
			K1G_ON;     //ok
		}
	}
    else if(LED_Handle.LED_Key2_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K2G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key2_flag=0;
                K2G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K2G_OFF;     //ok
        }
		else
		{
			K2G_ON;     //ok
		}
	}
    else if(LED_Handle.LED_Key4_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K4G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key4_flag=0;
                K4G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K4G_OFF;     //ok
        }
		else
		{
			K4G_ON;     //ok
		}
	}
    else if(LED_Handle.LED_Key2Key1_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K2G_ON;     //ok
            K1G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key2Key1_flag=0;
                K2G_OFF;     //ok
                K1G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K2G_OFF;     //ok
            K1G_OFF;     //ok
        }
		else
		{
			K2G_ON;     //ok
            K1G_ON;     //ok
		}
	}
     else if(LED_Handle.LED_Key2Key4_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K2G_ON;     //ok
            K4G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key2Key4_flag=0;
                K2G_OFF;     //ok
                K4G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K2G_OFF;     //ok
            K4G_OFF;     //ok
        }
		else
		{
			K2G_ON;     //ok
            K4G_ON;     //ok
		}
	}
    else if(LED_Handle.LED_Key1Key4_Trigger_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K1G_ON;     //ok
            K4G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key1Key4_Trigger_flag=0;
                K1G_OFF;     //ok
                K4G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K1G_OFF;     //ok
            K4G_OFF;     //ok
        }
		else
		{
			K1G_ON;     //ok
            K4G_ON;     //ok
		}
	}
    else if(LED_Handle.LED_Key1Key4Key3_Trigger_flag== 1)
	{
		LED_Handle.LED_All_cnt++;
		if(LED_Handle.LED_All_cnt>LED_TIME)        //控制时间10
		{
			LED_Handle.LED_All_cnt = 0;
			K1G_ON;     //ok
            K3G_ON;     //ok 
            K4G_ON;     //ok
            if(++LED_Handle.LED_times>=3)
            {
                LED_Handle.LED_times=0;
                LED_Handle.LED_Key1Key4Key3_Trigger_flag=0;
                K1G_OFF;     //ok
                K3G_OFF;     //ok
                K4G_OFF;     //ok
            }

		}
        else if(LED_Handle.LED_All_cnt<=(LED_TIME>>1))
        {
            K1G_OFF;     //ok
            K3G_OFF;     //ok
            K4G_OFF;     //ok
        }
		else
		{
			K1G_ON;     //ok
            K3G_ON;     //ok
            K4G_ON;     //ok
		}
	}








    else if(LED_Handle.LED_Three_Trigger_flag == 1)
	{
		LED_Handle.LED_Three_cnt++;
        if(LED_Handle.LED_Direction_flag==1)
        {
           if(LED_Handle.LED_Three_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_Three_cnt = 0;
			   LED_eerhT_G_ON();	
		   }
           else if(LED_Handle.LED_Three_cnt<=(LED_TIME>>1))
           {
               LED_eerhT_G_OFF();
           }
		   else
		   {
		       LED_eerhT_G_ON();
		   }
        }
        else
        {
           if(LED_Handle.LED_Three_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_Three_cnt = 0;
			   LED_Three_G_ON();	
		   }
           else if(LED_Handle.LED_Three_cnt<=(LED_TIME>>1))
           {
               LED_Three_G_OFF();
           }
		   else
		   {
		       LED_Three_G_ON();
		   }
        }
	}
	else if(LED_Handle.LED_Key3Key4_Trigger_flag == 1)
	{
		LED_Handle.LED_Two_cnt++;
           if(LED_Handle.LED_Two_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_Two_cnt = 0;
                K3G_ON;     //ok
                K4G_ON;     //ok
		   }
           else if(LED_Handle.LED_Two_cnt<=(LED_TIME>>1))
           {
                K3G_OFF;     //ok
                K4G_OFF;     //ok
           }
		   else
		   {
		        K3G_ON;     //ok
                K4G_ON;     //ok
		   }
        
	}
    else if(LED_Handle.LED_One_Trigger_flag == 1)
    {
        LED_Handle.LED_One_cnt++;
        if(LED_Handle.LED_Direction_flag==1)
        {
           if(LED_Handle.LED_One_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_One_cnt = 0;
			   LED_enO_G_ON();

		   }
           else if(LED_Handle.LED_One_cnt<=(LED_TIME>>1))
           {
               LED_enO_G_OFF();
            
           }
		   else
		   {
		       LED_enO_G_ON();
             
		   }
        }
        else
        {
           if(LED_Handle.LED_One_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_One_cnt = 0;
			   LED_One_G_ON();	
		   }
           else if(LED_Handle.LED_One_cnt<=(LED_TIME>>1))
           {
               LED_One_G_OFF();
           }
		   else
		   {
		       LED_One_G_ON();
		   }
        }
    }
     else if(LED_Handle.LED_Key4_Trigger_flag == 1)
    {
        LED_Handle.LED_One_cnt++;
     
           if(LED_Handle.LED_One_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_One_cnt = 0;
			   LED_enO_G_ON();

		   }
           else if(LED_Handle.LED_One_cnt<=(LED_TIME>>1))
           {
               LED_enO_G_OFF();
            
           }
		   else
		   {
		       LED_enO_G_ON();
             
		   }
        
    }
     else if(LED_Handle.LED_Key3_Trigger_flag == 1)
    {
        LED_Handle.LED_One_cnt++;
     
           if(LED_Handle.LED_One_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_One_cnt = 0;
               K3G_ON;     //ok
		   }
           else if(LED_Handle.LED_One_cnt<=(LED_TIME>>1))
           {
               K3G_OFF;     //ok
           }
		   else
		   {
               K3G_ON;     //ok        
		   }
        
    }
    else if(LED_Handle.LED_Key2_Trigger_flag == 1)
    {
        LED_Handle.LED_One_cnt++;
     
           if(LED_Handle.LED_One_cnt>LED_TIME)        //控制时间
		   {
			   LED_Handle.LED_One_cnt = 0;
               K2G_ON;     //ok
		   }
           else if(LED_Handle.LED_One_cnt<=(LED_TIME>>1))
           {
               K2G_OFF;     //ok
           }
		   else
		   {
               K2G_ON;     //ok        
		   }
        
    }
    
}





