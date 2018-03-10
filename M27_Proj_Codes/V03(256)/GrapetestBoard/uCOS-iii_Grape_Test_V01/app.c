/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*.
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : JJL
                  EHS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <app.h>

/*
*********************************************************************************************************
*                                             LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/
static  OS_SEM   AppSem; 

//启动任务
static  OS_TCB   AppTaskStartTCB; 
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

//通信控制任务
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
OS_TCB   AppTaskSerDataHandlingTCB; 
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static   CPU_STK  AppTaskSerDataHandlingStk[APP_TASK_SER_DATA_HANDLING_STK_SIZE];

//运动控制任务
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static   OS_TCB   AppTaskMotionControlTCB;
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static  CPU_STK  AppTaskMotionControlStk[APP_TASK_MOTION_CONTROL_STK_SIZE];

//LCD1602显示任务
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
OS_TCB   AppTaskSerCmdTCB; 
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static   CPU_STK  AppTask_Ser_Cmd_Stk[APP_TASK_Ser_Cmd_STK_SIZE];//1602显示任务

//输入输出IO状态显示 手动控制任务 
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
OS_TCB   AppTaskIoControlTCB; 
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static   CPU_STK  AppTaskIoControlStk[APP_TASK_Ser_Cmd_STK_SIZE];

//定时扫描任务
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static  OS_TMR   AppSensorScanTCB;

/*
*********************************************************************************************************
*                                            GLOBE VARIABLES
*********************************************************************************************************
*/

static uint8_t cmd[4]={'#','0','1',0x0d};

//电机控制
//CH1 相对行程
CPU_INT32U   CH1_Mtion_ABS_POS_Relative;
CPU_INT32U   CH1_Mtion_ABS_POS;
CPU_INT32U   L_Woking_Distance;

CPU_INT32U   CH3_Mtion_ABS_POS_Relative;
CPU_INT32U   CH3_Mtion_ABS_POS;
CPU_INT32U   R_Woking_Distance;

CPU_INT32U   R_Location_First; //
CPU_INT32U   R_Location_Second;//
CPU_INT32U   L_Location_First; //
CPU_INT32U   L_Location_Second;//

uint8_t *L_Press=(uint8_t *)&L_Press_Pressure;
uint8_t *R_Press=(uint8_t *)&R_Press_Pressure;
uint8_t *L_Pull =(uint8_t *)&L_Pull_Pressure;
uint8_t *R_Pull=(uint8_t *)&R_Pull_Pressure;
uint8_t *L_Pull_Max=(uint8_t *)&L_Pull_Max_Pressure;
uint8_t *R_Pull_Max=(uint8_t *)&R_Pull_Max_Pressure;

float L_Press_Pressure=0.0;//左电机压力报警值设定
float R_Press_Pressure=0.0;//右电机压力报警值设定
float L_Pull_Pressure=0.0;//左电机拉力报警值设定
float R_Pull_Pressure=0.0;//右电机拉力报警值设定
float L_Pull_Max_Pressure=0.0;//左电机最大拉力报警值设定
float R_Pull_Max_Pressure=0.0;//右电机最大拉力报警值设定

CPU_INT16U Manual_SpeedL,Manual_SpeedR;//手动操作电机速度
CPU_INT16U Auto_SpeedL,Auto_SpeedR;    //自动操作电机速度
CPU_INT16U Woking_SpeedL,Woking_SpeedR;//刮胶电机速度

CPU_INT32U Pass_Product_Counters;

//标志位
Fixture_Status_Flag   AppTask_Fixture_Status;   //App    应用·标志位
Sensor_Status_Flag    AppTask_Sensor_Status;    //Sensor 感应器·标志位


CPU_INT08U Step=0,MotorLBackFlag=0,MotorRBackFlag=0,DoorEnRecord=0;
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  AppTaskCreate (void);
static  void  AppTaskStart  (void *p_arg);
static  void  AppTaskSerDataHandling (void *p_arg);
static  void  AppTaskSerCmd(void *p_arg);
static  void  AppTaskIoControl(void *p_arg);
static  void  AppTaskMotionControl (void *p_arg);

uint32_t Conversion_fraction(uint8_t* Target, uint8_t* Source,uint8_t ADDR, uint32_t end_value);
void MotorTest(void);
void Led_Reverse(void);
static void ManualMotorL(void);
static void MotorLBack(void);
static void ManualMotorR(void);
static void MotorRBack(void);
static void IOControl(void);
static void SetMode(void);
static void MotorBack(void);    //双电机回原点
void Stop_Scan(void);
static void EmergencyButtonWasPressed(void);
static void EmergencyButtonWasReleased(void);

/*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************/
void main (void)   
{
    OS_ERR  err;


    BSP_IntDisAll();                                            /* Disable all inwterrupts.*/
       
    OSInit(&err); 
    /* Init uC/OS-III. */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"App Task Start",
                (OS_TASK_PTR )AppTaskStart, 
                (void       *)0,
               (OS_PRIO     )APP_TASK_START_PRIO,
                (CPU_STK    *)&AppTaskStartStk[0],
                (CPU_STK_SIZE)APP_TASK_START_STK_SIZE / 10,
                (CPU_STK_SIZE)APP_TASK_START_STK_SIZE,
                (OS_MSG_QTY  )0,
                (OS_TICK     )0,
                (void       *)0,
                (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */
}

void PrinfStringToScreen(uint8_t *ptr_str,uint16_t DIFF_ADDR)
{    
   uint8_t * lptr_str=ptr_str,i,j;
   CPU_INT16S   *lPtr_RegData;
   lPtr_RegData=RegData+DIFF_ADDR;
   
   for(i=0,j=0;*(lptr_str+i) != '\0';j++,i+=2){
     
      if(*(lptr_str+i+1) == '\0'){
         *(lPtr_RegData+j) = lptr_str[i]<<8;
      }
       else if(*(lptr_str+i) != '\0'){
         *(lPtr_RegData+j) = (lptr_str[i+1]<<8)|(lptr_str[i]);
       }     
    }
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq,cnts;
    OS_ERR      err;
    
    (void)p_arg;
    
    delay_Nms(500); //上电延时
    BSP_Init();
  
    AppTask_Sensor_Status.DoorEve=0;//光栅事件清空
    /* Initialize BSP functions                         */   
    App_OS_SetAllHooks();                                        //在J节拍中断服务中插入代码
    BSP_OS_SemCreate(&AppSem, 1, "Motion Control");              //创建一个  AppSem应用信号,控制马达动作                                                        
      
    cpu_clk_freq = BSP_CPU_ClkFreq();                             /* Determine SysTick reference freq.                */                                                                        
    cnts         = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;  /* Determine nbr SysTick increments                 */
    OS_CPU_SysTickInit(cnts);                                     /* Init uC/OS periodic time src (SysTick).          */
   
    AppTaskCreate();                        //创建应用任务
    
  while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.   */  
            Usart3SendCmd(cmd, 4 );
            delay_Nms(10); 
            Usart2SendCmd(cmd, 4 );
            OSTimeDlyHMSM( 24,
                          0,
                          0,
                          0,
                          OS_OPT_TIME_HMSM_STRICT,
                          &err);                
    }     
}

