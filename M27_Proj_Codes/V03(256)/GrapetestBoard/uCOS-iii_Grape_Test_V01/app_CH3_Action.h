#ifndef __APP_CH3_ACTION_H
#define __APP_CH3_ACTION_H

#include <includes.h>

void  CH3_Platen_Drop(void);
void  CH3_Platen_Rise(void);
void  CH3_Fixture_Action_IN(void);
void  CH3_Fixture_Action_OUT(void);

//打开关闭vbus
void CH3_POWER_ON(void);
void CH3_POWER_OFF(void);

//强制加载
void CH3_FORCEDLD_OPEN(void);
void CH3_FORCEDLD_CLOSE(void);

//平台退出，推入
void CH3_Platform_IN(void);
void CH3_Platform_OUT(void);
void CH3_Platform_Stop(void);
void CH3_Action_Platen_IN_OUT(); //平台进出触发
//计数器
void CH3_CNT_Trigger(void);
//状态显示灯
void CH3_PlatformLED_FLD(CPU_INT08U Statues);
void CH3_PlatformLED_ERROR(CPU_INT08U Statues);

//单步按钮
void CH3_Singal_Step_Button(void);

//检测异常信息
void Check_Fixture_CH3_AbnormalInfo(void);
void CH3_Action_Platen_IN_OUT();
void Clear_CH3_Flag(void);
void Fixture_CH3_StatuesInfo(void);

//通道三，串口命令解析
CPU_INT08U CH3_UartCMD_Analyze(void);
#endif