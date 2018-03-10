#include "app_action_ctl.h"



//�ϵ��о߲�״̬����λ�豸
void PowerOn_Test_AND_ResetFixture(void)
{
          OS_ERR      err;
          static CPU_INT08U  Motor_F_Reset;
          static CPU_INT08U  Fixture_StaRT,Fixture_StaER;
          
          CH1_Mtion_ABS_POS_Relative=(Reset_Distance_Max/10)*SUBDIVISION/16;;
          Motor1Starting_PWM1(DIR_CCW,CH1_Mtion_ResetSpeed);

          CH2_Mtion_ABS_POS_Relative=(Reset_Distance_Max/10)*SUBDIVISION/16;;
          Motor2Starting_PWM2(DIR_CCW,CH2_Mtion_ResetSpeed);
          
          CH3_Mtion_ABS_POS_Relative=(Reset_Distance_Max/10)*SUBDIVISION/16;;
          Motor3Starting_PWM3(DIR_CCW,CH3_Mtion_ResetSpeed);
          
          CH4_Mtion_ABS_POS_Relative=(Reset_Distance_Max/10)*SUBDIVISION/16;;
          Motor4Starting_PWM4(DIR_CCW,CH4_Mtion_ResetSpeed);
          //!(AppTask_Sensor_Status.F_x6 == 1 | AppTask_Sensor_Status.F_x14 == 1 | AppTask_Sensor_Status.F_x22 == 1 | AppTask_Sensor_Status.F_x30 == 1)
          Motor_F_Reset=0;
          while(Motor_F_Reset == 0){
             Press_DATA_Handling();
            if(GuangShan_Event == 2 && AppTask_Sensor_Status.F_x9 ==1){
                    CH1_Mtion_ABS_POS_Relative=0;
                    MotorStop_PWM1(TIM3);
                    CH2_Mtion_ABS_POS_Relative=0;
                    MotorStop_PWM2(TIM4);
                    CH3_Mtion_ABS_POS_Relative=0;
                    MotorStop_PWM3(TIM2);
                    CH4_Mtion_ABS_POS_Relative=0;
                    MotorStop_PWM4(TIM5);
                    //��ŷ�ֹͣ״̬
                    CH1_Platform_Stop();
                    CH2_Platform_Stop();
                    CH3_Platform_Stop();
                    CH4_Platform_Stop();
                    Motor_F_Reset=1;
            }else{
                if(CH1_Mtion_ABS_POS_Relative ==0 || BSP_PIN1_05 == 0)  { MotorStop_PWM1(TIM3);CH1_Mtion_ABS_POS_Relative=0;CH1_Platform_OUT();}
                if(CH2_Mtion_ABS_POS_Relative ==0 || BSP_PIN2_05 == 0)  { MotorStop_PWM2(TIM4);CH2_Mtion_ABS_POS_Relative=0;CH2_Platform_OUT();}
                if(CH3_Mtion_ABS_POS_Relative ==0 || BSP_PIN3_05 == 0)  { MotorStop_PWM3(TIM2);CH3_Mtion_ABS_POS_Relative=0;CH3_Platform_OUT();}
                if(CH4_Mtion_ABS_POS_Relative ==0 || BSP_PIN4_05 == 0)  { MotorStop_PWM4(TIM5);CH4_Mtion_ABS_POS_Relative=0;CH4_Platform_OUT();}
            }
          
            Fixture_StaRT = (BSP_PIN1_05 ==0 && BSP_PIN2_05 == 0&& BSP_PIN3_05 == 0 && BSP_PIN4_05 ==0);
            Fixture_StaER = (CH1_Mtion_ABS_POS_Relative ==0 && CH2_Mtion_ABS_POS_Relative ==0 && CH3_Mtion_ABS_POS_Relative ==0 &&CH4_Mtion_ABS_POS_Relative ==0);
           
            //����Ƿ�λ
            if(Fixture_StaRT ==1){ 
                if(BSP_PIN1_07 == 0 && BSP_PIN2_07 == 0 && BSP_PIN3_07 == 0 && BSP_PIN4_07 == 0)
                { 
                   Motor_F_Reset=1;      //��������־
                   AppTask_Fixture_Status.F_FixResetErr=0;
                }
            }else if(Fixture_StaRT == 0 && Fixture_StaER ==1 && AppTask_Fixture_Status.F_FixResetTimerout == 0){ //����г̲��������� �������� ���� �����������
                //����Ƿ�ʱ
                AppTask_Fixture_Status.F_FixResetTimerout = 1;
                Reset_Timer_Out=6000;   
            }//����һ���Ӻ��쳣��λ���
            //����쳣��λ״��
            if(AppTask_Fixture_Status.F_FixResetTimerout ==1 && Reset_Timer_Out == 0 ){
                AppTask_Fixture_Status.F_FixResetTimerout = 0;
                Reset_Timer_Out=50;
                AppTask_Fixture_Status.F_FixResetErr=1;
                Motor_F_Reset=1;
            }
            OSTimeDlyHMSM( 0,    //�����Եĵ���AppTaskMotionControl
                          0,
                          0,
                          10,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);
            
            //��ͣʱ�䴦�����
            Stop_Event=2;            
           }
}

