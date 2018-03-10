#include "app_CH3_Action.h"

#define POWER_ON   0
#define POWER_OFF  1

#define Mid_Press_Value 7

//相关端口定义
//#define   BSP_POUT3_01       PGout(11)
//#define   BSP_POUT3_02       PGout(9)
//#define   BSP_POUT3_03       PDout(6)
//#define   BSP_POUT3_04       PDout(4)
//#define   BSP_POUT3_05       PFout(2)  //13
//#define   BSP_POUT3_06       PFout(0)  //14
//#define   BSP_POUT3_07       PCout(14) //15
//#define   BSP_POUT3_08       PEout(6)  //16
//CH3 Action控制
void CH3_Action_Platen_IN_OUT()
{
      if(AppTask_Sensor_Status.F_x9 == 0 && AppTask_Sensor_Status.F_x19==1 && AppTask_Sensor_Status.F_x1 ==1){
        if(AppTask_Fixture_Status.F_CH3_PDFinish == 0 && AppTask_Sensor_Status.F_x22 ==1){
          if(AppTask_Fixture_Status.F_CH3_PendingDrop == 0){
            AppTask_Fixture_Status.F_CH3_START=1;
            AppTask_Fixture_Status.F_CH3_EXIT =0;
          }
        }  
        else if(AppTask_Fixture_Status.F_CH3_PDFinish == 1 ){
          if(AppTask_Fixture_Status.F_CH3_PendingRise == 0){
            AppTask_Fixture_Status.F_CH3_START=0;
            AppTask_Fixture_Status.F_CH3_EXIT=1;
          }
        }  
      }
}

//CH3单步按钮
void CH3_Singal_Step_Button(void)
{
    OS_ERR      err;
    
    if(AppTask_Sensor_Status.F_x18==1){
          OSTimeDlyHMSM( 0,    //周期性的调用AppTaskMotionControl
                        0,
                        0,
                        25,
                        OS_OPT_TIME_HMSM_STRICT,
                        &err); 
          if(AppTask_Sensor_Status.F_x18==1){  
               CH3_Action_Platen_IN_OUT();
          }   
      
    }
  
}
//平台推进
void CH3_Platform_IN(void)
{
    BSP_POUT3_01=POWER_ON;BSP_POUT3_02=POWER_OFF;
}
    
//平台退出
void CH3_Platform_OUT(void)
{
    BSP_POUT3_01=POWER_OFF;BSP_POUT3_02=POWER_ON;
}
//stop
void CH3_Platform_Stop(void)
{
    BSP_POUT3_01=POWER_OFF;BSP_POUT3_02=POWER_OFF;
}
//打开VBUS 
void CH3_POWER_ON(void)
{
     BSP_SW3_VBUS=1;
}
//关闭VBUS
void CH3_POWER_OFF(void)
{
     BSP_SW3_VBUS=0;
}
//BSP_SW3_VBUS
void CH3_FORCEDLD_OPEN(void)
{
    BSP_SW3_FLOAD=1;

}
//关闭VBUS
void CH3_FORCEDLD_CLOSE(void)
{
    BSP_SW3_FLOAD=0;
  
}
    
//LED指示忙
void CH3_PlatformLED_FLD(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT3_04=POWER_ON;
      }
      else{
        BSP_POUT3_04=POWER_OFF; 
      }
}

