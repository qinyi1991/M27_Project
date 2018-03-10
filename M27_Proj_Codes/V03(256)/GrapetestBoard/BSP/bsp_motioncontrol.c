#include <bsp_motioncontrol.h>
#include <bsp_Controltargetconf.h>

#define SPEED_PROFILE_ANALYSIS
volatile unsigned int KeepCounter,Cnt;
volatile unsigned char TIMER_OVRE_FLAG,TimeIsOver,HeatFinished,WaitFinished;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

Typedef_MtionDirection  MtionDirection_PWM1,MtionDirection_PWM2,MtionDirection_PWM3,MtionDirection_PWM4,MtionDirection_PWM5,MtionDirection_PWM6;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
void MOTOR1_TIM3_ISR(void);
void MOTOR2_TIM4_ISR(void);
void MOTOR3_TIM2_ISR(void);
void MOTOR4_TIM5_ISR(void);
void TIM1_ISR(void);
/** 
 * @brief  Motion control modul RCC initialization
 * @param  None
 * @retval None
 */
void MotionControl_RCC_Configuration() {
	/* Enable associated GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE,ENABLE);
	/* Enable associated TIM clock */ 
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
   
}

/**
 * @brief  Motion control modul GPIO initialization
 * @param  None
 * @retval None
 */
void MotionControl_GPIO_Configuration() {
      GPIO_InitTypeDef GPIO_InitStructure;
   
       GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 
      //PWM6(TIM4_CH3)
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3|GPIO_Pin_4;//DIR6 DIR4
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOB, &GPIO_InitStructure);     
      
      //PWM4(TIM2_CH2),PWM5(TIM5_CH1),PWM2(TIM3_CH1),PWM3(TIM1_CH4),
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0|GPIO_Pin_6|GPIO_Pin_11;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//DIR3 DIR2 DIR1
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOE, &GPIO_InitStructure); 
      
       //PWM1(TIM8_CH3)
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      
      GPIO_StructInit(&GPIO_InitStructure);//DIR5
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_Init(GPIOA, &GPIO_InitStructure); 
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
}

/**
 * @brief  Motion control modul NVIC initialization
 * @param  None
 * @retval None
 */
void   MotionControl_NVIC_Configuration_PWM2() {

	NVIC_InitTypeDef NVIC_InitStructure;
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
}

void   MotionControl_NVIC_Configuration_PWM6() {

	NVIC_InitTypeDef NVIC_InitStructure;
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
}

void   MotionControl_NVIC_Configuration_PWM4() {

	NVIC_InitTypeDef NVIC_InitStructure;
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
}

void   MotionControl_NVIC_Configuration_PWM5() {

	NVIC_InitTypeDef NVIC_InitStructure;
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
}

/**
 * @brief  Motion control modul TIM initialization
 * @param  None
 * @retval None
 */

