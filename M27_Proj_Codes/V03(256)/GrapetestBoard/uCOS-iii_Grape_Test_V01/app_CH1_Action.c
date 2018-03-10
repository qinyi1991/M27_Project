#include "app_CH1_Action.h"

#define POWER_ON   0
#define POWER_OFF  1

#define Mid_Press_Value 7

//��ض˿ڶ���
//#define   BSP_POUT1_01       PDout(5) 
//#define   BSP_POUT1_02       PDout(7)
//#define   BSP_POUT1_03       PGout(10)
//#define   BSP_POUT1_04       PGout(12)
//#define   BSP_POUT1_05       PGout(14)
//#define   BSP_POUT1_06       PBout(3)
//#define   BSP_POUT1_07       PGout(15)
//#define   BSP_POUT1_08       PGout(13)

void CH1_Action_Platen_IN_OUT()
{
      if(AppTask_Sensor_Status.F_x9 == 0 && AppTask_Sensor_Status.F_x3==1 && AppTask_Sensor_Status.F_x1 ==1){
        if(AppTask_Fixture_Status.F_CH1_PDFinish == 0 && AppTask_Sensor_Status.F_x6 ==1){
            if(AppTask_Fixture_Status.F_CH1_PendingDrop == 0){ //������ѹ
              AppTask_Fixture_Status.F_CH1_START=1;     //����
              AppTask_Fixture_Status.F_CH1_EXIT =0;
            }
        }   
        else if(AppTask_Fixture_Status.F_CH1_PDFinish == 1 ){
            if(AppTask_Fixture_Status.F_CH1_PendingRise == 0){ //��������
              AppTask_Fixture_Status.F_CH1_START=0;    //�˳�
              AppTask_Fixture_Status.F_CH1_EXIT=1;
            }
        }
      }
}

//CH1������ť
void CH1_Singal_Step_Button(void)
{
    OS_ERR      err;
    
    if(AppTask_Sensor_Status.F_x2==1){
          OSTimeDlyHMSM( 0,    //�����Եĵ���AppTaskMotionControl
                        0,
                        0,
                        25,
                        OS_OPT_TIME_HMSM_STRICT,
                        &err); 
          if(AppTask_Sensor_Status.F_x2==1){ 
               CH1_Action_Platen_IN_OUT();
          }   
      
    }
  
}

//ƽ̨�ƽ�
void CH1_Platform_IN(void)
{
    BSP_POUT1_01=POWER_ON;BSP_POUT1_02=POWER_OFF;
}
    
//ƽ̨�˳�
void CH1_Platform_OUT(void)
{
    BSP_POUT1_01=POWER_OFF;BSP_POUT1_02=POWER_ON;
}

//stop
void CH1_Platform_Stop(void)
{
    BSP_POUT1_01=POWER_OFF;BSP_POUT1_02=POWER_OFF;
}

//��VBUS 
void CH1_POWER_ON(void)
{
     BSP_SW1_VBUS=1;
}
//�ر�VBUS
void CH1_POWER_OFF(void)
{
     BSP_SW1_VBUS=0;
}
//BSP_SW1_VBUS
void CH1_FORCEDLD_OPEN(void)
{
    BSP_SW1_FLOAD=1;

}
//�ر�VBUS
void CH1_FORCEDLD_CLOSE(void)
{
    BSP_SW1_FLOAD=0;
  
}
    
//ǿ�Ƽ���ָʾLED
void CH1_PlatformLED_FLD(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT1_04=POWER_ON;
      }
      else{
        BSP_POUT1_04=POWER_OFF; 
      }
}

//LEDָʾ����
void CH1_PlatformLED_ERROR(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT1_03=POWER_ON;
      }
      else{
        BSP_POUT1_03=POWER_OFF; 
      }
}

//����������
void CH1_CNT_Trigger(void)
{
      static CPU_INT08U CNT_Step=0;
        
      if(AppTask_Fixture_Status.F_CH1_CNT == 1){
          AppTask_Fixture_Status.F_CH1_CNT=0;
          CNT_Step=1;
      }
      //�����������
      switch(CNT_Step)
      {
            case 1:{   
              BSP_POUT1_05=POWER_ON;
              CH1_Timer_Out=50;
              CNT_Step=2;
            }break;
            
            case 2:{  
              if(CH1_Timer_Out <= 0){
                  BSP_POUT1_05=POWER_OFF;
                  CNT_Step=3;
              }
            }break;
      
      }

}

