/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2007; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include <bsp.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/

#define  BSP_LED_START_BIT  (13 - 1)                            /* LEDs[3:1] are sequentially connected to PTD[15:13].  */


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq_MHz;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_GPIOFUNCTION_Init   (void);

/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  DWT_CR      *(CPU_REG32 *)0xE0001000
#define  DWT_CYCCNT  *(CPU_REG32 *)0xE0001004
#define  DEM_CR      *(CPU_REG32 *)0xE000EDFC
#define  DBGMCU_CR   *(CPU_REG32 *)0xE0042004


/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  DBGMCU_CR_TRACE_IOEN_MASK       0x10
#define  DBGMCU_CR_TRACE_MODE_ASYNC      0x00
#define  DBGMCU_CR_TRACE_MODE_SYNC_01    0x40
#define  DBGMCU_CR_TRACE_MODE_SYNC_02    0x80
#define  DBGMCU_CR_TRACE_MODE_SYNC_04    0xC0
#define  DBGMCU_CR_TRACE_MODE_MASK       0xC0

#define  DEM_CR_TRCENA                   (1 << 24)

#define  DWT_CR_CYCCNTENA                (1 <<  0)


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

#if ((CPU_CFG_TS_TMR_EN          != DEF_ENABLED) && \
     (APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          >  0u))
#error  "CPU_CFG_TS_EN                  illegally #define'd in 'cpu.h'"
#error  "                              [MUST be  DEF_ENABLED] when    "
#error  "                               using uC/Probe COM modules    "
#endif


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (b) The uC-Eval board MAY utilize the following pins depending on the application :
*                       (1) PE[5], MII_INT
*                       (1) PE[6], SDCard_Detection
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init (void)
{ 
    BSP_IntInit();

    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);                                  /* HSE = 8MHz ext. crystal.                            */
    RCC_WaitForHSEStartUp();
   
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                            /* HCLK    = AHBCLK  = PLL1 / AHBPRES(1) = 72MHz.       */
    RCC_PCLK2Config(RCC_HCLK_Div1);                             /* APB2CLK = AHBCLK  / APB2DIV(1)  = 72MHz.             */
    RCC_PCLK1Config(RCC_HCLK_Div2);                             /* APB1CLK = AHBCLK  / APB1DIV(2)  = 36MHz (max).       */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                           /* ADCCLK  = AHBCLK  / APB2DIV / 6 = 12MHz.             */

    FLASH_SetLatency(FLASH_Latency_2);                          /* 2 Flash wait states when HCLK > 48MHz.               */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
   
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
     RCC_PLLCmd(ENABLE);

     while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {      /* Wait for PLL to lock.                               */
        ;
     }  
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                 /* HCLK = SYSCLK = PLL1 = 72MHz.                        */    
     while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {      //等待PLL时钟就绪
        ;
     }
  
    BSP_CPU_ClkFreq_MHz = BSP_CPU_ClkFreq() / (CPU_INT32U)1000000;
    BSP_CPU_ClkFreq_MHz = BSP_CPU_ClkFreq_MHz;                  /* Surpress compiler warning BSP_CPU_ClkFreq_MHz    ... */
                                                                /* ... set and not used.                                */
    BSP_GPIOFUNCTION_Init();                                    /* Initialize the I/Os for the LED      controls.       */
    ///配置文件
    IIC_Configuration();
    MotionControl_Init();
    //BSP_Usart1_Init(115200);
    BSP_Usart4_Init(9600);
   // BSP_Usart2_WrByte(0xee);
 //   BSP_Usart3_Init(19200);
 //   BSP_Usart2_Init(19200);
  // BSP_Ser_WrByte(255);
  //  SPI1_Init();
   
  
#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_IOEN_MASK;                    /* Enable tracing (see Note #2).                        */
    DBGMCU_CR &= ~DBGMCU_CR_TRACE_MODE_MASK;                    /* Clr trace mode sel bits.                             */
    DBGMCU_CR |=  DBGMCU_CR_TRACE_MODE_SYNC_04;                 /* Cfg trace mode to synch 4-bit.                       */