//TIM3 CH1 PWM2
void  Motion_TIM_Configuration_PWM2(CPU_INT16U PrescalerValue,CPU_INT16U PeriodValue){
  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
        //PWM1(TIM3_CH3)
	/* Slave mode disabled - prescaler clocked by the internal clock */
	TIM_InternalClockConfig(TIM3);	
        
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = PeriodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Timer config */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* @note This parameter is valid only for TIM1 and TIM8. */

	TIM_OCInitStructure.TIM_Pulse = PeriodValue >> 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* Initialize all the registers by setting the UG bit in the TIMx_EGR register */
	TIM_PrescalerConfig(TIM3, PrescalerValue,
			TIM_PSCReloadMode_Immediate);

	TIM_CtrlPWMOutputs(TIM3, ENABLE); 
   
        BSP_IntVectSet(BSP_INT_ID_TIM3, MOTOR1_TIM3_ISR);
        BSP_IntEn(BSP_INT_ID_TIM3);        
}
//TIM4 CH3 PWM6
void  Motion_TIM_Configuration_PWM6(CPU_INT16U PrescalerValue,CPU_INT16U PeriodValue){
	
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
        
        
	/* Slave mode disabled - prescaler clocked by the internal clock */
	TIM_InternalClockConfig(TIM4);	
        
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = PeriodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* Timer config */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* @note This parameter is valid only for TIM1 and TIM8. */

	TIM_OCInitStructure.TIM_Pulse = PeriodValue >> 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);

	/* Initialize all the registers by setting the UG bit in the TIMx_EGR register */
	TIM_PrescalerConfig(TIM4, PrescalerValue,
			TIM_PSCReloadMode_Immediate);

	TIM_CtrlPWMOutputs(TIM4, ENABLE); 
   
        BSP_IntVectSet(BSP_INT_ID_TIM4, MOTOR2_TIM4_ISR);
        BSP_IntEn(BSP_INT_ID_TIM4);        
}
//TIM2 CH2 PWM4
void  Motion_TIM_Configuration_PWM4(CPU_INT16U PrescalerValue,CPU_INT16U PeriodValue){
	
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
        
        //PWM3(TIM2_CH2)
	/* Slave mode disabled - prescaler clocked by the internal clock */
	TIM_InternalClockConfig(TIM2);	
        //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
        
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = PeriodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Timer config */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* @note This parameter is valid only for TIM1 and TIM8. */

	TIM_OCInitStructure.TIM_Pulse = PeriodValue >> 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);

	/* Initialize all the registers by setting the UG bit in the TIMx_EGR register */
	TIM_PrescalerConfig(TIM2, PrescalerValue,
			TIM_PSCReloadMode_Immediate);

	TIM_CtrlPWMOutputs(TIM2, ENABLE); 
   
        BSP_IntVectSet(BSP_INT_ID_TIM2, MOTOR3_TIM2_ISR);
        BSP_IntEn(BSP_INT_ID_TIM2);        
}
//TIM5 CH1 PWM5
void  Motion_TIM_Configuration_PWM5(CPU_INT16U PrescalerValue,CPU_INT16U PeriodValue){
	
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
        
        //PWM4(TIM5_CH1)
	/* Slave mode disabled - prescaler clocked by the internal clock */
	TIM_InternalClockConfig(TIM5);	
        
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = PeriodValue;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	/* Timer config */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
        
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* @note This parameter is valid only for TIM1 and TIM8. */

	TIM_OCInitStructure.TIM_Pulse = PeriodValue >> 1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5, ENABLE);

	/* Initialize all the registers by setting the UG bit in the TIMx_EGR register */
	TIM_PrescalerConfig(TIM5, PrescalerValue,
			TIM_PSCReloadMode_Immediate);

	TIM_CtrlPWMOutputs(TIM5, ENABLE); 
   
        BSP_IntVectSet(BSP_INT_ID_TIM5, MOTOR4_TIM5_ISR);
        BSP_IntEn(BSP_INT_ID_TIM5);        
}

void Timer1Init(void)
{
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
      
        TIM_InternalClockConfig(TIM1);	
        //GPIO_PinRemapConfig(GPIO_FullRemap_TIM2,ENABLE);
        
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = 7119;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
        
        TIM_ClearFlag(TIM1,TIM_FLAG_Update);
        TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
        
        TIM_ARRPreloadConfig(TIM1, ENABLE);
        TIM_PrescalerConfig(TIM1, 7119,
			TIM_PSCReloadMode_Immediate);
//        TIM_Cmd(TIM2,ENABLE);
        
        BSP_IntVectSet(BSP_INT_ID_TIM1_UP, TIM1_ISR);
        BSP_IntEn(BSP_INT_ID_TIM1_UP); 
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
         
}

void MotionControl_Init() {
	/* STM32 init */
	MotionControl_RCC_Configuration();
	MotionControl_GPIO_Configuration();
        
	//Motion_TIM_Configuration_PWM1(0,2500);
	//MotionControl_NVIC_Configuration_PWM1();	
        
         Motion_TIM_Configuration_PWM2(0,2500);
         MotionControl_NVIC_Configuration_PWM2();
	
        // Motion_TIM_Configuration_PWM3(0,2500);
        // MotionControl_NVIC_Configuration_PWM3();
        
        Timer1Init();
        
        Motion_TIM_Configuration_PWM4(0,2500);
        MotionControl_NVIC_Configuration_PWM4();
}

