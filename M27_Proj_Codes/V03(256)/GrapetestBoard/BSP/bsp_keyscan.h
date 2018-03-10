#ifndef BSP_KEYSCAN_H
#define  BSP_KEYSCAN_H

//包含相关头文件
#include <includes.h>

///函数声明
void Fixture_Sensor_Scan(void *p_arg);
EXT CPU_INT08U  Timer_ARLARM;
void EXTI0_Configuration(void);
unsigned int Calculate_Temperature(float INPUTMV);
unsigned int Dectect_ENV_Temprature(void);
#endif