#endif
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                              LED FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             BSP_LED_Init()
*
* Description : Initialize the I/O for the LEDs
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  BSP_GPIOFUNCTION_Init (void)
{
    GPIO_InitTypeDef  gpio_init;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOG|RCC_APB2Periph_AFIO, ENABLE);  

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);
    
    //输出     
    gpio_init.GPIO_Pin   = DEF_BIT_04|DEF_BIT_05|DEF_BIT_07;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio_init); 
    
    gpio_init.GPIO_Pin   = DEF_BIT_00|DEF_BIT_01;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio_init); 
    
    gpio_init.GPIO_Pin   = DEF_BIT_00 | DEF_BIT_01 | DEF_BIT_02 | DEF_BIT_03|DEF_BIT_04|DEF_BIT_05| DEF_BIT_13 | DEF_BIT_14| DEF_BIT_15;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &gpio_init); 
    
    gpio_init.GPIO_Pin   = DEF_BIT_09;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &gpio_init);
    
    gpio_init.GPIO_Pin   = DEF_BIT_05|DEF_BIT_06|DEF_BIT_07|DEF_BIT_08|DEF_BIT_09|DEF_BIT_10|DEF_BIT_11|DEF_BIT_12|DEF_BIT_13;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &gpio_init);
    
    //输出置位
    BSP_POUT1_01=1; BSP_POUT1_02=1;BSP_POUT1_03=1;BSP_POUT1_04=1;BSP_POUT1_05=1;BSP_POUT1_05=1;BSP_POUT1_06=1; BSP_POUT1_07=1;BSP_POUT1_08=1;    
    BSP_POUT2_01=1; BSP_POUT2_02=1;BSP_POUT2_03=1;BSP_POUT2_04=1;BSP_POUT2_05=1;BSP_POUT2_05=1;BSP_POUT2_06=1; BSP_POUT2_07=1;BSP_POUT2_08=1; 
    BSP_POUT3_01=1; BSP_POUT3_02=1;BSP_POUT3_03=1;BSP_POUT3_04=1;BSP_POUT3_05=1;BSP_POUT3_05=1;BSP_POUT3_06=1; BSP_POUT3_07=1;BSP_POUT3_08=1; 
    
    //输入
    gpio_init.GPIO_Pin   = DEF_BIT_08|DEF_BIT_12;
    gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio_init); 
    
    gpio_init.GPIO_Pin   = DEF_BIT_05 | DEF_BIT_09;
    gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &gpio_init);    
    
    gpio_init.GPIO_Pin   = DEF_BIT_06|DEF_BIT_07|DEF_BIT_09|DEF_BIT_12;
    gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &gpio_init);  
    
    gpio_init.GPIO_Pin   = DEF_BIT_00|DEF_BIT_01|DEF_BIT_02|DEF_BIT_03|DEF_BIT_04 | DEF_BIT_05 | DEF_BIT_06 | DEF_BIT_07 | DEF_BIT_10 |DEF_BIT_11 | DEF_BIT_12 | DEF_BIT_13 | DEF_BIT_14 | DEF_BIT_15;
    gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &gpio_init);  
    
    gpio_init.GPIO_Pin   = DEF_BIT_00 | DEF_BIT_01;
    gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &gpio_init); 
    
    RMotorOn(); 
    LMotorOn(); 
}

void WritFP32TOBUFF(CPU_FP32 *Temp_dat,CPU_INT16U Add)
{
      CPU_INT16S   *LPtr_RegData=RegData;
      CPU_INT08U   i,*ptr,*ptr_mapreg;
      
      ptr=(CPU_INT08U*)(Temp_dat);
      ptr_mapreg=(CPU_INT08U*)(LPtr_RegData+Add);
        
      for(i=0;i<4;i++)
      {
           *(ptr_mapreg+i)=*(ptr+i);
      } 
}

