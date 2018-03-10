#include "bsp_spi_AD1248.h"

CPU_INT16S ChannelADC_Value[4];

CPU_INT16S Temp_CN;

#define ADS1248_CS_H()             ADS1248_CS2=1;
#define ADS1248_CS_L()             ADS1248_CS2=0;

#define ADS1248_RST_H()            ADS1248_RST2=1;
#define ADS1248_RST_L()            ADS1248_RST2=0;

#define ADS1248_START_H()          ADS1248_START2=1;
#define ADS1248_START_L()          ADS1248_START2=0;

//读取DRRDY信号
void ReadstandbySignalADS1248(void);
void ADS1248_Reset(void);
void ADS1248_Nop(void);

void EXTI1_Configuration(void)
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);     

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;  //PD8		  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_Init(GPIOD,&GPIO_InitStructure);
        
        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig( NVIC_PriorityGroup_0 ); //中断优先级配置
        NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn ;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource8);	 
	EXTI_DeInit();    
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 ;		
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	 
	EXTI_Init(&EXTI_InitStructure);
        
	EXTI_ClearITPendingBit(EXTI_Line8);
        
        BSP_IntVectSet(BSP_INT_ID_EXTI9_5 ,ReadstandbySignalADS1248);
        BSP_IntEn(BSP_INT_ID_EXTI9_5 );    
}

//电平变化可以进中断
void ReadstandbySignalADS1248(void)
{
       static uint8_t tmp_H,tmp_M,tmp_L,Mux_number=0;
      uint16_t tmp_value=0;
      
      if(EXTI_GetITStatus(EXTI_Line8) == SET)
      {
          tmp_H= ADS1248_ReadByte();
          tmp_M= ADS1248_ReadByte();
          tmp_L= ADS1248_ReadByte();
          tmp_value|=(tmp_H<<8)|(tmp_M);
       
          //通道一
          if(Mux_number<=3) {ChannelADC_Value[Mux_number++]=(int16_t)(tmp_value);}
       
          if(Mux_number==1) {
            ADS1248_SetInputChannel(P_AIN2, N_AIN3);
          //  ADS1248_SetIDAC(IDAC1_AIN2, IDAC2_AIN3, IMAG_1000);              // 设置电流源
          }
          if(Mux_number==2) {
            ADS1248_SetInputChannel(P_AIN4, N_AIN5);
           // ADS1248_SetIDAC(IDAC1_AIN4, IDAC2_AIN5, IMAG_1000);              // 设置电流源
          }
          if(Mux_number==3) {
            ADS1248_SetInputChannel(P_AIN6, N_AIN7);
          //  ADS1248_SetIDAC(IDAC1_AIN6, IDAC2_AIN7, IMAG_1000);              // 设置电流源
          }
          if(Mux_number==4) {
            Mux_number=0;
            ADS1248_SetInputChannel(P_AIN0, N_AIN1);
          //  ADS1248_SetIDAC(IDAC1_AIN0, IDAC2_AIN1, IMAG_1000);              // 设置电流源
          }
  
          EXTI_ClearITPendingBit(EXTI_Line8); 
      }
}

/*******************************************************************************
* @Function Name: ADS1248_Init
* @Description  : ADS1248初始化
* @param        : void
* @retval       : void
* @date         : 2014/10/04
*******************************************************************************/
void ADS1248_Init(void)
{
    ADS1248_RST_L();
    ADS1248_START_L();
    
    delay_Nms(4);
    ADS1248_RST_H();
    ADS1248_START_H();
    
    delay_Nms(20);
    ADS1248_CS_L(); 
    delay_Nms(10);
    ADS1248_Reset();
    delay_Nms(10);
    ADS1248_SetInputChannel(P_AIN0, N_AIN1);
    
#ifdef  USE_INTER_VREF
    ADS1248_SetReference(REF_Inter_AlwaysOn, SELT_Inter);                    // 设置内部基准为参考源
#else  /*   USE REF0  */
    delay_Nms(1);
    ADS1248_SetReference(REF_Inter_AlwaysOn, SELT_REF0);                      // 设置外部REF0为参考源
#endif
    
    delay_Nms(10);
    //ADS1248_SetIDAC(IDAC1_AIN0, IDAC2_AIN1, IMAG_1000);                       // 设置电流源
    ADS1248_SetPGAGainAndDataRate(PGAGain_128, DataRate_20);                    // 设置PGA倍数和传输速率
}
//Reset 指令 
void ADS1248_Reset(void)
{
  SPI1_ReadWrite(ADS_RESET);
  SPI1_ReadWrite(0x00);
  
}

//NOP 指令 
void ADS1248_Nop(void)
{
  SPI1_ReadWrite(ADS_NOP);
  SPI1_ReadWrite(0x00); 
}

/*******************************************************************************
* @Function Name: ADS1248_ReadByte
* @Description  : Reads a byte from the SPI Device
* @param                : void
* @retval                   : u8 Byte Read from the SPI Device
* @date                           : 2014/10/04
*******************************************************************************/
uint8_t ADS1248_ReadByte(void)
{
    return (SPI1_ReadWrite(0x00));
}

/*******************************************************************************
* @Function Name: ADS1248_SetIDAC
* @Description  :
* @param                : u8 idac1
* @param                : u8 idac2
* @param                : u8 idacImage
* @retval               : void
* @date                 : 2014/10/04
*******************************************************************************/
void ADS1248_SetIDAC(u8 idac1, u8 idac2, u8 idacImage)
{
    SPI1_ReadWrite(ADS_WREG | ADS_IDAC0);
    SPI1_ReadWrite(0x01);
    SPI1_ReadWrite(idacImage);
    SPI1_ReadWrite(idac1 | idac2);
}

/*******************************************************************************
* @Function Name: ADS1248_SetPGAGainAndDataRate
* @Description  : Set PGA gain and data rate (SPS)
* @param                : u8 pgaGain
* @param                : u8 dataRate
* @retval               : void
* @date                 : 2014/10/04
*******************************************************************************/
void ADS1248_SetPGAGainAndDataRate(u8 pgaGain, u8 dataRate)
{
    SPI1_ReadWrite(ADS_WREG | ADS_SYS0);
    SPI1_ReadWrite(0x00);
    SPI1_ReadWrite(pgaGain | dataRate);
}

/*******************************************************************************
* @Function Name: ADS1248_SetInputChannel
* @Description  :
* @param                : u8 positiveChannel
* @param                : u8 negativeChannel
* @retval               : void
* @date                 : 2014/10/04
*******************************************************************************/
void ADS1248_SetInputChannel(u8 positiveChannel, u8 negativeChannel)
{
    SPI1_ReadWrite(ADS_WREG | ADS_MUX0);
    SPI1_ReadWrite(0x00);
    SPI1_ReadWrite(positiveChannel | negativeChannel);
}

/*******************************************************************************
* @Function Name: ADS1248_SetReference
* @Description  :
* @param                : u8 interVrefOnOff
* @param                : u8 refSelected
* @retval               : void
* @date                 : 2014/10/04
*******************************************************************************/
void ADS1248_SetReference(u8 interVrefOnOff, u8 refSelected)
{
    SPI1_ReadWrite(ADS_WREG | ADS_MUX1);
    SPI1_ReadWrite(0x00);
    SPI1_ReadWrite(interVrefOnOff | refSelected);
}