//CH1 �ٶ� HSpeed,MSpeed,LSpeed
//CPU_INT32U   CH1_Mtion_HDistance,CH1_Mtion_MDistance,CH1_Mtion_LDistance;
//CPU_INT08U   CH1_Mtion_HSpeed,   CH1_Mtion_MSpeed,   CH1_Mtion_LSpeed;
//ѹͷ��ѹ
//������Ҫ�ߵĸ����εľ���
//First_distance =(CH1_Mtion_HDistance/10)*SUBDIVISION/8;
//Second_distance=(CH1_Mtion_MDistance/10)*SUBDIVISION/8;
//Third_distance =(CH1_Mtion_LDistance/10)*SUBDIVISION/8;

void  CH1_Platen_Drop(void)
{
     CPU_INT32U First_distance,Second_distance,Third_distance;
     //����
     static CPU_INT08U CH1_Main_Step=0;
     
     //ʹ�ñ�־λ F_CH1_PDStart == 1 ,���� CH1_Main_Step=1;
     //ʹ�ñ�־λ F_CH1_PDFinish == 1 ��ʾԤѹ��λ
     if(AppTask_Fixture_Status.F_CH1_PDStart == 1){
        CH1_Main_Step=1;AppTask_Fixture_Status.F_CH1_PDStart=0;
     }
     if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1 || AppTask_Fixture_Status.F_CH1_UP_Reset ==1 || CH1_CurrentValue>CH1_SetMaxPress_Value){ //guangshan�¼�
        CH1_Main_Step=0;
     }
 
     First_distance =(CH1_Mtion_HDistance/10)*SUBDIVISION/16;
     Second_distance= CH1_Mtion_MDistance;
     Third_distance = CH1_Mtion_LDistance;

     switch(CH1_Main_Step){
 //1--------------------------------------------------------------------------------------------------------------------------------------------         
           case 1:{
               CH1_Mtion_ABS_POS_Relative=First_distance;
               Motor1Starting_PWM1(DIR_CW,CH1_Mtion_HSpeed);
               CH1_Main_Step=2;
           }break;
           case 2:{
               
               if(CH1_CurrentValue > Mid_Press_Value+2){
                 MotorStop_PWM1(TIM3);
                 CH1_Main_Step=5;     //ѹ��ֵ���壬�������
               }
               if(CH1_Mtion_ABS_POS_Relative == 0 || CH1_CurrentValue > Mid_Press_Value-4){
                    MotorStop_PWM1(TIM3);
                    CH1_Main_Step=3;  //��������
               }
               
           }break;
//2--------------------------------------------------------------------------------------------------------------------------------------------           
           case 3:{
               CH1_Mtion_ABS_POS_Relative=Second_distance;
               Motor1Starting_PWM1(DIR_CW,CH1_Mtion_MSpeed);
               CH1_Main_Step=4;
           }break;
            case 4:{
                if(CH1_Mtion_ABS_POS_Relative == 0 || CH1_CurrentValue > Mid_Press_Value+2){
                    MotorStop_PWM1(TIM3); 
                    CH1_Main_Step=5;
                    CH1_Timer_Out=10;
                }
           }break;   
//3--------------------------------------------------------------------------------------------------------------------------------------------           
           case 5:{
             if(CH1_Timer_Out <= 0){
               CH1_Mtion_ABS_POS_Relative=Third_distance; //�� CH1_Mtion_LDistance ����
               Motor1Starting_PWM1(DIR_CW,CH1_Mtion_LSpeed);
               CH1_Main_Step=6;
             }
           }break;
           
           case 6:{
               //�½������ʵľ���
               if((CH1_Mtion_ABS_POS_Relative == 0) || (CH1_CurrentValue >= Mid_Press_Value+2)){
                   MotorStop_PWM1(TIM3);
                   CH1_Timer_Out=5;
                   CH1_Main_Step=7;
               }
               
           }
           case 7:{
             if(CH1_Timer_Out<=0){
               if(CH1_CurrentValue > CH1_SetPress_Value+CH1_SetDev_Value){
                 CH1_Main_Step=8;   //����С�ξ���
               }
               
               else if(CH1_CurrentValue < CH1_SetPress_Value){
                 CH1_Main_Step=5;   //�½�С�ξ���
               }
               
               else{
                 //��ѹ��ָ����״̬
                 AppTask_Fixture_Status.F_CH1_CNT=1;
                 CH1_Counter++;
                 //����EEPRO
                 IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd116),&CH1_Counter);
                 
                 CH1_Main_Step=0;
                 MotorStop_PWM1(TIM3);
                 AppTask_Fixture_Status.F_CH1_PDFinish=1;
                 AppTask_Fixture_Status.F_CH1_PendingDrop =0;
               }
             }
             
           }break;
           
           case 8:{
               //����С�ξ���
               CH1_Mtion_ABS_POS_Relative=2;
               Motor1Starting_PWM1(DIR_CCW,CH1_Mtion_LSpeed);
               CH1_Main_Step=9;
           }break;
           
           case 9:{
               //���������ʵľ���
               if((CH1_Mtion_ABS_POS_Relative == 0) | (CH1_CurrentValue < CH1_SetPress_Value)){
                     MotorStop_PWM1(TIM3);
                     CH1_Main_Step=7;
                }
           }break;
           

     } 
     //������λ��
     if(AppTask_Sensor_Status.F_x5 ==1 && AppTask_Fixture_Status.F_CH1_PendingDrop == 1){
           //���ֹͣ
           CH1_Mtion_ABS_POS_Relative=0;
           MotorStop_PWM1(TIM3);
           
           //�������һ����ѹ
           CH1_Main_Step=0;
           AppTask_Fixture_Status.F_CH1_PDFinish=1;
           AppTask_Fixture_Status.F_CH1_PendingDrop =0;
           
           //��ӡ������Ϣ
           EXT1_BSP_Ser_Printf("Motion Out of range");   
     }     
}

