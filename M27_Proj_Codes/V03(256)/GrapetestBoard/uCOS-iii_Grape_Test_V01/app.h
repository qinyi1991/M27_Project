/***********************����appȫ�ֱ���***************************************************************************/
#ifndef __APP_H
#define __APP_H

#include <includes.h>

#define   RXBUFFERSIZE   50
#define   BUFFERSIZE   50
#define AUTO  1
#define HAND  0
#define DANGEROUS 0
#define SAFE 1
#define OFF 0
#define ON 1
#define NG 0
#define OK 1

EXT CPU_INT32U   R_Location_First;
EXT CPU_INT32U   R_Location_Second;
EXT CPU_INT32U   R_Woking_Distance;
EXT CPU_INT32U   L_Location_First;
EXT CPU_INT32U   L_Location_Second;
EXT CPU_INT32U   L_Woking_Distance;
EXT OS_TCB       AppTaskSerDataHandlingTCB; 
EXT OS_TCB       AppTaskSerCmdTCB;
//�������
//CH1 ����г�
EXT CPU_INT32U   CH1_Mtion_ABS_POS_Relative;
EXT CPU_INT32U   CH1_Mtion_ABS_POS;

EXT CPU_INT32U   CH3_Mtion_ABS_POS_Relative;
EXT CPU_INT32U   CH3_Mtion_ABS_POS;
EXT CPU_INT16U Manual_SpeedL,Manual_SpeedR;//�ֶ���������ٶ�
EXT uint8_t *L_Press;
EXT uint8_t *R_Press;
EXT uint8_t *L_Pull;
EXT uint8_t *R_Pull;
EXT uint8_t *L_Pull_Max;
EXT uint8_t *R_Pull_Max;
EXT float L_Press_Pressure;//����ѹ������ֵ�趨
EXT float R_Press_Pressure;//�ҵ��ѹ������ֵ�趨
EXT float L_Pull_Pressure;//������������ֵ�趨
EXT float R_Pull_Pressure;//�ҵ����������ֵ�趨
EXT float L_Pull_Max_Pressure;//���������������ֵ�趨
EXT float R_Pull_Max_Pressure;//�ҵ�������������ֵ�趨
EXT CPU_INT16U Manual_SpeedL,Manual_SpeedR;//�ֶ���������ٶ�
EXT CPU_INT16U Auto_SpeedL,Auto_SpeedR;    //�Զ���������ٶ�
EXT CPU_INT16U Woking_SpeedL,Woking_SpeedR;//�ν�����ٶ�

//EXT CPU_INT16U HeatTime;//����ʱ��
//EXT CPU_INT16U HeatRunTime;//����ʱ��

EXT CPU_INT16U RunTime;//�������ʱ��
EXT CPU_INT16U OverPreWaitTimeL;//L�����ѹͣ��ʱ��
EXT CPU_INT16U OverPreWaitTimeR;//R�����ѹͣ��ʱ��
EXT CPU_INT16U FinishedWaitTime;//�����ɺ�ȴ���λʱ��
EXT CPU_INT16U Vacuum_PWait;    //����յȴ����ʱ��
EXT CPU_INT16U HeatMessage;//�¿ؼ���OK Message
EXT CPU_INT32U Pass_Product_Counters;

EXT CPU_INT32U Pass_Product_Counters;
EXT CPU_INT08U Step,MotorLBackFlag,MotorRBackFlag;


void Reset_System(void);
void InputSensorDis(void);
void SetOutput(void);
void SetSysPara(void);
void ControlSequence(void);
void Protection_Data_Action(void);
void Setting_Statues(unsigned char Statues_Code);
void WritFP32TOvari(CPU_FP32 *Temp_dat,CPU_INT16U Add);
void PrinfStringToScreen(uint8_t *ptr_str,uint16_t DIFF_ADDR);
void Stop_Scan(void);
#endif