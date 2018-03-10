#include "app_CH2_Action.h"

#define POWER_ON   0
#define POWER_OFF  1

#define Mid_Press_Value 7

//��ض˿ڶ���
//#define   BSP_POUT2_01       PGout(11)
//#define   BSP_POUT2_02       PGout(9)
//#define   BSP_POUT2_03       PDout(6)
//#define   BSP_POUT2_04       PDout(4)
//#define   BSP_POUT2_05       PFout(2)  //13
//#define   BSP_POUT2_06       PFout(0)  //14
//#define   BSP_POUT2_07       PCout(14) //15
//#define   BSP_POUT2_08       PEout(6)  //16
//CH2 Action����
void CH2_Action_Platen_IN_OUT()
{
    if(AppTask_Sensor_Status.F_x9 == 0 && AppTask_Sensor_Status.F_x11==1 && AppTask_Sensor_Status.F_x1 ==1){
      if(AppTask_Fixture_Status.F_CH2_PDFinish == 0 && AppTask_Sensor_Status.F_x14 ==1){
        if(AppTask_Fixture_Status.F_CH2_PendingDrop == 0){
          AppTask_Fixture_Status.F_CH2_START=1;
          AppTask_Fixture_Status.F_CH2_EXIT =0;
        }
      }  
      else if(AppTask_Fixture_Status.F_CH2_PDFinish == 1 ){
        if(AppTask_Fixture_Status.F_CH2_PendingRise == 0){
          AppTask_Fixture_Status.F_CH2_START=0;
          AppTask_Fixture_Status.F_CH2_EXIT=1;
        }
      }  
    }
}
//CH2������ť
void CH2_Singal_Step_Button(void)
{
    OS_ERR      err;
    
    if(AppTask_Sensor_Status.F_x10==1){
          OSTimeDlyHMSM( 0,    //�����Եĵ���AppTaskMotionControl
                        0,
                        0,
                        25,
                        OS_OPT_TIME_HMSM_STRICT,
                        &err); 
          if(AppTask_Sensor_Status.F_x10==1){  
                CH2_Action_Platen_IN_OUT();
          }   
      
    }
  
}
//ƽ̨�ƽ�
void CH2_Platform_IN(void)
{
    BSP_POUT2_01=POWER_ON;BSP_POUT2_02=POWER_OFF;
}
    
//ƽ̨�˳�
void CH2_Platform_OUT(void)
{
    BSP_POUT2_01=POWER_OFF;BSP_POUT2_02=POWER_ON;
}

//stop
void CH2_Platform_Stop(void)
{
    BSP_POUT2_01=POWER_OFF;BSP_POUT2_02=POWER_OFF;
}
//��VBUS 
void CH2_POWER_ON(void)
{
     BSP_SW2_VBUS=1;
}
//�ر�VBUS
void CH2_POWER_OFF(void)
{
     BSP_SW2_VBUS=0;
}
//BSP_SW2_VBUS
void CH2_FORCEDLD_OPEN(void)
{
    BSP_SW2_FLOAD=1;

}
//�ر�VBUS
void CH2_FORCEDLD_CLOSE(void)
{
    BSP_SW2_FLOAD=0;
  
}
    
//ǿ�Ƽ��أ�LEDָʾ
void CH2_PlatformLED_FLD(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT2_04=POWER_ON;
      }
      else{
        BSP_POUT2_04=POWER_OFF; 
      }
}

//LEDָʾ����
void CH2_PlatformLED_ERROR(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT2_03=POWER_ON;
      }
      else{
        BSP_POUT2_03=POWER_OFF; 
      }
}
//CH2����������
void CH2_CNT_Trigger(void)
{
      static CPU_INT08U CNT_Step=0;
        
      if(AppTask_Fixture_Status.F_CH2_CNT == 1){
          AppTask_Fixture_Status.F_CH2_CNT=0;
          CNT_Step=1;
      }
      //�����������
      switch(CNT_Step)
      {
            case 1:{   
              BSP_POUT2_05=POWER_ON;
              CH2_Timer_Out=50;
              CNT_Step=2;
            }break;
            
            case 2:{  
              if(CH2_Timer_Out <= 0){
                  BSP_POUT2_05=POWER_OFF;
                  CNT_Step=3;
              }
            }break;
      
      }

}
//CH2 �ٶ� HSpeed,MSpeed,LSpeed
//CPU_INT32U   CH2_Mtion_HDistance,CH2_Mtion_MDistance,CH2_Mtion_LDistance;
//CPU_INT08U   CH2_Mtion_HSpeed,   CH2_Mtion_MSpeed,   CH2_Mtion_LSpeed;
//ѹͷ��ѹ
//������Ҫ�ߵĸ����εľ���
//First_distance =(CH2_Mtion_HDistance/10)*SUBDIVISION/8;
//Second_distance=(CH2_Mtion_MDistance/10)*SUBDIVISION/8;
//Third_distance =(CH2_Mtion_LDistance/10)*SUBDIVISION/8;

