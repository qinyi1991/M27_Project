#ifndef __APP_CH1_ACTION_H
#define __APP_CH1_ACTION_H

#include <includes.h>

#define LED_CLOSE   0
#define LED_OPEN    1

void  CH1_Platen_Drop(void);
void  CH1_Platen_Rise(void);
void  CH1_Fixture_Action_IN(void);
void  CH1_Fixture_Action_OUT(void);
//打开关闭vbus
void CH1_POWER_ON(void);
void CH1_POWER_OFF(void);
//强制加载
void CH1_FORCEDLD_OPEN(void);
void CH1_FORCEDLD_CLOSE(void);
//平台退出，推入
void CH1_Platform_IN(void);
void CH1_Platform_OUT(void);
void CH1_Platform_Stop(void);
void CH1_Action_Platen_IN_OUT();
//计数器
void CH1_CNT_Trigger(void);
//状态显示灯
void CH1_PlatformLED_FLD(CPU_INT08U Statues);
void CH1_PlatformLED_ERROR(CPU_INT08U Statues);
//单步按钮
void CH1_Singal_Step_Button(void);
//检测异常信息
void Check_Fixture_CH1_AbnormalInfo(void);
void Fixture_CH1_StatuesInfo(void);
void Clear_CH1_Flag(void);

//解析指令
CPU_INT08U CH1_UartCMD_Analyze(void);
#endif