#ifndef  BSP_MOTIONCONTROL_H
#define  BSP_MOTIONCONTROL_H

#include <includes.h>
#include "stm32f10x.h"

//�����ز���
#define    FreqCLK          72000000

#define    RISE             0	//����	
#define    DROP             1    //�½�

#define    SUBDIVISION     6400  //������ϸ��
#define    COMPENSATE       10    //��������

//���������
#define BSP_DIR1  PEout(4)
#define BSP_DIR2  PEout(3)
#define BSP_DIR3  PEout(2)
#define BSP_DIR4  PBout(4)
#define BSP_DIR5  PAout(15)
#define BSP_DIR6  PBout(3)
//�������״̬
typedef enum {
     DIR_CW = ((CPU_INT08U) 0x01), DIR_CCW = ((CPU_INT08U) 0x00)
}Typedef_MtionDirection;

extern volatile unsigned int KeepCounter,Cnt;
extern volatile unsigned char TIMER_OVRE_FLAG,TimeIsOver,HeatFinished,WaitFinished;
		

void MotionControl_Init();
int Motor1Starting_PWM2(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed);
int Motor2Starting_PWM6(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed);
int Motor3Starting_PWM4(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed);
int Motor4Starting_PWM5(Typedef_MtionDirection Motion_DIR,CPU_INT16U speed);

void MotorStop(TIM_TypeDef* TIMx);

#endif