//LED指示错误
void CH3_PlatformLED_ERROR(CPU_INT08U Statues)
{
      if(Statues == 1){
        BSP_POUT3_03=POWER_ON;
      }
      else{
        BSP_POUT3_03=POWER_OFF; 
      }
}
//CH3计数器触发
void CH3_CNT_Trigger(void)
{
      static CPU_INT08U CNT_Step=0;
        
      if(AppTask_Fixture_Status.F_CH3_CNT == 1){
          AppTask_Fixture_Status.F_CH3_CNT=0;
          CNT_Step=1;
      }
      //产生脉冲计数
      switch(CNT_Step)
      {
            case 1:{   
              BSP_POUT3_05=POWER_ON;
              CH3_Timer_Out=50;
              CNT_Step=2;
            }break;
            
            case 2:{  
              if(CH3_Timer_Out <= 0){
                  BSP_POUT3_05=POWER_OFF;
                  CNT_Step=3;
              }
            }break;
      
      }

}
//CH3 速度 HSpeed,MSpeed,LSpeed
//CPU_INT32U   CH3_Mtion_HDistance,CH3_Mtion_MDistance,CH3_Mtion_LDistance;
//CPU_INT08U   CH3_Mtion_HSpeed,   CH3_Mtion_MSpeed,   CH3_Mtion_LSpeed;
//压头下压
//计算需要走的各个段的距离
//First_distance =(CH3_Mtion_HDistance/10)*SUBDIVISION/8;
//Second_distance=(CH3_Mtion_MDistance/10)*SUBDIVISION/8;
//Third_distance =(CH3_Mtion_LDistance/10)*SUBDIVISION/8;

void  CH3_Platen_Drop(void)
{
     CPU_INT32U First_distance,Second_distance,Third_distance;
     //变量
     static CPU_INT08U CH3_Main_Step=0;
     
     //使用标志位 F_CH3_PDStart == 1 ,控制 CH3_Main_Step=1;
     //使用标志位 F_CH3_PDFinish == 1 表示预压到位
     if(AppTask_Fixture_Status.F_CH3_PDStart == 1){
        CH3_Main_Step=1;AppTask_Fixture_Status.F_CH3_PDStart=0;
     }
     
     if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1 || AppTask_Fixture_Status.F_CH3_UP_Reset ==1|| CH3_CurrentValue>CH3_SetMaxPress_Value){ //急停事件，光栅事件
        CH3_Main_Step=0;
     }

     First_distance =(CH3_Mtion_HDistance/10)*SUBDIVISION/16;
     Second_distance= CH3_Mtion_MDistance;
     Third_distance = CH3_Mtion_LDistance;

     
     switch(CH3_Main_Step){
 //1--------------------------------------------------------------------------------------------------------------------------------------------         
           case 1:{
               CH3_Mtion_ABS_POS_Relative=First_distance;
               Motor3Starting_PWM3(DIR_CW,CH3_Mtion_HSpeed);
               CH3_Main_Step=2;
           }break;
           case 2:{
               if(CH3_CurrentValue > Mid_Press_Value+2){
                 MotorStop_PWM3(TIM2);
                 CH3_Main_Step=5;   //高速过压，进入低速
               }
               if(CH3_Mtion_ABS_POS_Relative == 0 ||CH3_CurrentValue > Mid_Press_Value-4){
                 MotorStop_PWM3(TIM2);
                 CH3_Main_Step=3;   //进入中速
               }
       
           }break;
//2--------------------------------------------------------------------------------------------------------------------------------------------           
           case 3:{
               CH3_Mtion_ABS_POS_Relative=Second_distance;
               Motor3Starting_PWM3(DIR_CW,CH3_Mtion_MSpeed);
               CH3_Main_Step=4;
           }break;
            case 4:{
                if(CH3_Mtion_ABS_POS_Relative == 0 || CH3_CurrentValue > Mid_Press_Value){
                    MotorStop_PWM3(TIM2); 
                    CH3_Main_Step=5; //进入低速
                    CH3_Timer_Out=10;
                }
           }break;   
//3--------------------------------------------------------------------------------------------------------------------------------------------           
           case 5:{
             if(CH3_Timer_Out <=0 ){
               CH3_Mtion_ABS_POS_Relative=Third_distance;
               Motor3Starting_PWM3(DIR_CW,CH3_Mtion_LSpeed);
               CH3_Main_Step=6;
             }
           }break;
           
           case 6:{
               //下降到合适的距离
               if((CH3_Mtion_ABS_POS_Relative == 0) || (CH3_CurrentValue > Mid_Press_Value+2)){
                   MotorStop_PWM3(TIM2);
                   CH3_Main_Step=7;
                   CH3_Timer_Out=5;
               }
               
           }
           case 7:{
             if(CH3_Timer_Out <= 0){
               if(CH3_CurrentValue > CH3_SetPress_Value+CH3_SetDev_Value){
                   CH3_Main_Step=8;   //上升小段距离
               }
               
               else if(CH3_CurrentValue < CH3_SetPress_Value){
                   CH3_Main_Step=5;   //下降小段距离
               }
               
               else{
                   //下压到指定的状态
                   AppTask_Fixture_Status.F_CH3_CNT=1;
                   CH3_Counter++;
                   //保存EEPRO
                   IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd124),&CH3_Counter);
                   
                   CH3_Main_Step=0;
                   MotorStop_PWM3(TIM2);
                   AppTask_Fixture_Status.F_CH3_PDFinish=1;
                   AppTask_Fixture_Status.F_CH3_PendingDrop =0;
               }
             }
           }break;
           
           case 8:{
               //上升小段距离
               CH3_Mtion_ABS_POS_Relative=2;
               Motor3Starting_PWM3(DIR_CCW,CH3_Mtion_LSpeed);
               CH3_Main_Step=9;
           }break;
           
           case 9:{
               //上升到合适的距离
               if((CH3_Mtion_ABS_POS_Relative == 0) || (CH3_CurrentValue < CH3_SetPress_Value)){
                     MotorStop_PWM3(TIM2);
                     CH3_Main_Step=7;
                 }
           }break;
     }  
     
     //碰到限位点
     if(AppTask_Sensor_Status.F_x21 == 1 && AppTask_Fixture_Status.F_CH3_PendingDrop == 1){
       //电机停止
       CH3_Mtion_ABS_POS_Relative=0;
       MotorStop_PWM3(TIM2);
       
       //类似完成一次下压
       CH3_Main_Step=0;
       AppTask_Fixture_Status.F_CH3_PDFinish=1;
       AppTask_Fixture_Status.F_CH3_PendingDrop =0;
       
       //打印警告信息
       EXT1_BSP_Ser_Printf("Motion Out of range");   
     }
}