//ѹͷ�˳�
void  CH1_Platen_Rise(void)
{   
     CPU_INT32U Reset_distance;
     static CPU_INT08U Reset_Step=0;
     
     //��λ���� 
     if( AppTask_Fixture_Status.F_CH1_PDReset == 1){
        Reset_Step=1;AppTask_Fixture_Status.F_CH1_PDReset=0;
     }
     if(Reset_Step == 1){
        Reset_distance =(Reset_Distance_Max/10)*SUBDIVISION/16;
     }
     if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1 ){
        Reset_Step=0;
     }
     switch(Reset_Step)
     {
          case 1:{
                //����С�ξ���
                CH1_Mtion_ABS_POS_Relative=Reset_distance;
                Motor1Starting_PWM1(DIR_CCW,CH1_Mtion_ResetSpeed);
                Reset_Step=2;
          }break;
          
          case 2:{ 
                if(CH1_Mtion_ABS_POS_Relative == 0 | BSP_PIN1_05 == 0){
                   
                   AppTask_Sensor_Status.F_x4=1; //ԭ��
                   MotorStop_PWM1(TIM3);
                   Reset_Step=0;
                   
                   AppTask_Fixture_Status.F_CH1_PDFinish=0;
                   AppTask_Fixture_Status.F_CH1_PendingRise=0;
                }
                
          }break;      
     }
  
}

