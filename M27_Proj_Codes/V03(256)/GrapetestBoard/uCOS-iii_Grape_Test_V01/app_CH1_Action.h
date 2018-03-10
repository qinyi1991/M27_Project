#ifndef __APP_CH1_ACTION_H
#define __APP_CH1_ACTION_H

#include <includes.h>

#define LED_CLOSE   0
#define LED_OPEN    1

void  CH1_Platen_Drop(void);
void  CH1_Platen_Rise(void);
void  CH1_Fixture_Action_IN(void);
void  CH1_Fixture_Action_OUT(void);
//�򿪹ر�vbus
void CH1_POWER_ON(void);
void CH1_POWER_OFF(void);
//ǿ�Ƽ���
void CH1_FORCEDLD_OPEN(void);
void CH1_FORCEDLD_CLOSE(void);
//ƽ̨�˳�������
void CH1_Platform_IN(void);
void CH1_Platform_OUT(void);
void CH1_Platform_Stop(void);
void CH1_Action_Platen_IN_OUT();
//������
void CH1_CNT_Trigger(void);
//״̬��ʾ��
void CH1_PlatformLED_FLD(CPU_INT08U Statues);
void CH1_PlatformLED_ERROR(CPU_INT08U Statues);
//������ť
void CH1_Singal_Step_Button(void);
//����쳣��Ϣ
void Check_Fixture_CH1_AbnormalInfo(void);
void Fixture_CH1_StatuesInfo(void);
void Clear_CH1_Flag(void);

//����ָ��
CPU_INT08U CH1_UartCMD_Analyze(void);
#endif