//压头退出
void  CH3_Platen_Rise(void)
{   
     CPU_INT32U Reset_distance;
     static CPU_INT08U Reset_Step=0;
     
     //复位距离 
     if( AppTask_Fixture_Status.F_CH3_PDReset == 1){
        Reset_Step=1;AppTask_Fixture_Status.F_CH3_PDReset=0;
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
                //上升小段距离
                CH3_Mtion_ABS_POS_Relative=Reset_distance;
                Motor3Starting_PWM3(DIR_CCW,CH3_Mtion_ResetSpeed);
                Reset_Step=2;
          }break;
          
          case 2:{ 
                if(CH3_Mtion_ABS_POS_Relative == 0 | BSP_PIN3_05 == 0){
                   
                   AppTask_Sensor_Status.F_x20=1; //原点
                   MotorStop_PWM3(TIM2);
                   Reset_Step=0;
                   
                   AppTask_Fixture_Status.F_CH3_PDFinish=0;
                   AppTask_Fixture_Status.F_CH3_PendingRise=0;
                }
                
          }break;      
     }
  
}

//一次下压动作
void CH3_Fixture_Action_IN(void)
{
    static CPU_INT08U CH3_Cy_Step=0;
      
    if(AppTask_Fixture_Status.F_CH3_START == 1){
        CH3_Cy_Step=1;
        AppTask_Fixture_Status.F_CH3_START=0;
        AppTask_Fixture_Status.F_CH3_PendingDrop=1;//正在执行下压动作
    }
    if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1|| AppTask_Fixture_Status.F_CH3_UP_Reset ==1){
      CH3_Cy_Step=0;
    }
    switch(CH3_Cy_Step){
          case 1: {
              CH3_Platform_IN();
              CH3_Cy_Step=2;
              CH3_Timer_Out=500;
          }break;
          
          case 2: {
            
            if((AppTask_Sensor_Status.F_x23==1)&&(CH3_Timer_Out>0)){
                CH3_Cy_Step=3;
                CH3_Timer_Out=50; //延时500ms
            }
            else if(CH3_Timer_Out == 0){
               //出错,气缸不到位
                CH3_PlatformLED_ERROR(LED_OPEN);
                CH3_Cy_Step=0;
            }
          }break;
      
          //AppTask_Fixture_Status.F_CH3_PDStart;
          case 3: {
                if(CH3_Timer_Out == 0){
                  //延时500ms后，启动下压
                  AppTask_Fixture_Status.F_CH3_PDStart =1;
                  CH3_Cy_Step=4;
                }
        
          }break;
    }
  
}

