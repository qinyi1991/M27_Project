#ifndef __APP_CH2_ACTION_H
#define __APP_CH2_ACTION_H

#include <includes.h>

void  CH2_Platen_Drop(void);
void  CH2_Platen_Rise(void);
void  CH2_Fixture_Action_IN(void);
void  CH2_Fixture_Action_OUT(void);
//�򿪹ر�vbus
void CH2_POWER_ON(void);
void CH2_POWER_OFF(void);
//ǿ�Ƽ���
void CH2_FORCEDLD_OPEN(void);
void CH2_FORCEDLD_CLOSE(void);
//ƽ̨�˳�������
void CH2_Platform_IN(void);
void CH2_Platform_OUT(void);
void CH2_Platform_Stop(void);
void CH2_Action_Platen_IN_OUT(); //ƽ̨��������
//������
void CH2_CNT_Trigger(void);
//״̬��ʾ��
void CH2_PlatformLED_FLD(CPU_INT08U Statues);
void CH2_PlatformLED_ERROR(CPU_INT08U Statues);
//������ť
void CH2_Singal_Step_Button(void);
//����쳣��Ϣ
void Check_Fixture_CH2_AbnormalInfo(void);
void CH2_Action_Platen_IN_OUT();
void Clear_CH2_Flag(void);
void Fixture_CH2_StatuesInfo(void); 

//ͨ�����������������
CPU_INT08U CH2_UartCMD_Analyze(void);
#endif