//һ����ѹ����
void CH1_Fixture_Action_IN(void)
{
    static CPU_INT08U CH1_Cy_Step=0;
      
    if(AppTask_Fixture_Status.F_CH1_START == 1){
        CH1_Cy_Step=1;
        AppTask_Fixture_Status.F_CH1_START=0;
        AppTask_Fixture_Status.F_CH1_PendingDrop=1;//����ִ����ѹ����
    }
    if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1 || AppTask_Fixture_Status.F_CH1_UP_Reset ==1){
      CH1_Cy_Step=0;
    }
    
    switch(CH1_Cy_Step){
          case 1: {
              CH1_Platform_IN();
              CH1_Cy_Step=2;
              CH1_Timer_Out=500;
          }break;
          
          case 2: {
            
            if((AppTask_Sensor_Status.F_x7==1)&&(CH1_Timer_Out>0)){
                CH1_Cy_Step=3;
                CH1_Timer_Out=50; //��ʱ500ms
                
            }
            else if(CH1_Timer_Out == 0){
               //����,���ײ���λ
                CH1_PlatformLED_ERROR(LED_OPEN);
                CH1_Cy_Step=0;
            }
          }break;
      
          //AppTask_Fixture_Status.F_CH1_PDStart;
          case 3: {
                if(CH1_Timer_Out == 0){
                  //��ʱ500ms��������ѹ
                  AppTask_Fixture_Status.F_CH1_PDStart =1;
                  CH1_Cy_Step=4;
                }
        
          }break;
    }
  
}

//һ���˳�����
void CH1_Fixture_Action_OUT(void)
{
    static CPU_INT08U CH1_Cy_ResetStep=0;
      
    if(AppTask_Fixture_Status.F_CH1_EXIT == 1){
        CH1_Cy_ResetStep=1;
        AppTask_Fixture_Status.F_CH1_EXIT=0;
        AppTask_Fixture_Status.F_CH1_PendingRise=1; //����ִ���˳� 
    }
    if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1){
      CH1_Cy_ResetStep=0;
    }
    
    switch(CH1_Cy_ResetStep){
      
           case 1: {
                  AppTask_Fixture_Status.F_CH1_PDReset =1;
                  CH1_Cy_ResetStep=2;
           }break;    
      
          case 2: {
                  //�ȴ�ѹͷ̧����ԭ��
                  if(AppTask_Fixture_Status.F_CH1_PendingRise == 0 && BSP_PIN1_05 == 0){
                      CH1_Platform_OUT();
                      CH1_Cy_ResetStep=3;
                      CH1_Timer_Out=500; //��ʱ���ʱ����5s
                  }
          }break;
          
          case 3: {
            //���������λ
            if((AppTask_Sensor_Status.F_x6 == 1)&&(CH1_Timer_Out>0)){
                CH1_Cy_ResetStep=4;
                CH1_Timer_Out=50; //��ʱ500ms
            }
//            else if(CH1_Timer_Out == 0){
//               //����,���ײ���λ
//                CH1_PlatformLED_ERROR(LED_OPEN);
//                CH1_Cy_ResetStep=0;
//            }
          
          }break;
      
          //AppTask_Fixture_Status.F_CH1_PDStart;
          case 4: {
                if(CH1_Timer_Out == 0){
                  //��ʱ500ms��������ѹ
                  CH1_Cy_ResetStep=5;
                }
        
          }break;
    }
  
}
//����豸�쳣��Ϣ
//AppTask_Fixture_Status.F_CH1_PendingRise == 1;  //������
//AppTask_Fixture_Status.F_CH1_PendingDrop == 1;  //�½���
//AppTask_Fixture_Status.F_CH1_PDFinish == 1;     //���
void Check_Fixture_CH1_AbnormalInfo(void)
{
      //����쳣���� CH1_Mtion_ABS_POS_Relative,CH2_Mtion_ABS_POS_Relative,CH3_Mtion_ABS_POS_Relative,CH4_Mtion_ABS_POS_Relative
      if(CH1_Mtion_ABS_POS_Relative!=0 && BSP_DIR1 == DIR_CCW && AppTask_Sensor_Status.F_x4 ==1){
            //����ֹͣ������� TIM3
            MotorStop_PWM1(TIM3);CH1_Mtion_ABS_POS_Relative=0;
      }
  
      //CH1�˶����̼���Ʒ�Ƿ���λ�� == 1��ʾ��λ�� F_CH1_SPInSitu,F_CH2_SPInSitu,F_CH3_SPInSitu,F_CH4_SPInSitu
      if(AppTask_Fixture_Status.F_CH1_PendingRise == 1 && AppTask_Fixture_Status.F_CH1_PendingDrop == 1 && AppTask_Fixture_Status.F_CH1_PDFinish == 1)
      {
            if(AppTask_Sensor_Status.F_x3 == 1){
              AppTask_Fixture_Status.F_CH1_SPInSitu=1;
              CH1_PlatformLED_ERROR(LED_CLOSE);
            }else{
              AppTask_Fixture_Status.F_CH1_SPInSitu=0;
              CH1_PlatformLED_ERROR(LED_OPEN);
              
            }
      } 

}