//一次退出动作
void CH3_Fixture_Action_OUT(void)
{
    static CPU_INT08U CH3_Cy_ResetStep=0;
      
    if(AppTask_Fixture_Status.F_CH3_EXIT == 1){
        CH3_Cy_ResetStep=1;
        AppTask_Fixture_Status.F_CH3_EXIT=0;
        AppTask_Fixture_Status.F_CH3_PendingRise=1; //正在执行退出 
    }
    if(AppTask_Sensor_Status.F_x9 == 1 || AppTask_Sensor_Status.F_x17 == 1){
      CH3_Cy_ResetStep=0;
    }
    
    switch(CH3_Cy_ResetStep){
      
           case 1: {
                  AppTask_Fixture_Status.F_CH3_PDReset =1;
                  CH3_Cy_ResetStep=2;
           }break;    
      
          case 2: {
                  if(AppTask_Fixture_Status.F_CH3_PendingRise == 0 && BSP_PIN3_05 == 0){
                      CH3_Platform_OUT();
                      CH3_Cy_ResetStep=3;
                      CH3_Timer_Out=500; //超时检测时间是2s
                  }
          }break;
          
          case 3: {
                  //检测气缸退位
                  if((AppTask_Sensor_Status.F_x22 == 1)&&(CH3_Timer_Out>0)){
                      CH3_Cy_ResetStep=4;
                      CH3_Timer_Out=50; //延时500ms
                  }
//                  else if(CH3_Timer_Out == 0){
//                     //出错,气缸不到位
//                      CH3_PlatformLED_ERROR(LED_OPEN);
//                      CH3_Cy_ResetStep=0;
//                  }
          
          }break;
      
          //AppTask_Fixture_Status.F_CH3_PDStart;
          case 4: {
                if(CH3_Timer_Out == 0){
                  //延时500ms后，启动下压
                  //AppTask_Fixture_Status.F_CH3_PDReset =1;
                  CH3_Cy_ResetStep=5;
                }
        
          }break;
    }
  
}
//检测设备异常信息
//AppTask_Fixture_Status.F_CH3_PendingRise == 1;  //上升期
//AppTask_Fixture_Status.F_CH3_PendingDrop == 1;  //下降期
//AppTask_Fixture_Status.F_CH3_PDFinish == 1;     //完成
void Check_Fixture_CH3_AbnormalInfo(void)
{
      //马达异常处理 CH1_Mtion_ABS_POS_Relative,CH2_Mtion_ABS_POS_Relative,CH3_Mtion_ABS_POS_Relative,CH4_Mtion_ABS_POS_Relative
      if(CH3_Mtion_ABS_POS_Relative!=0 && BSP_DIR3 == DIR_CCW && AppTask_Sensor_Status.F_x20 ==1){
        //立即停止电机运行 TIM2
        MotorStop_PWM3(TIM2);CH3_Mtion_ABS_POS_Relative=0;
      }
      //CH3运动过程检测产品是否在位置 == 1表示在位置 F_CH3_SPInSitu,F_CH3_SPInSitu,F_CH3_SPInSitu,F_CH4_SPInSitu
      if(AppTask_Fixture_Status.F_CH3_PendingRise == 1 && AppTask_Fixture_Status.F_CH3_PendingDrop == 1 && AppTask_Fixture_Status.F_CH3_PDFinish == 1)
      {
          if(AppTask_Sensor_Status.F_x19 == 1){
            AppTask_Fixture_Status.F_CH3_SPInSitu=1;
            CH3_PlatformLED_ERROR(LED_CLOSE);
          }else{
            AppTask_Fixture_Status.F_CH3_SPInSitu=0;
            CH3_PlatformLED_ERROR(LED_OPEN);
          }
      } 
}

