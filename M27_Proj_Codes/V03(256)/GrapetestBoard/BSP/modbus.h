#ifndef  MODBUS_H
#define  MODBUS_H

#include <includes.h>	/* 该文件包含了必需的.h文件 */
#include <modus_Controltargetconf.h>

typedef  struct 
{
	CPU_INT16U       Add;
	CPU_INT16U       num;

}Coil_Dis;

typedef  struct 
{
	CPU_INT16U   Add;
	CPU_INT16U   num;

} Regis_Dis;

EXT CPU_INT08U	receCount;	            //接收到的字节个
EXT CPU_INT08U  sendBuf[100],receBuf[100];  //发送接收缓冲
EXT CPU_INT08U  redrxBuf[20];
EXT CPU_INT08U	redrxCount;	           //接收到的字节个

EXT CPU_INT08U	receTimeOut;               //接收超时

EXT CPU_INT08U  CoilData[800];             //modbus 线圈状态   1--400 存入EEPRAM
EXT CPU_INT16S  RegData[200];	           //modbus 寄存器状态 1-100  存入EEPRAM

EXT Coil_Dis   Coil;
EXT Regis_Dis  Rdis;


void beginSend(CPU_INT16U sendCount);
void checkComm0Modbus(void);
void readCoil(void);
void readRegisters(void);
void forceSingleCoil(void);
void presetSingleRegister(void);
void presetMultipleRegisters(void);
void forceMultipleCoils(void);
CPU_INT16U SaveRegisterToEEPROM(CPU_INT16U Addr);
CPU_INT16U Write_4byte(void *Add,void *d);
#endif



