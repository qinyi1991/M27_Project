#include "bsp_spi.h"

void SPI1_Init(void)
{
      SPI_InitTypeDef SPI_InitStructure;
      GPIO_InitTypeDef GPIO_InitStructure;
      
       //GPIO_Remap_SWJ_NoJTRST
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
     // GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
      
      /* Configure SPI1 pins: RST1*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      /* Configure START1 */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
      GPIO_Init(GPIOB, &GPIO_InitStructure);
      /*  Configure CS1*/
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      
      ADS1248_CS2=1;ADS1248_RST2=1;ADS1248_START2=0;
      
      /* Configure SPI1 pins: SCK, MISO and MOSI */
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      
      /* Enable SPI1 and GPIOB clocks */
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
      SPI_Cmd(SPI1,DISABLE);  
      
      /* SPI1 configuration */ 
      SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
      SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
      SPI_InitStructure.SPI_CRCPolynomial = 7;
      SPI_Init(SPI1, &SPI_InitStructure);
      
      /* Enable SPI1  */
      SPI_Cmd(SPI1, ENABLE);
      
}

unsigned char SPI1_ReadWrite(unsigned char writedat)
{
   /* Loop while DR register in not emplty */
   while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

   /* Send byte through the SPI1 peripheral */
   SPI_SendData(SPI1, writedat);

   /* Wait to receive a byte */
   while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);

   /* Return the byte read from the SPI bus */
   return SPI_ReceiveData(SPI1);
}



