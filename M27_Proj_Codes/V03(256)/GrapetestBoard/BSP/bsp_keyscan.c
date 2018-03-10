#include <bsp_keyscan.h>
EXT Sensor_Status_Flag  AppTask_Sensor_Status;
/*
*********************************************************************************************************
*                                          AppTaskMotionControl
*
* Description : 用于传感器扫描
*
* Arguments   : p_arg   is the argument passed to 'AppTaskMotionControl()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
void Fixture_Sensor_Scan(void *p_arg)
{ 
  static CPU_INT08U Timerout_Counters[24]={0};
//  static CPU_INT16U Period=0;
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  CPU_INT08U   *Ptr_CoilData;
  Ptr_CoilData=CoilData;
//---------------------------------------------------------------------------------------1#
  //static CPU_INT16U Timerout_Err[4]={0};
if(BSP_PIN1_01==1){    //急停按钮按下                                                 
           if(Timerout_Counters[0]<2){
             Timerout_Counters[0]++;
           }
           else{
            if(AppTask_Sensor_Status.Emergency==0){
               AppTask_Sensor_Status.Emergency=1;
                AppTask_Sensor_Status.EmeEvent=1;
             }
           }
   }
else{
           if(Timerout_Counters[0]>0){
                 Timerout_Counters[0]--;
           }
           else{
               if(AppTask_Sensor_Status.Emergency==1){
                 AppTask_Sensor_Status.Emergency=0;          
             }
           }
   } 
//-------------------------------------------------------------------------------------2# 
if(BSP_PIN1_02 ==0){   //复位按钮按下
    
          if(Timerout_Counters[1]<5){
            Timerout_Counters[1]++;
           }
           else{

                if(AppTask_Sensor_Status.Reset==0){
                  AppTask_Sensor_Status.Reset=1;
                }
                if( AppTask_Sensor_Status.EmeEvent==1)
                {
                  AppTask_Sensor_Status.Reset_Event=1; 
                }
          }
   }
else{
          if(Timerout_Counters[1]>0){
                 Timerout_Counters[1]--;
          }
          else{
            
              if(AppTask_Sensor_Status.Reset==1){
                AppTask_Sensor_Status.Reset=0; 
              }
              
          }
  }
//-------------------------------------------------------------------------------------3# 
if(BSP_PIN1_03==0){    //左启动按钮    
        
           if(Timerout_Counters[2]<5){
             Timerout_Counters[2]++; 
           }
           else{
             
               if(AppTask_Sensor_Status.L_Start==0){
                   AppTask_Sensor_Status.L_Start=1;
              }
           }
   }
else{
           if(Timerout_Counters[2]>0){
                 Timerout_Counters[2]--;
           }
           else{
               if(AppTask_Sensor_Status.L_Start==1){
                 AppTask_Sensor_Status.L_Start=0; 
               }
           }
   }  
//-------------------------------------------------------------------------------------4# 
if(BSP_PIN1_04==0){    //右启动按钮
        
           if(Timerout_Counters[3]<2){
             Timerout_Counters[3]++; 
           }
           else{
             
               if(AppTask_Sensor_Status.R_Start==0){
                   AppTask_Sensor_Status.R_Start=1;
              }
           }
   }
else{
           if(Timerout_Counters[3]>0){
                 Timerout_Counters[3]--;
           }
           else{
               if(AppTask_Sensor_Status.R_Start==1){
                 AppTask_Sensor_Status.R_Start=0; 
               }
           }
   }  
//-------------------------------------------------------------------------------------5# 
if(BSP_PIN1_05==0){    //左电机上限位（原点位置）     
           if(Timerout_Counters[4]<2){
             Timerout_Counters[4]++;
           }
           else{
             if(AppTask_Sensor_Status.L_Motor_P==0){
               AppTask_Sensor_Status.L_Motor_P=1;
             }
           }
   }
else{
           if(Timerout_Counters[4]>0){
                 Timerout_Counters[4]--;
           }
           else{
               if(AppTask_Sensor_Status.L_Motor_P==1){
                 AppTask_Sensor_Status.L_Motor_P=0;
               }
           }
   }
//-------------------------------------------------------------------------------------6# 
if(BSP_PIN1_06==0){    //左电机下限位
           if(Timerout_Counters[5]<2){
             Timerout_Counters[5]++;
           }
           else{
             if(AppTask_Sensor_Status.L_Motor_N==0){
               AppTask_Sensor_Status.L_Motor_N=1;
            
             }
           }
   }
else{
           if(Timerout_Counters[5]>0){
                 Timerout_Counters[5]--;
           }
           else{
               if(AppTask_Sensor_Status.L_Motor_N==1){
                 AppTask_Sensor_Status.L_Motor_N=0; 
               }
           }
   } 
//-------------------------------------------------------------------------------------7# 
if(BSP_PIN1_07==0){    //右电机上限位（原点位置）
           if(Timerout_Counters[6]<2){
             Timerout_Counters[6]++;
           }
           else{
              if(AppTask_Sensor_Status.R_Motor_P==0){
               AppTask_Sensor_Status.R_Motor_P=1;
           
              }
           }
   }
else{
           if(Timerout_Counters[6]>0){
                 Timerout_Counters[6]--;
           }
           else{
               if(AppTask_Sensor_Status.R_Motor_P==1){
                 AppTask_Sensor_Status.R_Motor_P=0; 
             }
           }
   }   
//-------------------------------------------------------------------------------------8# 
if(BSP_PIN1_08==0){    //右电机下限位
           if(Timerout_Counters[7]<5){
             Timerout_Counters[7]++;
           }
           else{
             if(AppTask_Sensor_Status.R_Motor_N==0){
               AppTask_Sensor_Status.R_Motor_N=1;  
             }
           }
   }
else{
           if(Timerout_Counters[7]>0){
                 Timerout_Counters[7]--;
           }
           else{
               if(AppTask_Sensor_Status.R_Motor_N==1){
                 AppTask_Sensor_Status.R_Motor_N=0;
               }
           }
   }     
//-------------------------------------------------------------------------------------9# 
if(BSP_PIN2_01==0){    //上真空表状态
             if(Timerout_Counters[8]<3){
                 Timerout_Counters[8]++;
             }
             else{
                 if(AppTask_Sensor_Status.Vacuum_P==0){
                   AppTask_Sensor_Status.Vacuum_P=1;
                 }
             }
     }
else{
             if(Timerout_Counters[8]>0){
                   Timerout_Counters[8]--;
             }
             else{
                 if(AppTask_Sensor_Status.Vacuum_P== 1){
                      AppTask_Sensor_Status.Vacuum_P=0; 
                 }
             }
}    
//-------------------------------------------------------------------------------------10# 
if(BSP_PIN2_02==0){    //下真空表状态  
          if(Timerout_Counters[9]<3){
            Timerout_Counters[9]++;
           }
          else{
            if(AppTask_Sensor_Status.Vacuum_N==0){
              AppTask_Sensor_Status.Vacuum_N=1;
            }
           }
  }
else{
           if(Timerout_Counters[9]>0){
                Timerout_Counters[9]--;
           }
          else{
               if(AppTask_Sensor_Status.Vacuum_N==1){
                AppTask_Sensor_Status.Vacuum_N=0; 
              }
          }
   } 
//-------------------------------------------------------------------------------------11# 
if(BSP_PIN2_03==0){    //加热按钮
           if(Timerout_Counters[10]<5){
             Timerout_Counters[10]++;
           }
           else{
             if(AppTask_Sensor_Status.Heat_Start==0){
                 AppTask_Sensor_Status.Heat_Start=1;
                 AppTask_Sensor_Status.Heat_Event=1;
               }
             }
           }
else{
           if(Timerout_Counters[10]>0){
                 Timerout_Counters[10]--;
           }
           else{
               if(AppTask_Sensor_Status.Heat_Start==1){
                   AppTask_Sensor_Status.Heat_Start=0;
               }
           }
     } 
//-------------------------------------------------------------------------------------12# 
if(BSP_PIN2_04==1){    //预留输入1
           if(Timerout_Counters[11]<5){
           Timerout_Counters[11]++;
           }
           else{
             if(AppTask_Sensor_Status.DoorDet==0){
               AppTask_Sensor_Status.DoorDet=1;
             }
           }
   }
else{
           if(Timerout_Counters[11]>0){
                 Timerout_Counters[11]--;
           }
           else{
               if(AppTask_Sensor_Status.DoorDet==1){
                 AppTask_Sensor_Status.DoorDet=0;
               }
           }
   }   
//-------------------------------------------------------------------------------------13# 
if(BSP_PIN2_05==0){         //   
           if(Timerout_Counters[12]<5){
           Timerout_Counters[12]++;
           }
           else{
             if(AppTask_Sensor_Status.Input2==0){
               AppTask_Sensor_Status.Input2=1;
             }
           }
   }
else{
           if(Timerout_Counters[12]>0){
                 Timerout_Counters[12]--;
           }
           else{
               if(AppTask_Sensor_Status.Input2==1){
                 AppTask_Sensor_Status.Input2=0;
               }
           }
   } 
//-------------------------------------------------------------------------------------14# 
if(BSP_PIN2_06==0){          //
           if(Timerout_Counters[13]<2){
             Timerout_Counters[13]++;
           }
           else{
             if(AppTask_Sensor_Status.Input3==0){
               AppTask_Sensor_Status.Input3=1;
            }
           }
   }
else{
           if(Timerout_Counters[13]>0){
                 Timerout_Counters[13]--;
           }
           else{
              if(AppTask_Sensor_Status.Input3==1){
                 AppTask_Sensor_Status.Input3=0;
               }
           }
   }   
//-------------------------------------------------------------------------------------15# 
if(BSP_PIN2_07==0){    //
           if(Timerout_Counters[14]<5){
             Timerout_Counters[14]++;
           }
           else{
             if(AppTask_Sensor_Status.Input4==0){
               AppTask_Sensor_Status.Input4=1;  
             }
           }
   }
else{
           if(Timerout_Counters[14]>0){
                 Timerout_Counters[14]--;
           }
           else{
               if(AppTask_Sensor_Status.Input4==1){
                 AppTask_Sensor_Status.Input4=0;            
               }
           }
   } 
//-------------------------------------------------------------------------------------16# 
if(BSP_PIN2_08 == 0){     //
           if(Timerout_Counters[15]<5){
             Timerout_Counters[15]++;
           }
           else{
             if(AppTask_Sensor_Status.Input5==0){
               AppTask_Sensor_Status.Input5=1;
             }
           }
   }
else{
           if(Timerout_Counters[15]>0){
                 Timerout_Counters[15]--;
           }
           else{
               if(AppTask_Sensor_Status.Input5==1){
                 AppTask_Sensor_Status.Input5=0;      
               }
           }
   } 
//-------------------------------------------------------------------------------------17# 
if(BSP_PIN3_01==0){     //
           if(Timerout_Counters[16]<5){
            Timerout_Counters[16]++;
           }
           else{
             if(AppTask_Sensor_Status.Input6==0){
              AppTask_Sensor_Status.Input6=1;
              
            }
          }
  }
else{
          if(Timerout_Counters[16]>0){
                 Timerout_Counters[16]--;
          }
          else{
             if(AppTask_Sensor_Status.Input6==1){
                AppTask_Sensor_Status.Input6=0;             
              }
          }
   } 
//------------------------------------------------------------------------------------18# 
if(BSP_PIN3_02 == 0){   //
           if(Timerout_Counters[17]<2){
             Timerout_Counters[17]++;
           }
           else{
             if(AppTask_Sensor_Status.Input7==0){
               AppTask_Sensor_Status.Input7=1;
//               *(LVPtr_CoilData+NCoilAdd419)=1;  
             }
           }
   }
else{
           if(Timerout_Counters[17]>0){
                 Timerout_Counters[17]--;
           }
           else{
              if(AppTask_Sensor_Status.Input7==1){
                AppTask_Sensor_Status.Input7=0;
//                *(LVPtr_CoilData+NCoilAdd419)=0; 
              }
               
           }
   } 
//-------------------------------------------------------------------------------------19# 
if(BSP_PIN3_03==0){        //
           if(Timerout_Counters[18]<5){
             Timerout_Counters[18]++;
           }
           else{
             if(AppTask_Sensor_Status.Input8==0){
               AppTask_Sensor_Status.Input8=1;
//               *(LVPtr_CoilData+NCoilAdd421)=1;  
             }
           }
   }
else{
           if(Timerout_Counters[18]>0){
                 Timerout_Counters[18]--;
           }
           else{
               if(AppTask_Sensor_Status.Input8==1){
                 AppTask_Sensor_Status.Input8=0;
//                *(LVPtr_CoilData+NCoilAdd421)=0;             
               }
           }
   }    
//-------------------------------------------------------------------------------------20# 
if(BSP_PIN3_04==0){        //
           if(Timerout_Counters[19]<5){
             Timerout_Counters[19]++;
           }
           else{
             if(AppTask_Sensor_Status.Input9==0){
               AppTask_Sensor_Status.Input9=1;  
             }
           }
   }
else{
           if(Timerout_Counters[19]>0){
                 Timerout_Counters[19]--;
           }
           else{
               if(AppTask_Sensor_Status.Input9==1){
                 AppTask_Sensor_Status.Input9=0;             
               }
           }
   } 
//------------------------------------------------------------------------------------21#  
if(BSP_PIN3_05 == 0){      //
           if(Timerout_Counters[20]<2){
             Timerout_Counters[20]++;
           }
           else{
            if(AppTask_Sensor_Status.Input10==0){
               AppTask_Sensor_Status.Input10=1;
//             *(LVPtr_CoilData+NCoilAdd422)=1;  
            }
           }
   }
else{
           if(Timerout_Counters[20]>0){
                 Timerout_Counters[20]--;
           }
           else{
             if(AppTask_Sensor_Status.Input10==1){
                 AppTask_Sensor_Status.Input10=0;
//              *(LVPtr_CoilData+NCoilAdd422)=0;             
              }
           }
   } 
//-------------------------------------------------------------------------------------22#  
if(BSP_PIN3_06 == 0){      //    
           if(Timerout_Counters[21]<2){
             Timerout_Counters[21]++;
           }
           else{
             if(AppTask_Sensor_Status.Input11==0){
               AppTask_Sensor_Status.Input11=1;
//              *(LVPtr_CoilData+NCoilAdd423)=1;  
            }
           }
   }
else{
           if(Timerout_Counters[21]>0){
                 Timerout_Counters[21]--;
           }
           else{
              if(AppTask_Sensor_Status.Input11 == 1){
                 AppTask_Sensor_Status.Input11=0;            
              }
           }
   }    
//------------------------------------------------------------------------------------23#  
if(BSP_PIN3_07 == 0){      //
           if(Timerout_Counters[22]<5){
             Timerout_Counters[22]++;
           }
           else{
             if(AppTask_Sensor_Status.Input12==0){
               AppTask_Sensor_Status.Input12=1;   
             }
           }
   }
else{
           if(Timerout_Counters[22]>0){
                 Timerout_Counters[22]--;
           }
           else{
               if(AppTask_Sensor_Status.Input12 == 1){
                 AppTask_Sensor_Status.Input12=0;
                       
               }
           }
   }    
//-------------------------------------------------------------------------------------24#  
if(BSP_PIN3_08 == 0){       //
           if(Timerout_Counters[23]<5){
             Timerout_Counters[23]++;
           }
           else{
             if(AppTask_Sensor_Status.Input13==0){
               AppTask_Sensor_Status.Input13=1;
             }
           }
   }
else{
           if(Timerout_Counters[23]>0){
                 Timerout_Counters[23]--;
           }
           else{
               if(AppTask_Sensor_Status.Input13 == 1){
                 AppTask_Sensor_Status.Input13=0;         
               }
           }
   } 
//        Period++;
//        if(Period==100)
//        {
//            Period=0;
//             if(AppTask_Sensor_Status.HeatRunTimeEn==1)//加热倒计时
//             {
//               if(HeatRunTime>0)
//               {
//               HeatRunTime--;
//               *(Ptr_RegData+SRegisAdd83)=HeatRunTime; 
//               if( HeatRunTime==5)
//                { NVacuumOn(); }
//               }
//              else
//              {
//               HeatFinished=1;
//               AppTask_Sensor_Status.HeatRunTimeEn=0;
//              }  
//             *(Ptr_RegData+SRegisAdd83)=HeatRunTime; 
//             }
//            if(AppTask_Sensor_Status.Vacuum_PWaitEn==1)
//            {
//              if(Vacuum_PWait>0)
//              { 
//                Vacuum_PWait--;
//                if(Vacuum_PWait==0)
//                {
//                 AppTask_Sensor_Status.Vacuum_PWaitEn =0;
//                }
//              }
//            }
//            if(AppTask_Sensor_Status.RunTimeEn==1)//运行时间
//             {
//               RunTime++;
//               *(Ptr_RegData+SRegisAdd67)=RunTime; 
//             }
//            
//            if(AppTask_Sensor_Status.PressureLWaitEn==1)
//            {
//               if(OverPreWaitTimeL>0)
//               {
//                OverPreWaitTimeL--;
//               }
//              else
//              {
//               BeepOff();
//               *(Ptr_CoilData+NCoilAdd429)=0; 
//               AppTask_Sensor_Status.PressureLWaitEn=0;//停机时间到
//                     if(Step==15)
//                     {
//                       Woking_SpeedL=*(Ptr_RegData+SRegisAdd59);//继续走余程
//                       Woking_SpeedR=*(Ptr_RegData+SRegisAdd61);
//                         if(Woking_SpeedL<1)
//                         {
//                           Woking_SpeedL=1;
//                         }
//               
//                         if(Woking_SpeedR<1)
//                         {
//                            Woking_SpeedR=1;
//                         } 
//                         Motor1Starting_PWM2(DIR_CCW,Woking_SpeedR);
//                         Motor3Starting_PWM4(DIR_CCW,Woking_SpeedL);
//                     }
////                     else
////                     {
////                     Woking_SpeedL=*(Ptr_RegData+SRegisAdd63);//继续走余程
////                     Woking_SpeedR=*(Ptr_RegData+SRegisAdd65);
////                     }
//              
//              }  
//            }
//            if(AppTask_Sensor_Status.FinishedWaitTEn==1)
//            {
//                if(FinishedWaitTime>0)
//               {
//                FinishedWaitTime--;
//               }
//              else
//              {
//               WaitFinished=1;
//               AppTask_Sensor_Status.FinishedWaitTEn=0;
//              }  
//            }
//            
//        }
}

/////掉电保存，外部中断
//void EXTI0_Configuration(void) 
//{
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);     
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  		  	
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//        
//        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//        NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;	 
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//        NVIC_Init(&NVIC_InitStructure);	
//
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);	 
//	EXTI_DeInit();    
//	EXTI_InitStructure.EXTI_Line = EXTI_Line7 ;		
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ; 
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;		
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	 
//	EXTI_Init(&EXTI_InitStructure);
//        
//	EXTI_ClearITPendingBit(EXTI_Line7);
//        
//        BSP_IntVectSet(BSP_INT_ID_EXTI9_5,Protection_Data_Action );
//        BSP_IntEn(BSP_INT_ID_EXTI9_5);    
//}

                 