void  CH2_Platen_Drop(void)
{
     CPU_INT32U First_distance,Second_distance,Third_distance;
     //����
     static CPU_INT08U CH2_Main_Step=0;
     
     //ʹ�ñ�־λ F_CH2_PDStart == 1 ,���� CH2_Main_Step=1;
     //ʹ�ñ�־λ F_CH2_PDFinish == 1 ��ʾԤѹ��λ
     if(AppTask_Fixture_Status.F_CH2_PDStart == 1){
        CH2_Main_Step=1;AppTask_Fixture_Status.F_CH2_PDStart=0;
     }
     if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1|| AppTask_Fixture_Status.F_CH2_UP_Reset ==1|| CH2_CurrentValue>CH2_SetMaxPress_Value){ //��ͣ�¼�
        CH2_Main_Step=0;
     }

     First_distance =(CH2_Mtion_HDistance/10)*SUBDIVISION/16;
     Second_distance= CH2_Mtion_MDistance;
     Third_distance = CH2_Mtion_LDistance;

     
     switch(CH2_Main_Step){
 //1--------------------------------------------------------------------------------------------------------------------------------------------         
           case 1:{
               CH2_Mtion_ABS_POS_Relative=First_distance;
               Motor2Starting_PWM2(DIR_CW,CH2_Mtion_HSpeed);
               CH2_Main_Step=2;
           }break;
           case 2:{
             
               if(CH2_CurrentValue > Mid_Press_Value+2){
                 MotorStop_PWM2(TIM4);
                 CH2_Main_Step=5;   //���ٹ�ѹ���������
               }
               
               if(CH2_Mtion_ABS_POS_Relative == 0 || CH2_CurrentValue > Mid_Press_Value-4){
                    MotorStop_PWM2(TIM4);
                    CH2_Main_Step=3;  //��������
               }

           }break;
//2--------------------------------------------------------------------------------------------------------------------------------------------           
           case 3:{
               CH2_Mtion_ABS_POS_Relative=Second_distance;
               Motor2Starting_PWM2(DIR_CW,CH2_Mtion_MSpeed);
               CH2_Main_Step=4;
           }break;
            case 4:{
                if(CH2_Mtion_ABS_POS_Relative == 0 || CH2_CurrentValue > Mid_Press_Value){
                    MotorStop_PWM2(TIM4); //�������
                    CH2_Main_Step=5;
                    CH2_Timer_Out=10;
                }
           }break;   
//3--------------------------------------------------------------------------------------------------------------------------------------------           
           case 5:{
             if(CH2_Timer_Out <= 0){
               CH2_Mtion_ABS_POS_Relative=Third_distance;
               Motor2Starting_PWM2(DIR_CW,CH2_Mtion_LSpeed);
               CH2_Main_Step=6;
             }
           }break;
           
           case 6:{
               //�½������ʵľ���
               if((CH2_Mtion_ABS_POS_Relative == 0) || (CH2_CurrentValue > Mid_Press_Value+2)){
                   MotorStop_PWM2(TIM4);
                   CH2_Timer_Out=5;
                   CH2_Main_Step=7;
               }
               
           }
           case 7:{
             if(CH2_Timer_Out <= 0){
                 if(CH2_CurrentValue > CH2_SetPress_Value+CH2_SetDev_Value){
                   CH2_Main_Step=8;   //����С�ξ���
                 }
                 
                 else if(CH2_CurrentValue < CH2_SetPress_Value){
                   CH2_Main_Step=5;   //�½�С�ξ���
                 }
                 
                 else{
                   //��ѹ��ָ����״̬
                   AppTask_Fixture_Status.F_CH2_CNT=1;
                   CH2_Counter++;
                   //����EEPRO
                   IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd120),&CH2_Counter);
                   
                   CH2_Main_Step=0;
                   MotorStop_PWM2(TIM4);
                   AppTask_Fixture_Status.F_CH2_PDFinish=1;
                   AppTask_Fixture_Status.F_CH2_PendingDrop =0;
                 }
             }
           }break;
           
           case 8:{
               //����С�ξ���
               CH2_Mtion_ABS_POS_Relative=2;
               Motor2Starting_PWM2(DIR_CCW,CH2_Mtion_LSpeed);
               CH2_Main_Step=9;
           }break;
           
           case 9:{
               //���������ʵľ���
               if((CH2_Mtion_ABS_POS_Relative == 0) | (CH2_CurrentValue < CH2_SetPress_Value)){
                     MotorStop_PWM2(TIM4);
                     CH2_Main_Step=7;
                 }
           }break;
     }  
     
     //������λ��
     if(AppTask_Sensor_Status.F_x13==1 && AppTask_Fixture_Status.F_CH2_PendingDrop == 1){
         //���ֹͣ
         CH2_Mtion_ABS_POS_Relative=0;
         MotorStop_PWM2(TIM4);
         
         //�������һ����ѹ
         CH2_Main_Step=0;
         AppTask_Fixture_Status.F_CH2_PDFinish=1;
         AppTask_Fixture_Status.F_CH2_PendingDrop =0;
         
         //��ӡ������Ϣ
         EXT1_BSP_Ser_Printf("Motion Out of range");   
     }
}

