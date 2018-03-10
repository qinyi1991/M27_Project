#ifndef  MODBUS_H
#define  MODBUS_H

#include <includes.h>	/* ���ļ������˱����.h�ļ� */
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

EXT CPU_INT08U	receCount;	            //���յ����ֽڸ�
EXT CPU_INT08U  sendBuf[100],receBuf[100];  //���ͽ��ջ���
EXT CPU_INT08U  redrxBuf[20];
EXT CPU_INT08U	redrxCount;	           //���յ����ֽڸ�

EXT CPU_INT08U	receTimeOut;               //���ճ�ʱ

EXT CPU_INT08U  CoilData[800];             //modbus ��Ȧ״̬   1--400 ����EEPRAM
EXT CPU_INT16S  RegData[200];	           //modbus �Ĵ���״̬ 1-100  ����EEPRAM

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



