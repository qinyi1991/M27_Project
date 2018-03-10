#ifndef __APP_CH4_ACTION_H
#define __APP_CH4_ACTION_H

#include <includes.h>
void  CH4_Platen_Drop(void);
void  CH4_Platen_Rise(void);
void  CH4_Fixture_Action_IN(void);
void  CH4_Fixture_Action_OUT(void);
//�򿪹ر�vbus
void CH4_POWER_ON(void);
void CH4_POWER_OFF(void);
//ǿ�Ƽ���
void CH4_FORCEDLD_OPEN(void);
void CH4_FORCEDLD_CLOSE(void);
//ƽ̨�˳�������
void CH4_Platform_IN(void);
void CH4_Platform_OUT(void);
void CH4_Platform_Stop(void);
void Fixture_CH4_StatuesInfo(void); //ƽ̨��������
//������
void CH4_CNT_Trigger(void);
//״̬��ʾ��
void CH4_PlatformLED_FLD(CPU_INT08U Statues);
void CH4_PlatformLED_ERROR(CPU_INT08U Statues);

//������ť
void CH4_Singal_Step_Button(void);
//����쳣��Ϣ
void Check_Fixture_CH4_AbnormalInfo(void);
void CH4_Action_Platen_IN_OUT();
void Clear_CH4_Flag(void);
void Fixture_CH4_StatuesInfo(void);

//ͨ�����������������
CPU_INT08U CH4_UartCMD_Analyze(void);

#endif