//ѹͷ�˳�
void  CH2_Platen_Rise(void)
{   
     CPU_INT32U Reset_distance;
     static CPU_INT08U Reset_Step=0;
     
     //��λ���� 
     if( AppTask_Fixture_Status.F_CH2_PDReset == 1){
        Reset_Step=1;AppTask_Fixture_Status.F_CH2_PDReset=0;
     }
     if(Reset_Step == 1){
        Reset_distance =(Reset_Distance_Max/10)*SUBDIVISION/16;
     }
     if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1){
        Reset_Step=0;
     }
     switch(Reset_Step)
     {
          case 1:{
                //����С�ξ���
                CH2_Mtion_ABS_POS_Relative=Reset_distance;
                Motor2Starting_PWM2(DIR_CCW,CH2_Mtion_ResetSpeed);
                Reset_Step=2;
          }break;
          
          case 2:{ 
                if(CH2_Mtion_ABS_POS_Relative == 0 | BSP_PIN2_05 == 0){
                   
                   AppTask_Sensor_Status.F_x12=1; //ԭ��
                   MotorStop_PWM2(TIM4);
                   Reset_Step=0;
                   
                   AppTask_Fixture_Status.F_CH2_PDFinish=0;
                   AppTask_Fixture_Status.F_CH2_PendingRise=0;
                }
                
          }break;      
     }
  
}

//һ����ѹ����
void CH2_Fixture_Action_IN(void)
{
    static CPU_INT08U CH2_Cy_Step=0;
      
    if(AppTask_Fixture_Status.F_CH2_START == 1){
        CH2_Cy_Step=1;
        AppTask_Fixture_Status.F_CH2_START=0;
        AppTask_Fixture_Status.F_CH2_PendingDrop=1;//����ִ����ѹ����
    }
    if(AppTask_Sensor_Status.F_x9 == 1|| AppTask_Sensor_Status.F_x17 == 1 || AppTask_Fixture_Status.F_CH2_UP_Reset ==1){
      CH2_Cy_Step=0;
    }
    switch(CH2_Cy_Step){
          case 1: {
              CH2_Platform_IN();
              CH2_Cy_Step=2;
              CH2_Timer_Out=500;
          }break;
          
          case 2: {
            
            if((AppTask_Sensor_Status.F_x15==1)&&(CH2_Timer_Out>0)){
                CH2_Cy_Step=3;
                CH2_Timer_Out=120; //��ʱ1200ms
            }
            else if(CH2_Timer_Out == 0){
               //����,���ײ���λ
                CH2_PlatformLED_ERROR(LED_OPEN);
                CH2_Cy_Step=0;
            }
          }break;
      
          //AppTask_Fixture_Status.F_CH2_PDStart;
          case 3: {
                if(CH2_Timer_Out == 0){
                  //��ʱ500ms��������ѹ
                  AppTask_Fixture_Status.F_CH2_PDStart =1;
                  CH2_Cy_Step=4;
                }
        
          }break;
    }
  
}