void TIM1_ISR(void)
{       
    
        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) {          
          if(KeepCounter>0)
          {
            KeepCounter--;
          }
          else
          {
            TIMER_OVRE_FLAG=1;
          }
          
          if(Cnt>0)
          {
            Cnt--;
          }
          else
          {
            TimeIsOver=1;
          } 
          
        } 
        TIM_ClearFlag(TIM1,TIM_FLAG_Update);  
  
}

/**
  * @brief  This function handles Timer Update interrupt request.
  * @param  None
  * @retval None
  */    
    
void MOTOR1_TIM3_ISR(void){ 
    CPU_INT16S   *Ptr_RegData;
    Ptr_RegData=RegData;
    
    CPU_INT08U   *Ptr_CoilData;
    Ptr_CoilData=CoilData;
    
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) { 
      
      if(AppTask_Sensor_Status.RMotorUp)
      { 
        CH1_Mtion_ABS_POS--; 
      }
      if(AppTask_Sensor_Status.RMotorDown)
      {  
        CH1_Mtion_ABS_POS++;
        
        if(R_Woking_Distance>0)
        {
          R_Woking_Distance--;
        }
        
        if(AppTask_Sensor_Status.PressureREn==1)
        {
          BeepOn();
        }
      }
      
      //手动控制时用
      if(CH1_Mtion_ABS_POS_Relative>0)
      {
        CH1_Mtion_ABS_POS_Relative--;
        if(CH1_Mtion_ABS_POS_Relative == 0)
        {
          MotorStop(TIM3);
        }
      } 
      
      TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    } 
}


void MOTOR2_TIM4_ISR(void) {//right
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {          
//          if(CH2_Mtion_ABS_POS_Relative>0){
//              CH2_Mtion_ABS_POS_Relative--;
//              if(CH2_Mtion_ABS_POS_Relative == 0){
//                   MotorStop(TIM4);
//              }
//          }       
          TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}

void MOTOR3_TIM2_ISR(void)
{
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  CPU_INT08U   *Ptr_CoilData;
  Ptr_CoilData=CoilData;
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) { 
    if(AppTask_Sensor_Status.LMotorUp)
    { 
      CH3_Mtion_ABS_POS--;
      
      if(AppTask_Sensor_Status.LMotorWorkingUp)
      {
        L_Woking_Distance++; 
      }
      
    }
    if(AppTask_Sensor_Status.LMotorDown)
    {  
      CH3_Mtion_ABS_POS++;
      if(L_Woking_Distance>0)
      {
        L_Woking_Distance--;
      }
      if(AppTask_Sensor_Status.PressureREn==1)
      {
        BeepOn();
      }
    } 
    
    //手动时使用
    if(CH3_Mtion_ABS_POS_Relative>0)
    {
      CH3_Mtion_ABS_POS_Relative--;
      if(CH3_Mtion_ABS_POS_Relative == 0)
      {
        MotorStop(TIM2);
      }
    } 
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  } 
}

void MOTOR4_TIM5_ISR(void)
{
 	if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) {
////           if(AppTask_Sensor_Status.OriginStopDet==1)
//           {
////            if(AppTask_Sensor_Status.Origin)
//             {MotorStop(TIM5);}
//           }
//          if(CH4_Mtion_ABS_POS_Relative>0){
//              CH4_Mtion_ABS_POS_Relative--;
//             
//            
//              if(CH4_Mtion_ABS_POS_Relative == 0){
//                   MotorStop(TIM5);
//              }
//          }
//         if(CH4_Mtion_ABS_POS_Relative == 0){
//                  MotorStop_PWM4(TIM5);
//             }
          TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	} 
  
}
//方向设置******
void Direction_Motor_PWM1(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR1=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR1=0;
    }
}

void Direction_Motor_PWM2(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR2=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR2=0;
    }
}

void Direction_Motor_PWM3(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR3=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR3=0;
    }
}