//ˢ��LCD1602��ʾ
void Display_LCD1602(void)
{ 
      static CPU_INT08S  BUF_DIS1[11],BUF_DIS2[11],BUF_DIS3[11],BUF_DIS4[11];
      
      if(CH1_CurrentValue>999){CH1_CurrentValue=999;}
      if(CH2_CurrentValue>999){CH2_CurrentValue=999;}
      if(CH3_CurrentValue>999){CH3_CurrentValue=999;}
      if(CH4_CurrentValue>999){CH4_CurrentValue=999;}
             
      EXT2_BSP_LCD1602_Printf(BUF_DIS1,"%.1fN",CH1_CurrentValue);
      EXT2_BSP_LCD1602_Printf(BUF_DIS2,"%.1fN",CH2_CurrentValue);
      EXT2_BSP_LCD1602_Printf(BUF_DIS3,"%.1fN",CH3_CurrentValue);
      EXT2_BSP_LCD1602_Printf(BUF_DIS4,"%.1fN",CH4_CurrentValue);
      
      Str_Cat((CPU_CHAR*)BUF_DIS1,(const CPU_CHAR*)"   "); 
      Str_Cat((CPU_CHAR*)BUF_DIS2,(const CPU_CHAR*)"   ");
      Str_Cat((CPU_CHAR*)BUF_DIS3,(const CPU_CHAR*)"   "); 
      Str_Cat((CPU_CHAR*)BUF_DIS4,(const CPU_CHAR*)"   ");
       
      //ʱ��Ϊ360us
      Reflesh_number++;
      LCD_Write_String(6,1,(CPU_INT08U*)BUF_DIS1,(CPU_INT08U*)BUF_DIS2,(CPU_INT08U*)BUF_DIS3,(CPU_INT08U*)BUF_DIS4);
}

//ִ��ѹ������ת��
void Press_DATA_Handling(void)
{
        //CH1 ����ѹ������ ת��
        if(ChannelADC_Value[0]<=CH1_CalValueAD_Zero){
          CH1_CurrentValue = 0;
        }else if(ChannelADC_Value[0]>CH1_CalValueAD_Zero){
          CH1_CurrentValue =(CH1_SetValueAD_5KG-CH1_SetValueAD_Zero)*(ChannelADC_Value[0]-CH1_CalValueAD_Zero)/(CH1_CalValueAD_5KG-CH1_CalValueAD_Zero)+CH1_SetValueAD_Zero;
          CH1_CurrentValue=0.098*CH1_CurrentValue;       
        }
        
        //CH2 ����ѹ������ ת��
        if(ChannelADC_Value[1]<=CH2_CalValueAD_Zero){
          CH2_CurrentValue = 0;
        }else if(ChannelADC_Value[1]>CH2_CalValueAD_Zero){
          CH2_CurrentValue =(CH2_SetValueAD_5KG-CH2_SetValueAD_Zero)*(ChannelADC_Value[1]-CH2_CalValueAD_Zero)/(CH2_CalValueAD_5KG-CH2_CalValueAD_Zero)+CH2_SetValueAD_Zero;
          CH2_CurrentValue =0.098*CH2_CurrentValue;    
        }                 
        
        //CH3 ����ѹ������ ת��
        if(ChannelADC_Value[2]<=CH3_CalValueAD_Zero){
          CH3_CurrentValue = 0;
        }else if(ChannelADC_Value[2]>CH3_CalValueAD_Zero){
          CH3_CurrentValue =(CH3_SetValueAD_5KG-CH3_SetValueAD_Zero)*(ChannelADC_Value[2]-CH3_CalValueAD_Zero)/(CH3_CalValueAD_5KG-CH3_CalValueAD_Zero)+CH3_SetValueAD_Zero;
          CH3_CurrentValue =0.098*CH3_CurrentValue;
        }                  
        
        //CH4 ����ѹ������ ת�� 
        if(ChannelADC_Value[3]<=CH4_CalValueAD_Zero){
          CH4_CurrentValue = 0;
        }else if(ChannelADC_Value[3]>CH4_CalValueAD_Zero){
          CH4_CurrentValue =(CH4_SetValueAD_5KG-CH4_SetValueAD_Zero)*(ChannelADC_Value[3]-CH4_CalValueAD_Zero)/(CH4_CalValueAD_5KG-CH4_CalValueAD_Zero)+CH4_SetValueAD_Zero;
          CH4_CurrentValue =0.098*CH4_CurrentValue;
        }   
}