void ReadFP32TOBUFF(CPU_FP32 *Temp_dat,CPU_INT16U Add)
{
 
}

//atmel256k初始化
void Checkparama_AND_Inital(void)
{
      CPU_INT08U tempData;
      CPU_INT16S   *Ptr_RegData;
      Ptr_RegData=RegData;
      CPU_INT08U   *Ptr_CoilData;
      Ptr_CoilData=CoilData;    
      IIC_Channel=1;

        IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd1),&Manual_SpeedL);
        *(Ptr_RegData+SRegisAdd1)=Manual_SpeedL; 
        
        IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd3),&Manual_SpeedR);
        *(Ptr_RegData+SRegisAdd3)=Manual_SpeedR;
        
         IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd13),&L_Location_First);
         *(Ptr_RegData+SRegisAdd14)=L_Location_First*25&0x0000ffff;
         *(Ptr_RegData+SRegisAdd13)=L_Location_First*25>>16;
         
         IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd17),&L_Location_Second);
         *(Ptr_RegData+SRegisAdd18)=L_Location_Second*25&0x0000ffff;
         *(Ptr_RegData+SRegisAdd17)=L_Location_Second*25>>16;
         
         IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd21),&R_Location_First);
         *(Ptr_RegData+SRegisAdd22)=R_Location_First*25&0x0000ffff;
         *(Ptr_RegData+SRegisAdd21)=R_Location_First*25>>16;
         
         IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd25),&R_Location_Second);
         *(Ptr_RegData+SRegisAdd26)=R_Location_Second*25&0x0000ffff;
         *(Ptr_RegData+SRegisAdd25)=R_Location_Second*25>>16;
        
          IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd29),&R_Press_Pressure);
          *(Ptr_RegData+SRegisAdd29)=( R_Press[3]<<8)+ R_Press[2];
          *(Ptr_RegData+SRegisAdd30)=( R_Press[1]<<8)+ R_Press[0];
         
           IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd33),&R_Pull_Pressure);
          *(Ptr_RegData+SRegisAdd33)=( R_Pull[3]<<8)+ R_Pull[2];
          *(Ptr_RegData+SRegisAdd34)=( R_Pull[1]<<8)+ R_Pull[0];
         
           
          IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd37),&R_Pull_Max_Pressure);
          *(Ptr_RegData+SRegisAdd37)=( R_Pull_Max[3]<<8)+ R_Pull_Max[2];
          *(Ptr_RegData+SRegisAdd38)=( R_Pull_Max[1]<<8)+ R_Pull_Max[0];
       
           IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd41),&L_Press_Pressure);
          *(Ptr_RegData+SRegisAdd41)=( L_Press[3]<<8)+ L_Press[2];
          *(Ptr_RegData+SRegisAdd42)=( L_Press[1]<<8)+ L_Press[0];
         
           IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd45),&L_Pull_Pressure);
          *(Ptr_RegData+SRegisAdd45)=( L_Pull[3]<<8)+ L_Pull[2];
          *(Ptr_RegData+SRegisAdd46)=( L_Pull[1]<<8)+ L_Pull[0];
         
           
          IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd49),&L_Pull_Max_Pressure);
          *(Ptr_RegData+SRegisAdd49)=( L_Pull_Max[3]<<8)+ L_Pull_Max[2];
          *(Ptr_RegData+SRegisAdd50)=( L_Pull_Max[1]<<8)+ L_Pull_Max[0];
         