//�豸ͨ��һ״̬��Ϣ
void Fixture_CH1_StatuesInfo(void)
{
   
    if(AppTask_Sensor_Status.F_x17 == 0 && AppTask_Sensor_Status.F_x9 == 0 ){
          //��ѹ
          if(AppTask_Fixture_Status.F_CH1_UP_Drop == 1 && AppTask_Fixture_Status.F_CH1_PDFinish == 1){
            AppTask_Fixture_Status.F_CH1_UP_Drop=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //�˳�
          if(AppTask_Fixture_Status.F_CH1_UP_Rise == 1&& AppTask_Sensor_Status.F_x6 == 1 && AppTask_Sensor_Status.F_x4 ==1){
            AppTask_Fixture_Status.F_CH1_UP_Rise=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //��λ
          if(AppTask_Fixture_Status.F_CH1_UP_Reset == 1 && AppTask_Sensor_Status.F_x6 == 1){
            AppTask_Fixture_Status.F_CH1_UP_Reset=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");
            Clear_CH1_Flag();
          }
          
    }  
      
}

//��λ�� �����־λ
void Clear_CH1_Flag(void)
{
      AppTask_Fixture_Status.F_CH1_PendingDrop = 0;    
      AppTask_Fixture_Status.F_CH1_PendingRise = 0;     
      AppTask_Fixture_Status.F_CH1_PDFinish = 0;
}

//ͨ��һ��ָ�����
/*
      CH1_FORCEDLD_OPEN();
      CH2_FORCEDLD_OPEN();
      CH3_FORCEDLD_OPEN();
      CH4_FORCEDLD_OPEN();
*/
CPU_INT08U CH1_UartCMD_Analyze(void)
{
     //   CPU_INT16S   *LPtr_RegData=RegData;
    
        if(Str_Cmp((const CPU_CHAR*)"01 ACTION=CLOSE",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH1_UP_Drop=1;//׼����ӡ
              if(AppTask_Fixture_Status.F_CH1_PendingDrop == 0 && AppTask_Fixture_Status.F_CH1_PDFinish == 0){
                CH1_Action_Platen_IN_OUT();
              }
              return 0;
        }  
        
        else if(Str_Cmp((const CPU_CHAR*)"01 ACTION=OPEN",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH1_UP_Rise=1; //׼����ӡ
              if(AppTask_Fixture_Status.F_CH1_PendingRise == 0 && AppTask_Fixture_Status.F_CH1_PDFinish == 1){
                CH1_Action_Platen_IN_OUT();  
              }
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 ACTION=RESET",(const CPU_CHAR*)receBuf)==0){
              //01 ͨ����λ
              AppTask_Fixture_Status.F_CH1_UP_Reset =1;
              CH1_POWER_OFF(); //�ر�Vbus
              CH1_FORCEDLD_CLOSE();//�ر�ǿ�Ƽ���
              CH1_PlatformLED_FLD(LED_CLOSE);//�ر�fld LED
              CH1_PlatformLED_ERROR(LED_CLOSE); //�ر�errorָʾ��
              
              if(AppTask_Fixture_Status.F_CH1_PendingRise == 0){
                
                AppTask_Fixture_Status.F_CH1_START=0;    //�˳�
                AppTask_Fixture_Status.F_CH1_EXIT=1;
                
              }
              return 0;
        }

        else if(Str_Cmp((const CPU_CHAR*)"01 POWER=ON",(const CPU_CHAR*)receBuf)==0){
              //�л�usb on
              AppTask_Fixture_Status.F_CH1_UP_Vbus=1;
              CH1_POWER_ON();
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 POWER=OFF",(const CPU_CHAR*)receBuf)==0){
              //�л�usb off
              CH1_POWER_OFF();
              AppTask_Fixture_Status.F_CH1_UP_Vbus=0;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 POWER?",(const CPU_CHAR*)receBuf)==0){
              //��ѯ�л���־λ
              if(AppTask_Fixture_Status.F_CH1_UP_Vbus == 1)
              { //�ϵ�״̬
                EXT1_BSP_Ser_Printf("\r\n01 POWER ON\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }else{
                //û���ϵ�״̬
                EXT1_BSP_Ser_Printf("\r\n01 POWER OFF\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              return 0;
        }
        
        //ǿ������01 action=forcedld_open
        else if(Str_Cmp((const CPU_CHAR*)"01 ACTION=FORCEDLD_OPEN",(const CPU_CHAR*)receBuf)==0){
              CH1_FORCEDLD_OPEN();
              CH1_PlatformLED_FLD(LED_OPEN);
              AppTask_Fixture_Status.F_CH1_UP_FLoad=1;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 ACTION=FORCEDLD_CLOSE",(const CPU_CHAR*)receBuf)==0){
              CH1_FORCEDLD_CLOSE();
              CH1_PlatformLED_FLD(LED_CLOSE); 
              AppTask_Fixture_Status.F_CH1_UP_FLoad=0;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;          
        }       
        
        else if(Str_Cmp((const CPU_CHAR*)"01 RESULT=PASS",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              //CH1_PlatformLED_ERROR(LED_OPEN);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 RESULT=FAIL",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              // CH1_PlatformLED_ERROR(LED_CLOSE);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"01 KDPPWR=ON",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        else if(Str_Cmp((const CPU_CHAR*)"01 KDPPWR=OFF",(const CPU_CHAR*)receBuf)==0){
              //��ӡOK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        //��ӡ����
        else if(Str_Cmp((const CPU_CHAR*)"01 TSTCNT",(const CPU_CHAR*)receBuf)==0){
              EXT1_BSP_Ser_Printf("\r\n%d\r\n",CH1_Counter);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //��������
        else if(Str_Cmp((const CPU_CHAR*)"01 BUTTON=LOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH1_UP_LB=1; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              
              return 0;
        }	
        //�ͷŰ���
        else if(Str_Cmp((const CPU_CHAR*)"01 BUTTON=UNLOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH1_UP_LB=0; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //�о�״̬
        else if(Str_Cmp((const CPU_CHAR*)"01 DUT STAT",(const CPU_CHAR*)receBuf)==0){
              if(AppTask_Fixture_Status.F_CH1_PDFinish ==1 && AppTask_Sensor_Status.F_x3 ==1){
                //READING
                EXT1_BSP_Ser_Printf("\r\nREADY\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              else if(AppTask_Fixture_Status.F_CH1_PDFinish ==0 && AppTask_Sensor_Status.F_x3 ==1){
                //WAITING
                EXT1_BSP_Ser_Printf("\r\nWAITTING\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              else{
                //UNDETECTED
                EXT1_BSP_Ser_Printf("\r\nUNDETECTED\r\n"); 
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              
              return 0;
        }
        
        //��ǰ��֧��ѹ��������� CH1_CurrentValue
        else if(Str_Cmp((const CPU_CHAR*)"01 TPRESS=?",(const CPU_CHAR*)receBuf)==0){
          
              EXT1_BSP_Ser_Printf("\r\nTPRESS: 1\r\n\r\nTPRESS: 1,\"PRESS\"\r\n",CH1_CurrentValue);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //��ȡѹ�������ֵ
        else if(Str_Cmp_N((const CPU_CHAR*)"01 TPRESS=1,0,",(const CPU_CHAR*)receBuf,14)==0){
              EXT1_BSP_Ser_Printf("\r\n%.1f\r\n",CH1_CurrentValue);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //�趨CH1ѹ��������ֵ
        else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"01 TPRESS=1,1,",14) == 0 ){
              //CH1_SetMaxPress_Value = 
              CH1_SetMaxPress_Value = 1.0 * Conversion_fraction("01 TPRESS=1,1,",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd132),20);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }						
        
        
        else if(Str_Cmp((const CPU_CHAR*)"01 FIXTURE STAT",(const CPU_CHAR*)receBuf)==0){
          
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