void Direction_Motor_PWM4(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR4=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR4=0;
    }
}
void Direction_Motor_PWM5(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR5=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR5=0;
    }
}
void Direction_Motor_PWM6(Typedef_MtionDirection Motion_DIR)
{
    if(Motion_DIR == DIR_CW){
          BSP_DIR6=1;
    }else if(Motion_DIR == DIR_CCW){
          BSP_DIR6=0;
    }
}
//1#电机控制函数
int Motor1Starting_PWM2(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed)
{															  
	CPU_INT16U  i;
	double   dt=0.0,dd=0.0,ck;

	dd=FreqCLK;	
	dt=((double)speed/100)*SUBDIVISION/16;   					
	i=0;				  
	ck=0.0;
	do 
	{
		i++;		
		ck=dd/dt;
		ck=ck/(double)i;	  
	}while(ck>60000.0);
	if(i>=1)
	{i=i-1;}
        
        //方向信号
        MtionDirection_PWM2=Motion_DIR;
        Direction_Motor_PWM2(MtionDirection_PWM2);  
        //PWM1信号
	Motion_TIM_Configuration_PWM2(i,(CPU_INT16U)ck);
        
        /* Enable interrupt generation */
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_UpdateRequestConfig(TIM3, TIM_UpdateSource_Global);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
        
	/* Start timer */
        TIM_Cmd(TIM3,ENABLE);
        
	return 0;		
}

int Motor2Starting_PWM6(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed)
{															  
	CPU_INT16U  i;
	double   dt=0.0,dd=0.0,ck;

	dd=FreqCLK;	
	dt=((double)speed/100)*SUBDIVISION/16;   					
	i=0;				  
	ck=0.0;
	do 
	{
		i++;		
		ck=dd/dt;  //72M / 1  
		ck=ck/(double)i; 	  
	}while(ck>60000.0);
	if(i>=1)
	{i=i-1;}
        
        //方向信号
        MtionDirection_PWM6=Motion_DIR;
        Direction_Motor_PWM6(MtionDirection_PWM6);
        //脉冲信号
	Motion_TIM_Configuration_PWM6(i,(CPU_INT16U)ck);
        
        /* Enable interrupt generation */
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_UpdateRequestConfig(TIM4, TIM_UpdateSource_Global);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
        
	/* Start timer */
        TIM_Cmd(TIM4,ENABLE);
        
	return 0;		
}


int Motor3Starting_PWM4(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed)
{															  
	CPU_INT16U  i;
	double   dt=0.0,dd=0.0,ck;

	dd=FreqCLK;	
	dt=((double)speed/100)*SUBDIVISION/16;   					
	i=0;				  
	ck=0.0;
	do 
	{
		i++;		
		ck=dd/dt;
		ck=ck/(double)i;	  
	}while(ck>60000.0);
	if(i>=1)
	{i=i-1;}
        
        //方向信号
        MtionDirection_PWM4=Motion_DIR;
        Direction_Motor_PWM4(MtionDirection_PWM4);
        //脉冲信号
	Motion_TIM_Configuration_PWM4(i,(CPU_INT16U)ck);
        
        /* Enable interrupt generation */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Global);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        
	/* Start timer */
        TIM_Cmd(TIM2,ENABLE);
        
	return 0;		
}


int Motor4Starting_PWM5(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed)
{															  
	CPU_INT16U  i;
	double   dt=0.0,dd=0.0,ck;

	dd=FreqCLK;	
	dt=(double)((double)speed/10)*SUBDIVISION;
	i=0;				  
	ck=0.0;
	do 
	{
		i++;		
		ck=dd/dt;
		ck=ck/(double)i;	  
	}while(ck>60000.0);
	if(i>=1)
	{i=i-1;}
        
        //方向信号
        MtionDirection_PWM5=Motion_DIR;
        Direction_Motor_PWM5(MtionDirection_PWM5);
        //脉冲信号
	Motion_TIM_Configuration_PWM5(i,(CPU_INT16U)ck);
        
        /* Enable interrupt generation */
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_UpdateRequestConfig(TIM5, TIM_UpdateSource_Global);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
        
	/* Start timer */
        TIM_Cmd(TIM5,ENABLE);
        
	return 0;		
}

//停机控制
void MotorStop(TIM_TypeDef* TIMx)
{
	/* Deactivate timer */
	TIM_Cmd(TIMx, DISABLE);
	/* Disable interrupt generation */
	TIM_ITConfig(TIMx, TIM_IT_Update, DISABLE);
}