//һ���˳�����
void CH2_Fixture_Action_OUT(void)
{
    static CPU_INT08U CH2_Cy_ResetStep=0;
      
    if(AppTask_Fixture_Status.F_CH2_EXIT == 1){
        CH2_Cy_ResetStep=1;
        AppTask_Fixture_Status.F_CH2_EXIT=0;
        AppTask_Fixture_Status.F_CH2_PendingRise=1; //����ִ���˳� 
    }
    if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1){
      CH2_Cy_ResetStep=0;
    }
    
    switch(CH2_Cy_ResetStep){
      
           case 1: {
                  AppTask_Fixture_Status.F_CH2_PDReset =1;
                  CH2_Cy_ResetStep=2;
           }break;    
      
          case 2: {
                  if(AppTask_Fixture_Status.F_CH2_PendingRise == 0 && BSP_PIN2_05 == 0){
                      CH2_Platform_OUT();
                      CH2_Cy_ResetStep=3;
                      CH2_Timer_Out=500; //��ʱ���ʱ����5s
                  }
          }break;
          
          case 3: {
            //���������λ
            if((AppTask_Sensor_Status.F_x14 == 1)&&(CH2_Timer_Out>0)){
                CH2_Timer_Out=50; //��ʱ500ms
                CH2_Cy_ResetStep=4;
            }
//            else if(CH2_Timer_Out == 0){
//               //����,���ײ���λ
//                CH2_PlatformLED_ERROR(LED_OPEN);
//                CH2_Cy_ResetStep=0;
//            }
          
          }break;
      
          //AppTask_Fixture_Status.F_CH2_PDStart;
          case 4: {
                if(CH2_Timer_Out == 0){
                  //��ʱ500ms��������ѹ
                  //AppTask_Fixture_Status.F_CH2_PDReset =1;
                  CH2_Cy_ResetStep=5;
                }
        
          }break;
    }
}
//����豸�쳣��Ϣ
//AppTask_Fixture_Status.F_CH2_PendingRise == 1;  //������
//AppTask_Fixture_Status.F_CH2_PendingDrop == 1;  //�½���
//AppTask_Fixture_Status.F_CH2_PDFinish == 1;     //���
void Check_Fixture_CH2_AbnormalInfo(void)
{
      //����쳣���� CH2_Mtion_ABS_POS_Relative,CH2_Mtion_ABS_POS_Relative,CH3_Mtion_ABS_POS_Relative,CH4_Mtion_ABS_POS_Relative
      if(CH2_Mtion_ABS_POS_Relative!=0 && BSP_DIR2 == DIR_CCW && AppTask_Sensor_Status.F_x12 ==1){
        //����ֹͣ������� TIM4
        MotorStop_PWM2(TIM4);CH2_Mtion_ABS_POS_Relative=0;
        
      }
      //CH2�˶����̼���Ʒ�Ƿ���λ�� == 1��ʾ��λ�� F_CH2_SPInSitu,F_CH2_SPInSitu,F_CH3_SPInSitu,F_CH4_SPInSitu
      if(AppTask_Fixture_Status.F_CH2_PendingRise == 1 && AppTask_Fixture_Status.F_CH2_PendingDrop == 1 && AppTask_Fixture_Status.F_CH2_PDFinish == 1)
      {
          if(AppTask_Sensor_Status.F_x11 == 1){
            AppTask_Fixture_Status.F_CH2_SPInSitu=1;
            CH2_PlatformLED_ERROR(LED_CLOSE);
          }else{
            AppTask_Fixture_Status.F_CH2_SPInSitu=0;
            CH2_PlatformLED_ERROR(LED_OPEN);
            
          }
      } 
}

