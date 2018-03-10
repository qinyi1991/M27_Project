/***********************定义app全局变量***************************************************************************/
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
//电机控制
//CH1 相对行程
EXT CPU_INT32U   CH1_Mtion_ABS_POS_Relative;
EXT CPU_INT32U   CH1_Mtion_ABS_POS;

EXT CPU_INT32U   CH3_Mtion_ABS_POS_Relative;
EXT CPU_INT32U   CH3_Mtion_ABS_POS;
EXT CPU_INT16U Manual_SpeedL,Manual_SpeedR;//手动操作电机速度
EXT uint8_t *L_Press;
EXT uint8_t *R_Press;
EXT uint8_t *L_Pull;
EXT uint8_t *R_Pull;
EXT uint8_t *L_Pull_Max;
EXT uint8_t *R_Pull_Max;
EXT float L_Press_Pressure;//左电机压力报警值设定
EXT float R_Press_Pressure;//右电机压力报警值设定
EXT float L_Pull_Pressure;//左电机拉力报警值设定
EXT float R_Pull_Pressure;//右电机拉力报警值设定
EXT float L_Pull_Max_Pressure;//左电机最大拉力报警值设定
EXT float R_Pull_Max_Pressure;//右电机最大拉力报警值设定
EXT CPU_INT16U Manual_SpeedL,Manual_SpeedR;//手动操作电机速度
EXT CPU_INT16U Auto_SpeedL,Auto_SpeedR;    //自动操作电机速度
EXT CPU_INT16U Woking_SpeedL,Woking_SpeedR;//刮胶电机速度

//EXT CPU_INT16U HeatTime;//加热时间
//EXT CPU_INT16U HeatRunTime;//加热时间

EXT CPU_INT16U RunTime;//拆机运行时间
EXT CPU_INT16U OverPreWaitTimeL;//L电机过压停机时间
EXT CPU_INT16U OverPreWaitTimeR;//R电机过压停机时间
EXT CPU_INT16U FinishedWaitTime;//拆机完成后等待复位时间
EXT CPU_INT16U Vacuum_PWait;    //上真空等待检测时间
EXT CPU_INT16U HeatMessage;//温控加热OK Message
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