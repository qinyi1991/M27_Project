#ifndef BSP_KEYSCAN_H
#define  BSP_KEYSCAN_H

//�������ͷ�ļ�
#include <includes.h>

///��������
void Fixture_Sensor_Scan(void *p_arg);
EXT CPU_INT08U  Timer_ARLARM;
void EXTI0_Configuration(void);
unsigned int Calculate_Temperature(float INPUTMV);
unsigned int Dectect_ENV_Temprature(void);
#endif