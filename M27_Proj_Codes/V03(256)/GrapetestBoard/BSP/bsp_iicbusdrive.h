
#ifndef __BSP_IICBUSDRIVE_H
#define __BSP_IICBUSDRIVE_H

#include <includes.h>

//-----------------------------------------------------------------
//	Modbus
//-----------------------------------------------------------------
#define   ModbusCoilAdd       200	   //200-254

#define   ModbusRegAdd        0     //0-200
#define   OperateAdd(a,b)     a+b

//-----------------------------------------------------------------
//	AT24c02
//-----------------------------------------------------------------
#define    WriteDeviceIIC    0xA0	   //写
#define    ReadDeviceIIC     0xA1	   //读

//-----------------------------------------------------------------
//	TPL0401 Definitions
//-----------------------------------------------------------------
#define TPL0401A_ADDRESS					0x2E					//TPL0401A address is fixed 010 1110 + (R/W bit)
#define TPL0401B_ADDRESS					0x3E					//TPL0401B address is fixed 011 1110 + (R/W bit)
#define TPL0401_R                                               0x00

///IIC_1总线
#define GPIO_Pin_SDA1  GPIO_Pin_7
#define Set_IIC_SCL1  {GPIO_SetBits(GPIOB,GPIO_Pin_6);}
#define Clr_IIC_SCL1  {GPIO_ResetBits(GPIOB,GPIO_Pin_6);} 
#define Set_IIC_SDA1  {GPIO_SetBits(GPIOB,GPIO_Pin_SDA1);}
#define Clr_IIC_SDA1  {GPIO_ResetBits(GPIOB,GPIO_Pin_SDA1);} 
#define READ_SDA1     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_SDA1)

///IIC_2总线
#define GPIO_Pin_SDA2  GPIO_Pin_11

#define Set_IIC_SCL2  {GPIO_SetBits(GPIOB,GPIO_Pin_10);}
#define Clr_IIC_SCL2  {GPIO_ResetBits(GPIOB,GPIO_Pin_10);} 

#define Set_IIC_SDA2  {GPIO_SetBits(GPIOB,GPIO_Pin_SDA2);}
#define Clr_IIC_SDA2  {GPIO_ResetBits(GPIOB,GPIO_Pin_SDA2);} 
#define READ_SDA2      GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_SDA2)





/**********************************************************************/
void IIC_Configuration(void);

void delay_Nus(CPU_INT16U us);  //延时us,delya function
void delay_Nms(CPU_INT16U ms);  //延时ms

void IIC_Write_Data(CPU_INT16U RomAddress,CPU_INT08U data);
CPU_INT08U IIC_Read_Data(CPU_INT16U RomAddress);

void IIC_Write_1Byte(CPU_INT16U RomAddress, void *udata);
void IIC_Read_1Byte(CPU_INT16U RomAddress,void *udata);

void IIC_Write_2Byte(CPU_INT16U RomAddress, void *udata);
void IIC_Read_2Byte(CPU_INT16U RomAddress,void *udata);

void IIC_Write_4Byte(CPU_INT16U RomAddress,void *udata);
void IIC_Read_4Byte(CPU_INT16U RomAddress,void *udata);

void IIC_Write_double(CPU_INT16U RomAddress,double udata);
double IIC_Read_double(CPU_INT16U RomAddress);

///读写一片数据
void IIC_Write_Nbyte(CPU_INT08U *pc,CPU_INT16U Addr,CPU_INT08U number);
void IIC_Read_Nbyte(CPU_INT08U *pc,CPU_INT16U Addr,CPU_INT08U number);


EXT  CPU_INT08U IIC_Channel;//通道







#endif