//��λ�� �����־λ
void Clear_CH2_Flag(void)
{
      AppTask_Fixture_Status.F_CH2_PendingDrop = 0;    
      AppTask_Fixture_Status.F_CH2_PendingRise = 0;     
      AppTask_Fixture_Status.F_CH2_PDFinish = 0;
}

//�豸ͨ����״̬��Ϣ
void Fixture_CH2_StatuesInfo(void)
{
    
    if(AppTask_Sensor_Status.F_x17 == 0 && AppTask_Sensor_Status.F_x9 == 0 ){
          //��ѹ
          if(AppTask_Fixture_Status.F_CH2_UP_Drop == 1 && AppTask_Fixture_Status.F_CH2_PDFinish == 1){
            AppTask_Fixture_Status.F_CH2_UP_Drop=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //�˳�
          if(AppTask_Fixture_Status.F_CH2_UP_Rise == 1&& AppTask_Sensor_Status.F_x14 == 1 && AppTask_Sensor_Status.F_x12 ==1){
            AppTask_Fixture_Status.F_CH2_UP_Rise=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //��λ
          if(AppTask_Fixture_Status.F_CH2_UP_Reset == 1 && AppTask_Sensor_Status.F_x14 == 1){
            AppTask_Fixture_Status.F_CH2_UP_Reset=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");
            Clear_CH2_Flag();
          }
          
    }  
      
}

//ͨ������ָ�����
CPU_INT08U CH2_UartCMD_Analyze(void)
{
    //    CPU_INT16S   *LPtr_RegData=RegData;
        
        if(Str_Cmp((const CPU_CHAR*)"02 ACTION=CLOSE",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH2_UP_Drop=1;//׼����ӡ
              if(AppTask_Fixture_Status.F_CH2_PendingDrop == 0 && AppTask_Fixture_Status.F_CH2_PDFinish == 0){
                CH2_Action_Platen_IN_OUT();
              }
              return 0;
        }  
        
        else if(Str_Cmp((const CPU_CHAR*)"02 ACTION=OPEN",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH2_UP_Rise=1; //׼����ӡ
              if(AppTask_Fixture_Status.F_CH2_PendingRise == 0 && AppTask_Fixture_Status.F_CH2_PDFinish == 1){
                CH2_Action_Platen_IN_OUT();  
              }
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 ACTION=RESET",(const CPU_CHAR*)receBuf)==0){
              //02 ͨ����λ
              AppTask_Fixture_Status.F_CH2_UP_Reset =1;
              CH2_POWER_OFF(); //�ر�Vbus
              CH2_FORCEDLD_CLOSE();//�ر�ǿ�Ƽ���
              CH2_PlatformLED_FLD(LED_CLOSE);//�ر�fld LED
              CH2_PlatformLED_ERROR(LED_CLOSE); //�ر�errorָʾ��
              
              if(AppTask_Fixture_Status.F_CH2_PendingRise == 0){
                
                AppTask_Fixture_Status.F_CH2_START=0;    //�˳�
                AppTask_Fixture_Status.F_CH2_EXIT=1;
                
              }
              return 0;
        }

        else if(Str_Cmp((const CPU_CHAR*)"02 POWER=ON",(const CPU_CHAR*)receBuf)==0){
              //�л�usb on
              AppTask_Fixture_Status.F_CH2_UP_Vbus=1;
              CH2_POWER_ON();
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 POWER=OFF",(const CPU_CHAR*)receBuf)==0){
              //�л�usb off
              CH2_POWER_OFF();
              AppTask_Fixture_Status.F_CH2_UP_Vbus=0;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 POWER?",(const CPU_CHAR*)receBuf)==0){
              //��ѯ�л���־λ
              if(AppTask_Fixture_Status.F_CH2_UP_Vbus == 1)
              { //�ϵ�״̬
                EXT1_BSP_Ser_Printf("\r\n02 POWER ON\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }else{
                //û���ϵ�״̬
                EXT1_BSP_Ser_Printf("\r\n02 POWER OFF\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              return 0;
        }
        
        //ǿ������02 action=forcedld_open
        else if(Str_Cmp((const CPU_CHAR*)"02 ACTION=FORCEDLD_OPEN",(const CPU_CHAR*)receBuf)==0){
              CH2_FORCEDLD_OPEN();
              CH2_PlatformLED_FLD(LED_OPEN);
              AppTask_Fixture_Status.F_CH2_UP_FLoad=1;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 ACTION=FORCEDLD_CLOSE",(const CPU_CHAR*)receBuf)==0){
              CH2_FORCEDLD_CLOSE();
              CH2_PlatformLED_FLD(LED_CLOSE); 
              AppTask_Fixture_Status.F_CH2_UP_FLoad=0;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;          
        } 
                                  
        else if(Str_Cmp((const CPU_CHAR*)"02 RESULT=PASS",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              //CH2_PlatformLED_ERROR(LED_OPEN);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 RESULT=FAIL",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              // CH2_PlatformLED_ERROR(LED_CLOSE);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"02 KDPPWR=ON",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        else if(Str_Cmp((const CPU_CHAR*)"02 KDPPWR=OFF",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        //��ӡ����
        else if(Str_Cmp((const CPU_CHAR*)"02 TSTCNT",(const CPU_CHAR*)receBuf)==0){
              EXT1_BSP_Ser_Printf("\r\n%d\r\n",CH2_Counter);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //��������
        else if(Str_Cmp((const CPU_CHAR*)"02 BUTTON=LOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH2_UP_LB=1; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              
              return 0;
        }	
        //�ͷŰ���
        else if(Str_Cmp((const CPU_CHAR*)"02 BUTTON=UNLOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH2_UP_LB=0; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //�о�״̬
        else if(Str_Cmp((const CPU_CHAR*)"02 DUT STAT",(const CPU_CHAR*)receBuf)==0){
              if(AppTask_Fixture_Status.F_CH2_PDFinish ==1 && AppTask_Sensor_Status.F_x3 ==1){
                //READING
                EXT1_BSP_Ser_Printf("\r\nREADY\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              else if(AppTask_Fixture_Status.F_CH2_PDFinish ==0 && AppTask_Sensor_Status.F_x3 ==1){
                //WAITING
                EXT1_BSP_Ser_Printf("\r\nWAITTING\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              else {
                //UNDETECTED
                EXT1_BSP_Ser_Printf("\r\nUNDETECTED\r\n"); 
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              
              return 0;
        }
        
        //��ǰ��֧��ѹ��������� CH2_CurrentValue
        else if(Str_Cmp((const CPU_CHAR*)"02 TPRESS=?",(const CPU_CHAR*)receBuf)==0){
          
              EXT1_BSP_Ser_Printf("\r\nTPRESS: 1\r\n\r\nTPRESS: 1,\"PRESS\"\r\n",CH2_CurrentValue);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //��ȡѹ�������ֵ
        else if(Str_Cmp_N((const CPU_CHAR*)"02 TPRESS=1,0,",(const CPU_CHAR*)receBuf,14)==0){
              EXT1_BSP_Ser_Printf("\r\n%.1f\r\n",CH2_CurrentValue);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //�趨CH2ѹ��������ֵ
        else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"02 TPRESS=1,1,",14) == 0){
              CH2_SetMaxPress_Value = 1.0 * Conversion_fraction("02 TPRESS=1,1,",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd134),20);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }						
        
        
        else if(Str_Cmp((const CPU_CHAR*)"02 FIXTURE STAT",(const CPU_CHAR*)receBuf)==0){
          
              EXT1_BSP_Ser_Printf("\r\nRUNSTAT:\r\n");   
              //����
              
              EXT1_BSP_Ser_Printf("\r\nMOTORLIFE:\r\n");
              
              EXT1_BSP_Ser_Printf("\r\nBAROINFO:0.5\r\n");
              
              EXT1_BSP_Ser_Printf("\r\nGRATING:");
              
              if(AppTask_Sensor_Status.F_x9 == 1){
                EXT1_BSP_Ser_Printf("ABNORMAL\r\n");  
              }else{
                EXT1_BSP_Ser_Printf("NORMAL\r\n");
              }
              
              EXT1_BSP_Ser_Printf("\r\nESTOP:");
              //ABNORMAL
              if(AppTask_Sensor_Status.F_x17 == 1){
                EXT1_BSP_Ser_Printf("ABNORMAL\r\n");  
              }else{
                EXT1_BSP_Ser_Printf("NORMAL\r\n");
              }
              
              return 0;
         }  
  
         return 0;  
  
}