//�����־λ
void Clear_ALL_Flag(void)
{
      AppTask_Fixture_Status.F_CH1_PendingDrop=0;
      AppTask_Fixture_Status.F_CH2_PendingDrop=0;
      AppTask_Fixture_Status.F_CH3_PendingDrop=0;
      AppTask_Fixture_Status.F_CH4_PendingDrop=0;
      
      AppTask_Fixture_Status.F_CH1_PendingRise=0;
      AppTask_Fixture_Status.F_CH2_PendingRise=0;
      AppTask_Fixture_Status.F_CH3_PendingRise=0;
      AppTask_Fixture_Status.F_CH4_PendingRise=0;
      
      AppTask_Fixture_Status.F_CH1_PDFinish = 0 ;
      AppTask_Fixture_Status.F_CH2_PDFinish = 0 ;
      AppTask_Fixture_Status.F_CH3_PDFinish = 0 ;
      AppTask_Fixture_Status.F_CH4_PDFinish = 0 ;

      CH1_PlatformLED_ERROR(LED_CLOSE);
      CH2_PlatformLED_ERROR(LED_CLOSE);
      CH3_PlatformLED_ERROR(LED_CLOSE);
      CH4_PlatformLED_ERROR(LED_CLOSE);
      
      CH1_POWER_OFF();
      CH2_POWER_OFF();
      CH3_POWER_OFF();
      CH4_POWER_OFF();
      
      CH1_FORCEDLD_CLOSE();
      CH2_FORCEDLD_CLOSE();
      CH3_FORCEDLD_CLOSE();
      CH4_FORCEDLD_CLOSE();
      
      CH1_PlatformLED_FLD(LED_CLOSE);
      CH2_PlatformLED_FLD(LED_CLOSE);
      CH3_PlatformLED_FLD(LED_CLOSE);
      CH4_PlatformLED_FLD(LED_CLOSE);     
      
}
                                                             
//��������ֵ����
void Protect_Max_Press(void)
{
      //��ѹ����
      if(AppTask_Fixture_Status.F_CH1_PendingDrop == 1 && CH1_CurrentValue>CH1_SetMaxPress_Value){
          AppTask_Fixture_Status.F_CH1_PDFinish = 1;
          AppTask_Fixture_Status.F_CH1_PendingDrop =0;
          CH1_Mtion_ABS_POS_Relative=0;
          MotorStop_PWM1(TIM3);
          CH1_Action_Platen_IN_OUT();
          CH1_PlatformLED_ERROR(LED_OPEN);
      }
      
      if(AppTask_Fixture_Status.F_CH2_PendingDrop == 1 && CH2_CurrentValue>CH2_SetMaxPress_Value){
          AppTask_Fixture_Status.F_CH2_PDFinish = 1;
          AppTask_Fixture_Status.F_CH2_PendingDrop =0;
          CH2_Mtion_ABS_POS_Relative=0;
          MotorStop_PWM2(TIM4);
          CH2_Action_Platen_IN_OUT(); 
          CH2_PlatformLED_ERROR(LED_OPEN);
      }
      
      if(AppTask_Fixture_Status.F_CH3_PendingDrop == 1 && CH3_CurrentValue>CH3_SetMaxPress_Value){
          AppTask_Fixture_Status.F_CH3_PDFinish = 1;
          AppTask_Fixture_Status.F_CH3_PendingDrop =0;
          CH3_Mtion_ABS_POS_Relative=0;
          MotorStop_PWM3(TIM2);
          CH3_Action_Platen_IN_OUT(); 
          CH3_PlatformLED_ERROR(LED_OPEN);
      }
      
      if(AppTask_Fixture_Status.F_CH4_PendingDrop == 1 && CH4_CurrentValue>CH4_SetMaxPress_Value){
          AppTask_Fixture_Status.F_CH4_PDFinish = 1;
          AppTask_Fixture_Status.F_CH4_PendingDrop =0;
          CH4_Mtion_ABS_POS_Relative=0;
          MotorStop_PWM4(TIM5);
          CH4_Action_Platen_IN_OUT(); 
          CH4_PlatformLED_ERROR(LED_OPEN);
      }
  
}