//         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd53),& HeatTime);
//         *(Ptr_RegData+SRegisAdd53)=HeatTime; 
//         HeatRunTime=HeatTime;
         *(Ptr_RegData+SRegisAdd83)=HeatRunTime;
         
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd55),&OverPreWaitTimeL);
         *(Ptr_RegData+SRegisAdd55)=OverPreWaitTimeL;  
        
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd57),& FinishedWaitTime);
         *(Ptr_RegData+SRegisAdd57)= FinishedWaitTime;  
         
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd59),& Auto_SpeedL);
         if(Auto_SpeedL>3000)
         {
            Auto_SpeedL=3000;
         }
         *(Ptr_RegData+SRegisAdd59)=  Auto_SpeedL;  
         
         
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd61),& Auto_SpeedR);
         if(Auto_SpeedR>3000)
         {
            Auto_SpeedR=3000;
         }
         *(Ptr_RegData+SRegisAdd61)=  Auto_SpeedR; 
         
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd63),&  Woking_SpeedL);
         if(Woking_SpeedL>1500)
         {
            Woking_SpeedL=1500;
         }
         *(Ptr_RegData+SRegisAdd63)=  Woking_SpeedL; 
         
         IIC_Read_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd65),&  Woking_SpeedR);
         if(Woking_SpeedR>1500)
         {
            Woking_SpeedR=1500;
         }
         *(Ptr_RegData+SRegisAdd65)=  Woking_SpeedR; 
         
        IIC_Read_4Byte(OperateAdd(ModbusRegAdd,SRegisAdd69),&Pass_Product_Counters);
        *(Ptr_RegData+SRegisAdd70)=Pass_Product_Counters&0x0000ffff;
        *(Ptr_RegData+SRegisAdd69)=Pass_Product_Counters>>16;
         
         IIC_Read_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd20),&tempData);
         if(tempData==0xff)
         {
         *(Ptr_CoilData+SCoilAdd20)=1;
          AppTask_Sensor_Status.DoorEn=ON;//打开
         }
         else
         {
         *(Ptr_CoilData+SCoilAdd20)=0; 
          AppTask_Sensor_Status.DoorEn=OFF;//关闭
         }
        
         IIC_Read_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd21),&tempData);
         if(tempData==0xff)
         {
         *(Ptr_CoilData+SCoilAdd21)=1;
         AppTask_Sensor_Status.Heat_Vacuum_N=ON;
         }
         else
         {
         *(Ptr_CoilData+SCoilAdd21)=0;
         AppTask_Sensor_Status.Heat_Vacuum_N=OFF;
         }
         IIC_Read_1Byte(OperateAdd(ModbusCoilAdd,SCoilAdd23),&tempData);
         if(tempData==0xff)
         {
         *(Ptr_CoilData+SCoilAdd23)=1;
         AppTask_Sensor_Status.DoorMode=ON;
         }
         else
         {
         *(Ptr_CoilData+SCoilAdd23)=0;
         AppTask_Sensor_Status.DoorMode=OFF;
         }
         
//        PrinfStringToScreen("V04 ",SRegisAdd60);
//        PrinfStringToScreen("V02 ",SRegisAdd68);
//        PrinfStringToScreen("ShenZhen Strong Technology Co.,Ltd. ",SRegisAdd88);
//        PrinfStringToScreen("20160912",SRegisAdd138);
}


/*
*********************************************************************************************************
*********************************************************************************************************
*                                           OS PROBE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                           OSProbe_TmrInit()
*
* Description : Select & initialize a timer for use with the uC/Probe Plug-In for uC/OS-II.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : OSProbe_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          == 1))
void  OSProbe_TmrInit (void)
{
}
#endif


/*
*********************************************************************************************************
*                                            OSProbe_TmrRd()
*
* Description : Read the current counts of a free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32-bit timer counts.
*
* Caller(s)   : OSProbe_TimeGetCycles().
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if ((APP_CFG_PROBE_OS_PLUGIN_EN == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN          == 1))
CPU_INT32U  OSProbe_TmrRd (void)
{
    return ((CPU_INT32U)DWT_CYCCNT);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  cpu_clk_freq_hz;
      
      
    DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                /* Enable Cortex-M3's DWT CYCCNT reg.                   */
    DWT_CYCCNT      = (CPU_INT32U)0u;
    DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;

    cpu_clk_freq_hz = BSP_CPU_ClkFreq();    
    CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
     return ((CPU_TS_TMR)DWT_CYCCNT);
}
#endif
