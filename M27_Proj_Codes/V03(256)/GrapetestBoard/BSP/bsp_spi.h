/***********************定义app全局变量***************************************************************************/
#ifndef __BSP_SPI_AD127_H
#define __BSP_SPI_AD127_H

#include <includes.h>

//----------------------------------------------
#define SPI_FLAG_RXNE           SPI_I2S_FLAG_RXNE
#define SPI_FLAG_TXE            SPI_I2S_FLAG_TXE
#define SPI_FLAG_OVR            SPI_I2S_FLAG_OVR
#define SPI_FLAG_BSY            SPI_I2S_FLAG_BSY
//----------------------------------------------
#define SPI_GetFlagStatus       SPI_I2S_GetFlagStatus
#define SPI_SendData            SPI_I2S_SendData
#define SPI_ReceiveData         SPI_I2S_ReceiveData

//ADS1248相关宏定义
//ADS1248 2#
#define   ADS1248_RST2     PBout(12)
#define   ADS1248_CS2      PEout(15)
#define   ADS1248_START2   PEout(14)

void SPI1_Init(void);
unsigned char SPI1_ReadWrite(unsigned char writedat);

void SPI2_Init(void);
unsigned char SPI2_ReadWrite(unsigned char writedat);


#endif