/*
*********************************************************************************************************
*                                          AppTaskCreate
*
* Description : 用于应用任务的创建
*
* Arguments   : p_arg   is the argument passed to 'AppTaskCreate()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
    OS_ERR      err;
  
    //创建任务一：用于串行通信，MODBUS
    //用中断采集数据
    OSTaskCreate((OS_TCB     *)&AppTaskSerDataHandlingTCB,           /* Create the SerDataHandling                   */
                 (CPU_CHAR   *)"App Task Ser Data Handling",              
                 (OS_TASK_PTR )AppTaskSerDataHandling, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_SER_DATA_HANDLING_PRIO,
                 (CPU_STK    *)&AppTaskSerDataHandlingStk[0],
                 (CPU_STK_SIZE)APP_TASK_SER_DATA_HANDLING_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_SER_DATA_HANDLING_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);      
    
     //创建任务二：用于串口查询 
     OSTaskCreate((OS_TCB     *)&AppTaskSerCmdTCB,           
                 (CPU_CHAR   *)"App Task Ser Cmd",              
                 (OS_TASK_PTR )AppTaskSerCmd, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_Ser_Cmd_PRIO,
                 (CPU_STK    *)&AppTask_Ser_Cmd_Stk[0],
                 (CPU_STK_SIZE)APP_TASK_Ser_Cmd_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_Ser_Cmd_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
     
      //创建任务三：用于数据采集
     //属性：定时任务,100hz
     //应用：1，AD数据采集，可以使用DMA传输方式，使用任务监控对应数据的变化;
     //      2，感应器信号采集
     
     OSTmrCreate ((OS_TMR     *)          &AppSensorScanTCB,
                  (CPU_CHAR   *)          "App Sensor Scan",
                  (OS_TICK     )          1,
                  (OS_TICK     )          1, 
                  (OS_OPT      )          OS_OPT_TMR_PERIODIC,
                  (OS_TMR_CALLBACK_PTR)   Fixture_Sensor_Scan,
                  (void*      )           0,
                  (OS_ERR    *)          &err);
       
     OSTmrStart  ((OS_TMR     *)          &AppSensorScanTCB,
                  (OS_ERR     *)          &err);
     
     //创建任务四：用于运动控制       
     OSTaskCreate((OS_TCB     *)&AppTaskMotionControlTCB,                   
                 (CPU_CHAR   *)"App Task Motion Control",              
                 (OS_TASK_PTR )AppTaskMotionControl, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_MOTION_CONTROL_PRIO,
                 (CPU_STK    *)&AppTaskMotionControlStk[0],
                 (CPU_STK_SIZE)APP_TASK_MOTION_CONTROL_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_MOTION_CONTROL_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
     
     //创建任务五：用于IO输入输出状态显示
     OSTaskCreate((OS_TCB     *)& AppTaskIoControlTCB,           
                 (CPU_CHAR   *)"App Task Io Control",              
                 (OS_TASK_PTR )AppTaskIoControl, 
                 (void       *)0,
                 (OS_PRIO     )APP_TASK_IO_CONTROL_PRIO,
                 (CPU_STK    *)&AppTaskIoControlStk[0],
                 (CPU_STK_SIZE)APP_TASK_Io_Control_STK_SIZE / 10,
                 (CPU_STK_SIZE)APP_TASK_Io_Control_STK_SIZE,
                 (OS_MSG_QTY  )0,
                 (OS_TICK     )0,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
}
/*
*********************************************************************************************************
*                                          AppTaskSerDataHandling
*
* Description : 串行接口数据处理
*
* Arguments   : p_arg   is the argument passed to 'AppTaskSerDataHandling()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskSerDataHandling (void *p_arg)
{
     OS_ERR      err;
     CPU_TS      ts;
     
     (void)p_arg;
    
     while (DEF_TRUE) {  
             
             OSTaskSemPend((OS_TICK )0,
                           (OS_OPT  )OS_OPT_PEND_BLOCKING,
                           (CPU_TS *)&ts,
                           (OS_ERR *)&err);
             if(err==OS_ERR_NONE){
             
                 checkComm0Modbus(); 
             }
             OSTimeDlyHMSM( 0,           
                            0,
                            0,
                            15,
                            OS_OPT_TIME_HMSM_STRICT,
                            &err);            
     
     }
}
/*
*********************************************************************************************************
*                                      AppTaskSerCmd
*
* Description : 串口命令处理
*
* Arguments   : p_arg   is the argument passed to 'AppTaskSerDataHandling()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskSerCmd(void *p_arg)
{
     OS_ERR      err;
    
     (void)p_arg;
     
     while (DEF_TRUE) {  
         
              OSTimeDlyHMSM( 0,           
                            0,
                            0,
                            12,
                            OS_OPT_TIME_HMSM_STRICT,
                            &err);            
     }
}
/*
*********************************************************************************************************
*                                          AppTaskIoControl
*
* Description : 用于IO输入输出状态显示 
*
* Arguments   : p_arg   is the argument passed to 'AppTaskMotionControl()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskIoControl(void *p_arg)
{
           OS_ERR      err;
           (void)p_arg; 
           while(DEF_TRUE){
             
            SetMode();         //按键参数
            InputSensorDis();  //输入传感器
            SetSysPara();      //设定参数
   
            OSTimeDlyHMSM(   0,    //周期性的调用AppTaskMotionControl
                             0,
                             0,
                             5,
                             OS_OPT_TIME_HMSM_STRICT,
                             &err);         
          }        
}
/*
*********************************************************************************************************
*                                          AppTaskMotionControl
*
* Description : 用于运动任务的控制
*
* Arguments   : p_arg   is the argument passed to 'AppTaskMotionControl()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  AppTaskMotionControl (void *p_arg)
{
      OS_ERR      err;
      (void)p_arg;  
      
      Checkparama_AND_Inital(); //初始化modus相关参数
      
      Reset_System();                
      EmergencyButtonWasPressed();   //急停被按下
      EmergencyButtonWasReleased();  //松开急停处理
      
      while(DEF_TRUE){
        
        ControlSequence();
        
        OSTimeDlyHMSM(   0,    //周期性的调用AppTaskMotionControl
                      0,
                      0,
                      3,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);         
      }        
}

void Stop_Scan(void)
{
  if(BSP_PIN1_01==1)//急停按钮按下
  {                                                     
    if(AppTask_Sensor_Status.Emergency==0)
    {
      AppTask_Sensor_Status.Emergency=1;
      AppTask_Sensor_Status.EmeEvent=1;
    }
  }
  else
  {
    if(AppTask_Sensor_Status.Emergency==1)
    {
      AppTask_Sensor_Status.Emergency=0;          
    }
  }
  if(BSP_PIN1_03==0)
  {    //左启动按钮    
    if(AppTask_Sensor_Status.L_Start==0)
    {
      AppTask_Sensor_Status.L_Start=1;
    }
    
  }
  else
  {
    if(AppTask_Sensor_Status.L_Start==1)
    {
      AppTask_Sensor_Status.L_Start=0; 
    }
    
  }
  if(BSP_PIN1_04==0){    //右启动按钮   
    if(AppTask_Sensor_Status.R_Start==0){
      AppTask_Sensor_Status.R_Start=1;
    }
    
  }
  else{
    if(AppTask_Sensor_Status.R_Start==1)
    {
      AppTask_Sensor_Status.R_Start=0; 
    }
    
  } 
  //=================================
  if(BSP_PIN2_04==1)//预留输入1
  {    
    if(AppTask_Sensor_Status.DoorDet==0)
    {
      AppTask_Sensor_Status.DoorDet=1;
    }
  }
  else
  {
    if(AppTask_Sensor_Status.DoorDet==1)
    {
      AppTask_Sensor_Status.DoorDet=0;
    }
  } 
  if(AppTask_Sensor_Status.DoorEve==1)
  {
    if(AppTask_Sensor_Status.R_Start==1&&AppTask_Sensor_Status.L_Start==1)
      AppTask_Sensor_Status.DoorEve=0;
  } 
  if(BSP_PIN1_02 ==0){ //复位按钮按下
    
    if(AppTask_Sensor_Status.Reset==0){
      AppTask_Sensor_Status.Reset=1;
    }
    if( AppTask_Sensor_Status.EmeEvent==1)
    {
      AppTask_Sensor_Status.Reset_Event=1; 
    }
  }
  else{
    
    if(AppTask_Sensor_Status.Reset==1){
      AppTask_Sensor_Status.Reset=0; 
    }
    
  }
}

void IOControl(void)
{
 
    CPU_INT08U   *Ptr_CoilData;
    Ptr_CoilData=CoilData;               //触控 按键事件 处理
    if(Coil.Add>0)          
    {            
      //按钮状态扫描------------------------------------------------------------
      switch(Coil.Add)    
      {
        //强制端口输出
      case SCoilAdd1: //自动手动模式切换
        {
          if(*(Ptr_CoilData+SCoilAdd1) == 1)
          {
            BSP_POUT1_01=0;   
          }
          else
          {
            BSP_POUT1_01=1; 
          }
          
        }break;   
      case SCoilAdd2://真空泵
        {
          if(*(Ptr_CoilData+SCoilAdd2) == 1)
          {
            BSP_POUT1_02=0;   
          }
          else
          {
            BSP_POUT1_02=1;   
          } 
        }break;  
      case SCoilAdd3:   //上真空吸
        {
          if(*(Ptr_CoilData+SCoilAdd3) == 1)
          {
            BSP_POUT1_03=0;   
          }
          else
          {
            BSP_POUT1_03=1;   
          } 
          
        }break;   
      case SCoilAdd4:   //下真空吸
        {
          if(*(Ptr_CoilData+SCoilAdd4) == 1)
          {
            BSP_POUT1_04=0;   
          }
          else
          {
            BSP_POUT1_04=1;   
          }
        }break;   
      case SCoilAdd5:   //设备加热控制
        {
          if(*(Ptr_CoilData+SCoilAdd5) == 1)
          {
            BSP_POUT1_05=0;   
          }
          else
          {
            BSP_POUT1_05=1;    
          }
        }break;   
      case SCoilAdd6:   //蜂鸣器控制
        {
          if(*(Ptr_CoilData+SCoilAdd6) == 1)
          {
            BSP_POUT1_06=0;   
          }
          else
          {
            BSP_POUT1_06=1;   
          }
          
        }break;   
      case SCoilAdd7:   //复位灯控制
        {
          if(*(Ptr_CoilData+SCoilAdd7) == 1)
          {
            BSP_POUT1_07=0;   
          }
          else
          {
            BSP_POUT1_07=1;   
          }
        }break; 
      case SCoilAdd8:   //左启灯控制
        {
          if(*(Ptr_CoilData+SCoilAdd8) == 1)
          {
            BSP_POUT1_08=0;   
          }
          else
          {
            BSP_POUT1_08=1;   
          }
        }break;
        
      case SCoilAdd9:   //右启动灯控制
        {
          if(*(Ptr_CoilData+SCoilAdd9) == 1)
          {
            BSP_POUT2_01=0;   
          }
          else
          {
            BSP_POUT2_01=1;    
          }
        }break; 
      case SCoilAdd10:   //左电机向上运动
        {
          if(*(Ptr_CoilData+SCoilAdd10) == 1)
          {
            BSP_POUT2_02=0;   
          }
          else
          {
            BSP_POUT2_02=1;    
          }
        }break;      
      case SCoilAdd11:   //左电机向下运动
        {
          if(*(Ptr_CoilData+SCoilAdd11) == 1)
          {
            BSP_POUT2_03=0;   
          }
          else
          {
            BSP_POUT2_03=1;    
          }  
        }break;   
      case SCoilAdd12:  //左电机回原点
        {
          if(*(Ptr_CoilData+SCoilAdd12) == 1)
          {
            BSP_POUT2_04=0; 
          }
          else
          {
            BSP_POUT2_04=1;   
          }
        }break;   
      case SCoilAdd13:  //右电机向上运动
        {
          if(*(Ptr_CoilData+SCoilAdd13) == 1)
          {
            BSP_POUT2_05=0; 
          }
          else
          {
            BSP_POUT2_05=1;   
          }
        }break;  
      case SCoilAdd14:  //右电机向下运动
        {
          if(*(Ptr_CoilData+SCoilAdd14) == 1)
          {
            BSP_POUT2_06=0; 
          }
          else
          {
            BSP_POUT2_06=1;   
          }
        }break;   
      case SCoilAdd15:  //右电机回原点
        {
          if(*(Ptr_CoilData+SCoilAdd15) == 1)
          {
            BSP_POUT2_07=0; 
          }
          else
          {
            BSP_POUT2_07=1;   
          }
        }break; 
      case SCoilAdd16:  
        {
          if(*(Ptr_CoilData+SCoilAdd16) == 1)
          {
            BSP_POUT2_08=0; 
          }
          else
          {
            BSP_POUT2_08=1;   
          }
        }break;
        
        
      case SCoilAdd17:   //右启动灯控制
        {
          if(*(Ptr_CoilData+SCoilAdd17) == 1)
          {
            BSP_POUT3_01=0;   
          }
          else
          {
            BSP_POUT3_01=1;    
          }
        }break; 
      case SCoilAdd18:   //左电机向上运动
        {
          if(*(Ptr_CoilData+SCoilAdd18) == 1)
          {
            BSP_POUT3_02=0;   
          }
          else
          {
            BSP_POUT3_02=1;    
          }
        }break;      
      case SCoilAdd19:   //左电机向下运动
        {
          if(*(Ptr_CoilData+SCoilAdd19) == 1)  
          {
            BSP_POUT3_03=0;   
          }
          else
          {
            BSP_POUT3_03=1;    
          }  
        }break;   
      case SCoilAdd20:  //左电机回原点
        {
          if(*(Ptr_CoilData+SCoilAdd20) == 1)
          {
            BSP_POUT3_04=0; 
          }
          else
          {
            BSP_POUT3_04=1;   
          }
        }break;   
      case SCoilAdd21:  //右电机向上运动
        {
          if(*(Ptr_CoilData+SCoilAdd21) == 1)
          {
            BSP_POUT3_05=0; 
          }
          else
          {
            BSP_POUT3_05=1;   
          }
        }break;  
      case SCoilAdd22:  //右电机向下运动
        {
          if(*(Ptr_CoilData+SCoilAdd22) == 1)
          {
            BSP_POUT3_06=0; 
          }
          else
          {
            BSP_POUT3_06=1;   
          }
        }break;   
      case SCoilAdd23:  //右电机回原点
        {
          if(*(Ptr_CoilData+SCoilAdd23) == 1)
          {
            BSP_POUT3_07=0; 
          }
          else
          {
            BSP_POUT3_07=1;   
          }
        }break; 
      case SCoilAdd24:  
        {
          if(*(Ptr_CoilData+SCoilAdd24) == 1)
          {
            BSP_POUT3_08=0; 
          }
          else
          {
            BSP_POUT3_08=1;   
          }
        }break;
        
        Coil.Add=0;
        Coil.num=0;		
      }  
      
    }

}
void InputSensorDis(void)//输入传感器状态显示
{
    CPU_INT08U   *Ptr_CoilData;
    Ptr_CoilData=CoilData;
    
    if(AppTask_Sensor_Status.Emergency==1)//急停按钮
    {
      *(Ptr_CoilData+NCoilAdd402)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd402)=0;
    }    
    if(AppTask_Sensor_Status.Reset==1)//复位按钮 
    {
      *(Ptr_CoilData+NCoilAdd403)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd403)=0;
    }
    if(AppTask_Sensor_Status.L_Start==1)//左启动按钮 
    {
      *(Ptr_CoilData+NCoilAdd404)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd404)=0;
    }
    if(AppTask_Sensor_Status.R_Start==1)//左启动按钮 
    {
      *(Ptr_CoilData+NCoilAdd405)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd405)=0;
    }
    if(AppTask_Sensor_Status.L_Motor_P==1)//左电机上限位 
    {
      *(Ptr_CoilData+NCoilAdd406)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd406)=0;
    } 
    if(AppTask_Sensor_Status.L_Motor_N==1)//左电机下限位 
    {
      *(Ptr_CoilData+NCoilAdd407)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd407)=0;
    }    
    if(AppTask_Sensor_Status.R_Motor_P==1)//右电机上限位 左启动
    {
      *(Ptr_CoilData+NCoilAdd408)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd408)=0;
    }
    if(AppTask_Sensor_Status.R_Motor_N==1)//右电机下限位 右启动
    {
      *(Ptr_CoilData+NCoilAdd409)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd409)=0;
    } 
    if(AppTask_Sensor_Status.Vacuum_P==1)//上真空表
    {
      *(Ptr_CoilData+NCoilAdd410)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd410)=0;
    } 
    if(AppTask_Sensor_Status.Vacuum_N==1)//下真空表
    {
      *(Ptr_CoilData+NCoilAdd411)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd411)=0;
    }
    if(AppTask_Sensor_Status.Heat_Start==1)//加热按钮
    {
      *(Ptr_CoilData+NCoilAdd412)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd412)=0;
    }
    if(AppTask_Sensor_Status.DoorDet==1&&AppTask_Sensor_Status.DoorEn==ON)//光栅检测
    {
      
      if(Step>4&&Step<10) //拆卸过程光栅无效
      { 
        *(Ptr_CoilData+NCoilAdd413)=0;
      }
      else
      {
        *(Ptr_CoilData+NCoilAdd413)=1;
      }
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd413)=0;
    }
    if(AppTask_Sensor_Status.Input2==1)//预留输入2
    {
      *(Ptr_CoilData+NCoilAdd414)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd414)=0;
    }
    if(AppTask_Sensor_Status.Input3==1)//预留输入3
    {
      *(Ptr_CoilData+NCoilAdd415)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd415)=0;
    }
    if(AppTask_Sensor_Status.Input4==1)//预留输入4
    {
      *(Ptr_CoilData+NCoilAdd416)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd416)=0;
    }
    if(AppTask_Sensor_Status.Input5==1)//预留输入5
    {
      *(Ptr_CoilData+NCoilAdd417)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd417)=0;
    }
    if(AppTask_Sensor_Status.Input6==1)//预留输入6
    {
      *(Ptr_CoilData+NCoilAdd418)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd418)=0;
    }
    if(AppTask_Sensor_Status.Input7==1)//预留输入7
    {
      *(Ptr_CoilData+NCoilAdd419)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd419)=0;
    }
    if(AppTask_Sensor_Status.Input8==1)//预留输入8
    {
      *(Ptr_CoilData+NCoilAdd420)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd420)=0;
    }
    if(AppTask_Sensor_Status.Input9==1)//预留输入9
    {
      *(Ptr_CoilData+NCoilAdd421)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd421)=0;
    }
    if(AppTask_Sensor_Status.Input10==1)//预留输入10
    {
      *(Ptr_CoilData+NCoilAdd422)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd422)=0;
    }
    if(AppTask_Sensor_Status.Input11==1)//预留输入11
    {
      *(Ptr_CoilData+NCoilAdd423)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd423)=0;
    }
    if(AppTask_Sensor_Status.Input12==1)//预留输入12
    {
      *(Ptr_CoilData+NCoilAdd424)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd424)=0;
    }
    if(AppTask_Sensor_Status.Input13==1)//预留输入13
    {
      *(Ptr_CoilData+NCoilAdd425)=1;
    }
    else
    {
      *(Ptr_CoilData+NCoilAdd425)=0;
    }
}

void SetOutput(void)//手动输出控制
{

   CPU_INT16S   *Ptr_RegData;
     Ptr_RegData=RegData;
     CPU_INT08U   *Ptr_CoilData;
     Ptr_CoilData=CoilData;               //触控 按键事件 处理
     if(Coil.Add>0)          
     {            
       //按钮状态扫描------------------------------------------------------------
       switch(Coil.Add)    
       {
         //强制端口输出  
       case SCoilAdd2:   //上真空吸
         {
           if(*(Ptr_CoilData+SCoilAdd2) == 1)
           {
             PVacuumOn();
           }
           else
           {
             PVacuumOff(); 
           } 
           
         }break;   
       case SCoilAdd3:   //下真空吸
         {
           if(*(Ptr_CoilData+SCoilAdd3) == 1)
           {
             NVacuumOn();
           }
           else
           {
             NVacuumOff(); 
           }
         }break; 
       case SCoilAdd4://真空泵
         {
           if(*(Ptr_CoilData+SCoilAdd4) == 1)
           {
             //  VacuumOn();
           }
           else
           {
             // VacuumOff(); 
           } 
         }break; 
       case SCoilAdd5:   //设备加热控制
         {
           if(*(Ptr_CoilData+SCoilAdd5) == 1)
           {
             HeatOn();
           }
           else
           {
             HeatOff();  
           }
         }break;
       case SCoilAdd6:   //左启灯控制
         {
           if(*(Ptr_CoilData+SCoilAdd6) == 1)
           {
             LStartLedOn();
           }
           else
           {
             LStartLedOff(); 
           }
         }break;
       case SCoilAdd7:   //右启动灯控制
         {
           if(*(Ptr_CoilData+SCoilAdd7) == 1)
           {
             RStartLedOn();
           }
           else
           {
             RStartLedOff(); 
           }
         }break;
       case SCoilAdd8:   //复位灯控制
         {
           if(*(Ptr_CoilData+SCoilAdd8) == 1)
           {
             ResLedOn() ;
           }
           else
           {
             ResLedOff(); 
           }
         }break;
       case SCoilAdd9:   //蜂鸣器控制
         {
           if(*(Ptr_CoilData+SCoilAdd9) == 1)
           {
             BeepOn();
           }
           else
           {
             BeepOff();  
           }
           
         }break;   
       case SCoilAdd10:   //左电机向上运动
         {
           if((*(Ptr_CoilData+SCoilAdd10) == 1)&&(0==AppTask_Sensor_Status.L_Motor_P)&&(0==AppTask_Sensor_Status.DoorDet)&&(0==AppTask_Sensor_Status.Emergency))
           { 
             LMotorOn(); 
             AppTask_Sensor_Status.LMotorUp=1;//使能记录距离标志位
             ManualMotorL();
             
           }
           else
           {
             MotorStop(TIM2);
             AppTask_Sensor_Status.LMotorUp=0;
           }
         }break;      
       case SCoilAdd11:   //左电机向下运动
         {
           if((*(Ptr_CoilData+SCoilAdd11) == 1)&&(0==AppTask_Sensor_Status.L_Motor_N)&&(0==AppTask_Sensor_Status.DoorDet)&&(0==AppTask_Sensor_Status.Emergency))
           { 
             AppTask_Sensor_Status.LMotorDown=1;//使能记录距离标志位
             LMotorOn(); 
             ManualMotorL();
             
           }
           else
           {
             MotorStop(TIM2);
             AppTask_Sensor_Status.LMotorDown=0;
           }
         }break;   
       case SCoilAdd12:  //左电机回原点
         {
           if((*(Ptr_CoilData+SCoilAdd12) == 1)&&(0==AppTask_Sensor_Status.L_Motor_P))
           {
             MotorLBackFlag=1;
             MotorRBackFlag=1;
             AppTask_Sensor_Status.LMotorUp=1;//使能记录距离标志位
             AppTask_Sensor_Status.RMotorUp=1;
           }
           else
           {
             
           }
         }break;   
       case SCoilAdd13:  //右电机向上运动
         {
           if((*(Ptr_CoilData+SCoilAdd13) == 1)&&(0==AppTask_Sensor_Status.R_Motor_P)&&(0==AppTask_Sensor_Status.DoorDet)&&(0==AppTask_Sensor_Status.Emergency))
           {
             AppTask_Sensor_Status.RMotorUp=1;//使能记录距离标志位
             RMotorOn(); 
             ManualMotorR();
             
           }
           else
           {
             MotorStop(TIM3); 
             AppTask_Sensor_Status.RMotorUp=0;
           }
         }break;  
       case SCoilAdd14:  //右电机向下运动
         {
           if((*(Ptr_CoilData+SCoilAdd14) == 1)&&(0==AppTask_Sensor_Status.R_Motor_N)&&(0==AppTask_Sensor_Status.DoorDet)&&(0==AppTask_Sensor_Status.Emergency))
           {
             AppTask_Sensor_Status.RMotorDown=1;//使能记录距离标志位
             RMotorOn(); 
             ManualMotorR();
             
           }
           else
           {
             MotorStop(TIM3); 
             AppTask_Sensor_Status.RMotorDown=0;
           }
         }break;   
       case SCoilAdd15:  //右电机回原点
         {
           if((*(Ptr_CoilData+SCoilAdd15) == 1)&&(0==AppTask_Sensor_Status.R_Motor_P))
           {
             MotorLBackFlag=1;
             MotorRBackFlag=1;
             AppTask_Sensor_Status.LMotorUp=1;//使能记录距离标志位
             AppTask_Sensor_Status.RMotorUp=1;
           }
           else
           {
             
           } 
           
         }break;  
       case SCoilAdd16:  //左电机拆屏前位置保存
         {
           if(*(Ptr_CoilData+SCoilAdd16) ==1)
           {
             L_Location_First=CH3_Mtion_ABS_POS;
             IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd13),&L_Location_First);
             *(Ptr_RegData+SRegisAdd14)=L_Location_First*25&0x0000ffff;
             *(Ptr_RegData+SRegisAdd13)=L_Location_First*25>>16;
           }
           else
           {
             
           } 
           
         }break;  
       case SCoilAdd17:  //左电机拆屏完成位置保存
         {
           if(*(Ptr_CoilData+SCoilAdd17) == 1)
           {
             L_Location_Second=L_Location_First-CH3_Mtion_ABS_POS;
             IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd17),&L_Location_Second);
             *(Ptr_RegData+SRegisAdd18)=L_Location_Second*25&0x0000ffff;
             *(Ptr_RegData+SRegisAdd17)=L_Location_Second*25>>16;
           }
           else
           {
             
           } 
           
         }break; 
       case SCoilAdd18:  //右电机拆屏前位置保存
         {
           if(*(Ptr_CoilData+SCoilAdd18) ==1)
           {
             R_Location_First=CH1_Mtion_ABS_POS;
             IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd21),&R_Location_First);
             *(Ptr_RegData+SRegisAdd22)=R_Location_First*25&0x0000ffff;
             *(Ptr_RegData+SRegisAdd21)=R_Location_First*25>>16;
           }
           else
           {
             
           } 
           
         }break;  
       case SCoilAdd19:  //右电机拆屏完成位置保存
         {
           if(*(Ptr_CoilData+SCoilAdd19) == 1)
           {
             R_Location_Second=R_Location_First-CH1_Mtion_ABS_POS;
             IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd25),&R_Location_Second);
             *(Ptr_RegData+SRegisAdd26)=R_Location_Second*25&0x0000ffff;
             *(Ptr_RegData+SRegisAdd25)=R_Location_Second*25>>16;
           }
           else
           {
           } 
         }break; 
         Coil.Add=0;
         Coil.num=0;		
       }  
       
     }
 }
void SetMode(void)
{
    static CPU_INT08U SaveFlag1=0,SaveFlag2=0,SaveFlag3=0,SaveFlag4=0,SaveFlag5=0,SaveFlag6=0;
    CPU_INT08U tempData;
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    CPU_INT08U   *Ptr_CoilData;
    Ptr_CoilData=CoilData;               //触控 按键事件 处理
    if(Coil.Add>0)          
    {            
      //按钮状态扫描------------------------------------------------------------
      switch(Coil.Add)    
      {
        //强制端口输出
      case SCoilAdd1: //自动手动模式切换
        {
          if(*(Ptr_CoilData+SCoilAdd1) == 0)
          {
            if(Step==1||Step==0)//电机在原点 状态机在初始位置 模式切换有效
            {
              AppTask_Sensor_Status.Fixture_Mode=AUTO;//自动模式
            }
          }
          else
          {
            if(Step==1||Step==0)
            {
              AppTask_Sensor_Status.Fixture_Mode=HAND;//手动模式 
            }
          }
          
        }break;
      case SCoilAdd20: //光栅检测使能
        {
          if(*(Ptr_CoilData+SCoilAdd20) == 1)
          {
            AppTask_Sensor_Status.DoorEn=ON;//打开
            SaveFlag2=0;
            if(SaveFlag1==0)
            {
              tempData=0xff;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd20),&tempData);
              SaveFlag1=1;
            }
          }
          else
          {
            AppTask_Sensor_Status.DoorEn=OFF;//关闭
            SaveFlag1=0;
            if(SaveFlag2==0)
            {
              tempData=0x00;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd20),&tempData);
              SaveFlag2=1;
            }
          }
          
        }break; 
      case SCoilAdd21:  //加热下真空开使能
        {
          if(*(Ptr_CoilData+SCoilAdd21) == 1)
          {
            AppTask_Sensor_Status.Heat_Vacuum_N=ON;
            SaveFlag4=0;
            if(SaveFlag3==0)
            {
              tempData=0xff;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd21),&tempData);
              SaveFlag3=1;
            }
          }
          else
          {
            AppTask_Sensor_Status.Heat_Vacuum_N=OFF;
            SaveFlag3=0;
            if(SaveFlag4==0)
            {
              tempData=0x00;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd21),&tempData);
              SaveFlag4=1;
            }
          } 
        }break; 
      case SCoilAdd22:  //产量清零
        {
          if(*(Ptr_CoilData+SCoilAdd22) == 1)
          {
            Pass_Product_Counters=0;
            IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd69),&Pass_Product_Counters);
            *(Ptr_RegData+SRegisAdd70)=Pass_Product_Counters&0x0000ffff;
            *(Ptr_RegData+SRegisAdd69)=Pass_Product_Counters>>16;
          }
          else
          {
          } 
        }break; 
      case SCoilAdd23: //
        {
          if(*(Ptr_CoilData+SCoilAdd23) == 1)
          {
            AppTask_Sensor_Status.DoorMode=ON;//打开
            SaveFlag6=0;
            if(SaveFlag5==0)
            {
              tempData=0xff;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd23),&tempData);
              SaveFlag5=1;
            }
          }
          else
          {
            AppTask_Sensor_Status.DoorMode=OFF;//关闭
            SaveFlag5=0;
            if(SaveFlag6==0)
            {
              tempData=0x00;
              IIC_Write_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd23),&tempData);
              SaveFlag6=1;
            }
          }
          
        }break; 
        Coil.Add=0;
        Coil.num=0;		
      }  
    }
}

void SetSysPara(void)//参数读取
{
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    CPU_INT16S ValH,ValL;
    
    if(Rdis.Add > 0){
      
      switch(Rdis.Add)  
      {
      case SRegisAdd1:{   //左电机手动速度
        Manual_SpeedL=*(Ptr_RegData+SRegisAdd1); 
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd1),&Manual_SpeedL); 
      }break; 
      case SRegisAdd3:{   //右电机手动速度
        Manual_SpeedR=*(Ptr_RegData+SRegisAdd3); 
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd3),&Manual_SpeedR); 
      }break; 
      case SRegisAdd29:{   //右最大拉力报警值
        ValH= *(Ptr_RegData+SRegisAdd29);
        ValL= *(Ptr_RegData+SRegisAdd30);
        R_Press[3]=ValH>>8;
        R_Press[2]=ValH&0xFF;
        R_Press[1]=ValL>>8;
        R_Press[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd29),&R_Press_Pressure);                       
      }break;
      case SRegisAdd33:{   //右压力报警值
        ValH= *(Ptr_RegData+SRegisAdd33);
        ValL= *(Ptr_RegData+SRegisAdd34);
        R_Pull[3]=ValH>>8;
        R_Pull[2]=ValH&0xFF;
        R_Pull[1]=ValL>>8;
        R_Pull[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd33),&R_Pull_Pressure);      
      }break;
      case SRegisAdd37:{   //右拆机拉力报警值
        ValH= *(Ptr_RegData+SRegisAdd37);
        ValL= *(Ptr_RegData+SRegisAdd38);
        R_Pull_Max[3]=ValH>>8;
        R_Pull_Max[2]=ValH&0xFF;
        R_Pull_Max[1]=ValL>>8;
        R_Pull_Max[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd37),&R_Pull_Max_Pressure);      
        
      }break;
      case SRegisAdd41:{   //左最大拉力报警值
        ValH= *(Ptr_RegData+SRegisAdd41);
        ValL= *(Ptr_RegData+SRegisAdd42);
        L_Press[3]=ValH>>8;
        L_Press[2]=ValH&0xFF;
        L_Press[1]=ValL>>8;
        L_Press[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd41),&L_Press_Pressure);      
      }break;
      case SRegisAdd45:{   //左压力报警值
        ValH= *(Ptr_RegData+SRegisAdd45);
        ValL= *(Ptr_RegData+SRegisAdd46);
        L_Pull[3]=ValH>>8;
        L_Pull[2]=ValH&0xFF;
        L_Pull[1]=ValL>>8;
        L_Pull[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd45),&L_Pull_Pressure);            
      }break;   
      case SRegisAdd49:{   //左拆机拉力报警值
        ValH= *(Ptr_RegData+SRegisAdd49);
        ValL= *(Ptr_RegData+SRegisAdd50);
        L_Pull_Max[3]=ValH>>8;
        L_Pull_Max[2]=ValH&0xFF;
        L_Pull_Max[1]=ValL>>8;
        L_Pull_Max[0]=ValL&0xFF;
        IIC_Write_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd49),&L_Pull_Max_Pressure);      
      }break; 
      case SRegisAdd53:{   //加热时间
//        HeatTime=*(Ptr_RegData+SRegisAdd53);
//        *(Ptr_RegData+SRegisAdd83)=HeatTime;
//        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd53),&HeatTime); 
      }break;
      case SRegisAdd55:{   //拆机拉力报警停机时间
        OverPreWaitTimeL=*(Ptr_RegData+SRegisAdd55);
        OverPreWaitTimeR=*(Ptr_RegData+SRegisAdd55);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd55),&OverPreWaitTimeL); 
      }break;
      case SRegisAdd57:{   //拆机完成等待时间
        FinishedWaitTime=*(Ptr_RegData+SRegisAdd57);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd57),&FinishedWaitTime);    
      }break; 
      case SRegisAdd59:{   //左电机自动速度
        Auto_SpeedL=*(Ptr_RegData+SRegisAdd59);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd59),&Auto_SpeedL);  
      }break;
      case SRegisAdd61:{   //右电机自动速度
        Auto_SpeedR=*(Ptr_RegData+SRegisAdd61);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd61),&Auto_SpeedR); 
      }break;
      case SRegisAdd63:{   //左拆机速度
        Woking_SpeedL=*(Ptr_RegData+SRegisAdd63);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd63),&Woking_SpeedL);   
      }break; 
      case SRegisAdd65:{   //右拆机速度
        Woking_SpeedR=*(Ptr_RegData+SRegisAdd65);
        IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd65),&Woking_SpeedR);  
      }break;
      
      }
      Rdis.Add=0;
    }	   
		
}
static void MotorSpeed(void)
{
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    if(Step==4)
    {
      Auto_SpeedL=*(Ptr_RegData+SRegisAdd59);
      if(Auto_SpeedL<1)
      {
        Auto_SpeedL=1;
      }
      Auto_SpeedR=*(Ptr_RegData+SRegisAdd61);
      if(Auto_SpeedR<1)
      {
        Auto_SpeedR=1;
      } 
      Motor3Starting_PWM4(DIR_CW,Auto_SpeedL); 
      Motor1Starting_PWM2(DIR_CW,Auto_SpeedR); 
    }
    else if(Step==9)
    {
      Woking_SpeedL=*(Ptr_RegData+SRegisAdd63);
      if(Woking_SpeedL<1)
      {
        Woking_SpeedL=1;
      }
      Woking_SpeedR=*(Ptr_RegData+SRegisAdd65);
      if(Woking_SpeedR<1)
      {
        Woking_SpeedR=1;
      } 
      Motor3Starting_PWM4(DIR_CCW,Woking_SpeedL);  
      Motor1Starting_PWM2(DIR_CCW,Woking_SpeedR); 
    }
    else if(Step==12)
    {
      Woking_SpeedL=*(Ptr_RegData+SRegisAdd63);
      if(Woking_SpeedL<1)
      {
        Woking_SpeedL=1;
      }
      Woking_SpeedR=*(Ptr_RegData+SRegisAdd65);
      if(Woking_SpeedR<1)
      {
        Woking_SpeedR=1;
      } 
      Motor3Starting_PWM4(DIR_CW,Woking_SpeedL);     
      Motor1Starting_PWM2(DIR_CW,Woking_SpeedR);   
    }
    else
    {
      //MotorStop_PWM5(TIM5);  
    }
}

static void ManualMotorL(void)//手动控制左电机
{
  OS_ERR      err;
  CPU_INT08U MotorVal=0;
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  CPU_INT08U   *Ptr_CoilData;
  Ptr_CoilData=CoilData;    
  while((((*(Ptr_CoilData+SCoilAdd10) == 1)&&(0==AppTask_Sensor_Status.L_Motor_P))||((*(Ptr_CoilData+SCoilAdd11) == 1)&&0==AppTask_Sensor_Status.L_Motor_N))&&(DoorSafety()==SAFE))
  {
    if(MotorVal==0)
    {
      Manual_SpeedL=*(Ptr_RegData+SRegisAdd1);
      if(Manual_SpeedL<1)Manual_SpeedL=1;
      if((*(Ptr_CoilData+SCoilAdd10) == 1))//左电机手动向上运动
      {
        Motor3Starting_PWM4(DIR_CCW,Manual_SpeedL);
      }
      
      if((*(Ptr_CoilData+SCoilAdd11) == 1)) 
      {
        Motor3Starting_PWM4(DIR_CW,Manual_SpeedL);
      }
      MotorVal=1; 
    } 
    OSTimeDlyHMSM( 0,    
                  0,
                  0,
                  5,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);   
  } 
}

static void MotorLBack(void)//手动控制左电机回原地
{
  OS_ERR      err;
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  if(MotorLBackFlag)
  {
    AppTask_Sensor_Status.LMotorOriStopDet=ON; 
    Manual_SpeedL=*(Ptr_RegData+SRegisAdd1);
    if(Manual_SpeedL<1)Manual_SpeedL=1;
    Motor3Starting_PWM4(DIR_CCW,Manual_SpeedL);
  }
  while((MotorLBackFlag)&&(DoorSafety()==SAFE)&&(0==AppTask_Sensor_Status.Emergency))
  {
    if(1==AppTask_Sensor_Status.L_Motor_P)
    {
      MotorStop(TIM2);
      CH3_Mtion_ABS_POS=0;
      AppTask_Sensor_Status.LMotorOriStopDet=OFF;
      MotorLBackFlag=0; 
      AppTask_Sensor_Status.LMotorUp=0;//使能记录距离标志位
    }
    OSTimeDlyHMSM( 0,    
                  0,
                  0,
                  5,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err); 
  }
}

static void ManualMotorR(void)//手动控制右电机
{
  OS_ERR      err;
  CPU_INT08U MotorVal=0;
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  CPU_INT08U   *Ptr_CoilData;
  Ptr_CoilData=CoilData;    
  while((((*(Ptr_CoilData+SCoilAdd13) == 1)&&(0==AppTask_Sensor_Status.R_Motor_P))||((*(Ptr_CoilData+SCoilAdd14) == 1)&&0==AppTask_Sensor_Status.R_Motor_N))&&(DoorSafety()==SAFE))
  {
    if(MotorVal==0)
    {
      Manual_SpeedR=*(Ptr_RegData+SRegisAdd3);
      if(Manual_SpeedR<1)Manual_SpeedR=1;
      if((*(Ptr_CoilData+SCoilAdd13) == 1))//左电机手动向上运动
      {
        Motor1Starting_PWM2(DIR_CCW,Manual_SpeedR);   
      }
      
      if((*(Ptr_CoilData+SCoilAdd14) == 1)) 
      {
        Motor1Starting_PWM2(DIR_CW,Manual_SpeedR);
      }
      MotorVal=1; 
    } 
    OSTimeDlyHMSM( 0,    
                  0,
                  0,
                  5,
                  OS_OPT_TIME_HMSM_STRICT,
                  &err);   
  } 
}

static void MotorRBack(void)//手动控制右电机回原地
{
    OS_ERR      err;
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    if(MotorRBackFlag)
    {
      AppTask_Sensor_Status.RMotorOriStopDet=ON; 
      Manual_SpeedR=*(Ptr_RegData+SRegisAdd3);
      if(Manual_SpeedR<1)Manual_SpeedR=1;
      Motor1Starting_PWM2(DIR_CCW,Manual_SpeedR);
    }
    while((1==MotorRBackFlag)&&(DoorSafety()==SAFE)&&(0==AppTask_Sensor_Status.Emergency))
    {
      if(1==AppTask_Sensor_Status.R_Motor_P)
      {
        MotorStop(TIM3);
        CH1_Mtion_ABS_POS=0;
        AppTask_Sensor_Status.RMotorOriStopDet=OFF;
        MotorRBackFlag=0; 
        AppTask_Sensor_Status.RMotorUp=0;
      }
      OSTimeDlyHMSM( 0,    
                    0,
                    0,
                    5,
                    OS_OPT_TIME_HMSM_STRICT,
                    &err); 
    }
}        

static void MotorBack(void)//双电机回原点
{
    OS_ERR      err;
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    if(MotorRBackFlag)
    {
      AppTask_Sensor_Status.RMotorOriStopDet=ON; 
      Manual_SpeedR=*(Ptr_RegData+SRegisAdd61);
      if(Manual_SpeedR<1)Manual_SpeedR=1;
      Motor1Starting_PWM2(DIR_CCW,Manual_SpeedR);
    }
    if(MotorLBackFlag)
    {
      AppTask_Sensor_Status.LMotorOriStopDet=ON; 
      Manual_SpeedL=*(Ptr_RegData+SRegisAdd59);
      if(Manual_SpeedL<1)Manual_SpeedL=1;
      Motor3Starting_PWM4(DIR_CCW,Manual_SpeedL);
    }
    while((1==MotorRBackFlag||1==MotorLBackFlag)&&(DoorSafety()==SAFE)&&(0==AppTask_Sensor_Status.Emergency))
    {
      if(1==AppTask_Sensor_Status.R_Motor_P)
      {
        MotorStop(TIM3);
        AppTask_Sensor_Status.RMotorOriStopDet=OFF;
        MotorRBackFlag=0; 
        AppTask_Sensor_Status.RMotorUp=0;//失能记录距离标志位
      }
      if(1==AppTask_Sensor_Status.L_Motor_P)
      {
        MotorStop(TIM2);
        CH3_Mtion_ABS_POS=0;
        AppTask_Sensor_Status.LMotorOriStopDet=OFF;
        MotorLBackFlag=0; 
        AppTask_Sensor_Status.LMotorUp=0;//失能记录距离标志位
      }
      OSTimeDlyHMSM( 0,    
                    0,
                    0,
                    5,
                    OS_OPT_TIME_HMSM_STRICT,
                    &err); 
    } 
}

void AgingMode(void)//老化测试
{
  
}

void Reset_System(void)//复位
{
    CPU_INT08U   *Ptr_CoilData;
    Ptr_CoilData=CoilData;
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    AppTask_Sensor_Status.Fixture_Mode=AUTO;
    
    PVacuumOff();
    NVacuumOff(); 
    LStartLedOff();
    RStartLedOff(); 
    BeepOff();
    *(Ptr_CoilData+NCoilAdd426)=0; //报警字幕清除
    *(Ptr_CoilData+NCoilAdd427)=0;//报警字幕清除
    *(Ptr_CoilData+NCoilAdd428)=0; //报警字幕清除
    *(Ptr_CoilData+NCoilAdd429)=0; //报警字幕清除
    *(Ptr_CoilData+NCoilAdd430)=0; //报警字幕清除
    AppTask_Sensor_Status.RunTimeEn=0;
    *(Ptr_RegData+SRegisAdd67)=0; 
}


static void EmergencyButtonWasPressed(void)//拍下急停处理事件
{ 
  AppTask_Sensor_Status.PressureLEn=0;//拉力过压停机标志清零
  AppTask_Sensor_Status.PressureREn=0;//压力过压停机标志清零
  CH1_Mtion_ABS_POS_Relative=0;   //电机脉冲步数清零
  CH3_Mtion_ABS_POS_Relative=0;   //电机脉冲步数清零
  MotorLBackFlag=1;//L 回原点使能
  AppTask_Sensor_Status.LMotorUp=1;//L 使能记录距离标志位 
  MotorRBackFlag=1;//R 回原点使能
  AppTask_Sensor_Status.RMotorUp=1;//R 使能记录距离标志位 
  AppTask_Sensor_Status.RunTimeEn=0;//运行时间停止计时 
  RunTime=0;//运行时间清零
//  AppTask_Sensor_Status.HeatRunTimeEn=0;//加热运行时间停止计时
  HeatFinished=0;//加热时间到标志清零  
//  HeatRunTime=0;//加热运行时间清零
  FinishedWaitTime = 0;//加热完成等待时间清零
  AppTask_Sensor_Status.FinishedWaitTEn=0;//拆料完成运行时间停止计时
  WaitFinished=0;//拆料完成运行时间清零
  AppTask_Sensor_Status.DoorEve=0;//光栅事件清空
  BeepOn();
  Step=0; //状态机清零
  MotorStop(TIM2);//停机
  MotorStop(TIM3);//停机
  if(AppTask_Sensor_Status.DoorEn==ON)
  {
    AppTask_Sensor_Status.DoorEn=OFF;
    DoorEnRecord=1;
  }
}

static void EmergencyButtonWasReleased(void)//松开急停处理事件
{     
  ResLedOn() ;
  Reset_System();
  MotorBack();//急停双电机回原点
  if((1==AppTask_Sensor_Status.L_Motor_P)&&(1==AppTask_Sensor_Status.R_Motor_P))//双电机回到原点
  {
    AppTask_Sensor_Status.EmeEvent=0;
    AppTask_Sensor_Status.Reset_Event=0;
    ResLedOff() ;
    Step=0; //状态机清零
  }
  if(DoorEnRecord==1)
  {
    DoorEnRecord=0;
    AppTask_Sensor_Status.DoorEn=ON;
  }
}

//static void  DoorIsDangerous(void)//检测到门不安全的操作
//{     
//  MotorStop(TIM2);//停机
//  MotorStop(TIM3);//停机
//}

void ControlSequence(void)//动作执行顺序
{
//  OS_ERR      err;
//  CPU_INT16S   *Ptr_RegData;
//  Ptr_RegData=RegData;
  
  
 } 