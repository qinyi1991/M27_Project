#include "bspadc.h"

#define ADC1_DR_Address    ((CPU_INT32U)0x4001244C)

#define AD_BUFFERSIZE 3
CPU_INT32U  AD_value[AD_BUFFERSIZE];

void DMA1_Channel_ISR(void);


//使用定时器TIM1 触发ADC采样模式
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
        
//----------------------------------------------------------------------------------------------------------------------------------------------------
//定时器触发采样       
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* Slave mode disabled - prescaler clocked by the internal clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	      
	/* Time base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 1000;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* Timer config */
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_Pulse = 500;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;   /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set; /* @note This parameter is valid only for TIM1 and TIM8. */
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);    
        TIM_CtrlPWMOutputs(TIM1, ENABLE);
        TIM_Cmd(TIM1,ENABLE);
//----------------------------------------------------------------------------------------------------------------------------------------------------       
	
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
        
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
        
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv =ADC_ExternalTrigConv_T1_CC1;   // ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
        
	ADC_Init(ADC1,&ADC_InitStructure);  
   //     ADC_Init(ADC2,&ADC_InitStructure);
        
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);  
        //ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
        
	ADC_DMACmd(ADC1, ENABLE);   
        
	ADC_Cmd(ADC1,ENABLE);
     //   ADC_Cmd(ADC2,ENABLE);
        
        //ADC1--------
        ADC_ResetCalibration(ADC1);
        while(ADC_GetResetCalibrationStatus(ADC1));
        ADC_StartCalibration(ADC1);
        while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
        while(ADC_GetSoftwareStartConvStatus(ADC1));
        
        ///ADC2--------
//        ADC_ResetCalibration(ADC2);
//        while(ADC_GetResetCalibrationStatus(ADC2));
//        ADC_StartCalibration(ADC2);
//        while(ADC_GetCalibrationStatus(ADC2));
//	ADC_SoftwareStartConvCmd(ADC2,ENABLE);
//        while(ADC_GetSoftwareStartConvStatus(ADC2));
}

void   ADC_DMA_NVIC_Configuration() {

	NVIC_InitTypeDef NVIC_InitStructure;
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0C;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	
}

void DMA1_Channel1_Configuration(void)
{
	DMA_InitTypeDef	DMA_InitStructure;
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
        
	DMA_DeInit(DMA1_Channel1);	
        
	DMA_InitStructure.DMA_PeripheralBaseAddr = (CPU_INT32U)ADC1_DR_Address; 	
        DMA_InitStructure.DMA_MemoryBaseAddr = (CPU_INT32U)&AD_value[0];				
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				
	DMA_InitStructure.DMA_BufferSize = AD_BUFFERSIZE;						
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;		  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;						
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;					   
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure); 
   
        DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);///配置DMA channel1传输完成中断
        ADC_DMA_NVIC_Configuration();
        BSP_IntVectSet(BSP_INT_ID_DMA1_CH1, DMA1_Channel_ISR);
        BSP_IntEn(BSP_INT_ID_DMA1_CH1);    
        DMA_Cmd(DMA1_Channel1,ENABLE);
}

///滤波算法
void filter()
{
    CPU_INT32U sum1=0;
               //sum2=0;
    CPU_INT08U i=0;
    CPU_INT16U Max1=0,Min1=0;
               //Max2=0,Min2=0;
      
    //中值滤波
       Min1=AD_value[0]&0xffff;
 //     Min2= (AD_value[0]&0xffff0000)>>16;
      for(i=0;i<AD_BUFFERSIZE;i++){
          //adc1
          if((AD_value[i]&0xffff)>Max1){
            Max1=AD_value[i]&0xffff;
          }
          if((AD_value[i]&0xffff)<Min1){
            Min1=AD_value[i]&0xffff;
          }
          sum1+=AD_value[i]&0xffff;
          
//          //adc2
//          if(((AD_value[i]&0xffff0000)>>16)>Max2){
//            Max2=(AD_value[i]&0xffff0000)>>16;
//          }
//          if(((AD_value[i]&0xffff0000)>>16)<Min2){
//            Min2=(AD_value[i]&0xffff0000)>>16;
//          }  
//          sum2+=(AD_value[i]&0xffff0000)>>16;
      }
      
        NTC_ConverredValue=sum1-Max1-Min1;
//      ADC2_ConvertedValue=(sum2-Max2-Min2)/(AD_BUFFERSIZE-2);
}

//DMA1 channel1 中断服务函数
void DMA1_Channel_ISR(void){
    if(DMA_GetITStatus(DMA1_IT_TC1) == SET)
    { 
         filter();
         DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
}

CPU_INT16U TestADC1_ConversionResult(void)
{
        CPU_FP64 conresult; 
        conresult = NTC_ConverredValue* 2500;
        conresult = conresult / 4096;	

//	if(ADC1_ConvertedValue<=CalValueAD_Zero)
//	{
//		conresult=0;
//	}
//	else if((ADC1_ConvertedValue>CalValueAD_Zero)&&(ADC1_ConvertedValue<=CalValueAD_3KG))
//	{
//		conresult=SetValueHMI_3KG*(ADC1_ConvertedValue-CalValueAD_Zero)/(CalValueAD_3KG-CalValueAD_Zero);
//	}
//	else 
//	{
//		conresult=(SetValueHMI_5KG-SetValueHMI_3KG)*(ADC1_ConvertedValue-CalValueAD_3KG)/(CalValueAD_5KG-CalValueAD_3KG)+SetValueHMI_3KG;
//	}
        
        return  (CPU_INT16S)conresult;
} 

//CPU_INT16S TestADC2_ConversionResult(void)
//{
//        CPU_FP64 conresult;  
//        conresult = ADC2_ConvertedValue * 2500;
//        conresult = conresult / 4096;	
////         CPU_FP64 conresult; 
////	if(ADC2_ConvertedValue<=EXT1_CalValueAD_Zero)
////	{
////		conresult=0;
////	}
////	else if((ADC2_ConvertedValue>EXT1_CalValueAD_Zero)&&(ADC2_ConvertedValue<=EXT1_CalValueAD_3KG))
////	{
////		conresult=EXT1_SetValueHMI_3KG*(ADC2_ConvertedValue-EXT1_CalValueAD_Zero)/(EXT1_CalValueAD_3KG-EXT1_CalValueAD_Zero);
////	}
////	else 
////	{
////		conresult=(EXT1_SetValueHMI_5KG-EXT1_SetValueHMI_3KG)*(ADC2_ConvertedValue-EXT1_CalValueAD_3KG)/(EXT1_CalValueAD_5KG-EXT1_CalValueAD_3KG)+EXT1_SetValueHMI_3KG;
////	}      
////         
//        return  (CPU_INT16S)conresult;
//}