//复位后 清除标志位
void Clear_CH3_Flag(void)
{
      AppTask_Fixture_Status.F_CH3_PendingDrop = 0;    
      AppTask_Fixture_Status.F_CH3_PendingRise = 0;     
      AppTask_Fixture_Status.F_CH3_PDFinish = 0;
}

//设备通道二状态信息
void Fixture_CH3_StatuesInfo(void)
{
    if(AppTask_Sensor_Status.F_x17 == 0 && AppTask_Sensor_Status.F_x9 == 0 ){
          //下压
          if(AppTask_Fixture_Status.F_CH3_UP_Drop == 1 && AppTask_Fixture_Status.F_CH3_PDFinish == 1){
            AppTask_Fixture_Status.F_CH3_UP_Drop=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //退出
          if(AppTask_Fixture_Status.F_CH3_UP_Rise == 1&& AppTask_Sensor_Status.F_x22 == 1 && AppTask_Sensor_Status.F_x20 ==1){
            AppTask_Fixture_Status.F_CH3_UP_Rise=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");  
          }
          
          //复位
          if(AppTask_Fixture_Status.F_CH3_UP_Reset == 1 && AppTask_Sensor_Status.F_x22 == 1){
            AppTask_Fixture_Status.F_CH3_UP_Reset=0;
            EXT1_BSP_Ser_Printf("\r\nOK\r\n");
            Clear_CH3_Flag();
          }
          
    }  
      
}

//通道三：指令解析
CPU_INT08U CH3_UartCMD_Analyze(void)
{
      //  CPU_INT16S   *LPtr_RegData=RegData;
        if(Str_Cmp((const CPU_CHAR*)"03 ACTION=CLOSE",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH3_UP_Drop=1;//准备打印
              if(AppTask_Fixture_Status.F_CH3_PendingDrop == 0 && AppTask_Fixture_Status.F_CH3_PDFinish == 0){
                CH3_Action_Platen_IN_OUT();
              }

              return 0;
        }  
        
        else if(Str_Cmp((const CPU_CHAR*)"03 ACTION=OPEN",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH3_UP_Rise=1; //准备打印
              if(AppTask_Fixture_Status.F_CH3_PendingRise == 0 && AppTask_Fixture_Status.F_CH3_PDFinish == 1){
                CH3_Action_Platen_IN_OUT();  
              }
  
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"03 ACTION=RESET",(const CPU_CHAR*)receBuf)==0){
              //03 通道复位
              AppTask_Fixture_Status.F_CH3_UP_Reset =1;
              CH3_POWER_OFF(); //关闭Vbus
              CH3_FORCEDLD_CLOSE();//关闭强制加载
              CH3_PlatformLED_FLD(LED_CLOSE);//关闭fld LED
              CH3_PlatformLED_ERROR(LED_CLOSE); //关闭error指示灯
              
              if(AppTask_Fixture_Status.F_CH3_PendingRise == 0){
                
                AppTask_Fixture_Status.F_CH3_START=0;    //退出
                AppTask_Fixture_Status.F_CH3_EXIT=1;
                
              }
     
              return 0;
        }

        else if(Str_Cmp((const CPU_CHAR*)"03 POWER=ON",(const CPU_CHAR*)receBuf)==0){
              //切换usb on
              AppTask_Fixture_Status.F_CH3_UP_Vbus=1;
              CH3_POWER_ON();
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"03 POWER=OFF",(const CPU_CHAR*)receBuf)==0){
              //切换usb off
              CH3_POWER_OFF();
              AppTask_Fixture_Status.F_CH3_UP_Vbus=0;
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"03 POWER?",(const CPU_CHAR*)receBuf)==0){
              //查询切换标志位
              if(AppTask_Fixture_Status.F_CH3_UP_Vbus == 1)
              { //上电状态
                EXT1_BSP_Ser_Printf("\r\n03 POWER ON\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }else{
                //没有上电状态
                EXT1_BSP_Ser_Printf("\r\n03 POWER OFF\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              return 0;
        }
        
        //强制下载03 action=forcedld_open
        else if(Str_Cmp((const CPU_CHAR*)"03 ACTION=FORCEDLD_OPEN",(const CPU_CHAR*)receBuf)==0){
          CH3_FORCEDLD_OPEN();
          CH3_PlatformLED_FLD(LED_OPEN);
          AppTask_Fixture_Status.F_CH3_UP_FLoad=1;
          EXT1_BSP_Ser_Printf("\r\nOK\r\n");
          return 0;
        }

        else if(Str_Cmp((const CPU_CHAR*)"03 ACTION=FORCEDLD_CLOSE",(const CPU_CHAR*)receBuf)==0){
          CH3_FORCEDLD_CLOSE();
          CH3_PlatformLED_FLD(LED_CLOSE); 
          AppTask_Fixture_Status.F_CH3_UP_FLoad=0;
          EXT1_BSP_Ser_Printf("\r\nOK\r\n");
          return 0;          
        } 
        
        else if(Str_Cmp((const CPU_CHAR*)"03 RESULT=PASS",(const CPU_CHAR*)receBuf)==0){
              //打印OK
              //CH3_PlatformLED_ERROR(LED_OPEN);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"03 RESULT=FAIL",(const CPU_CHAR*)receBuf)==0){
              //打印OK
              // CH3_PlatformLED_ERROR(LED_CLOSE);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        
        else if(Str_Cmp((const CPU_CHAR*)"03 KDPPWR=ON",(const CPU_CHAR*)receBuf)==0){
              //打印OK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        else if(Str_Cmp((const CPU_CHAR*)"03 KDPPWR=OFF",(const CPU_CHAR*)receBuf)==0){
              //打印OK
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }
        //打印计数
        else if(Str_Cmp((const CPU_CHAR*)"03 TSTCNT",(const CPU_CHAR*)receBuf)==0){
              EXT1_BSP_Ser_Printf("\r\n%d\r\n",CH3_Counter);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //锁定按键
        else if(Str_Cmp((const CPU_CHAR*)"03 BUTTON=LOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH3_UP_LB=1; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              
              return 0;
        }	
        //释放按键
        else if(Str_Cmp((const CPU_CHAR*)"03 BUTTON=UNLOCK",(const CPU_CHAR*)receBuf)==0){
              AppTask_Fixture_Status.F_CH3_UP_LB=0; 
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //夹具状态
        else if(Str_Cmp((const CPU_CHAR*)"03 DUT STAT",(const CPU_CHAR*)receBuf)==0){
              if(AppTask_Fixture_Status.F_CH3_PDFinish ==1 && AppTask_Sensor_Status.F_x3 ==1){
                //READING
                EXT1_BSP_Ser_Printf("\r\nREADY\r\n");
                EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              }
              else if(AppTask_Fixture_Status.F_CH3_PDFinish ==0 && AppTask_Sensor_Status.F_x3 ==1){
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
        
        //当前可支持压力块的总数 CH3_CurrentValue
        else if(Str_Cmp((const CPU_CHAR*)"03 TPRESS=?",(const CPU_CHAR*)receBuf)==0){
          
              EXT1_BSP_Ser_Printf("\r\nTPRESS: 1\r\n\r\nTPRESS: 1,\"PRESS\"\r\n");
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        
        //读取压力块的数值
        else if(Str_Cmp_N((const CPU_CHAR*)"03 TPRESS=1,0,",(const CPU_CHAR*)receBuf,14)==0){
              EXT1_BSP_Ser_Printf("\r\n%.1f\r\n",CH3_CurrentValue);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }	
        //设定CH3压力块的最大值
        else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"03 TPRESS=1,1,",14) == 0){
              CH3_SetMaxPress_Value =1.0 * Conversion_fraction("03 TPRESS=1,1,",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd136),20);
              EXT1_BSP_Ser_Printf("\r\nOK\r\n");
              return 0;
        }						
        
        
        else if(Str_Cmp((const CPU_CHAR*)"03 FIXTURE STAT",(const CPU_CHAR*)receBuf)==0){
          
              EXT1_BSP_Ser_Printf("\r\nRUNSTAT:\r\n");   
              //补充
              
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
