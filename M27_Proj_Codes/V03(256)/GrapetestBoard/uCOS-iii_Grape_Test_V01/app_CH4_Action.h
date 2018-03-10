#ifndef __APP_CH4_ACTION_H
#define __APP_CH4_ACTION_H

#include <includes.h>
void  CH4_Platen_Drop(void);
void  CH4_Platen_Rise(void);
void  CH4_Fixture_Action_IN(void);
void  CH4_Fixture_Action_OUT(void);
//打开关闭vbus
void CH4_POWER_ON(void);
void CH4_POWER_OFF(void);
//强制加载
void CH4_FORCEDLD_OPEN(void);
void CH4_FORCEDLD_CLOSE(void);
//平台退出，推入
void CH4_Platform_IN(void);
void CH4_Platform_OUT(void);
void CH4_Platform_Stop(void);
void Fixture_CH4_StatuesInfo(void); //平台进出触发
//计数器
void CH4_CNT_Trigger(void);
//状态显示灯
void CH4_PlatformLED_FLD(CPU_INT08U Statues);
void CH4_PlatformLED_ERROR(CPU_INT08U Statues);

//单步按钮
void CH4_Singal_Step_Button(void);
//检测异常信息
void Check_Fixture_CH4_AbnormalInfo(void);
void CH4_Action_Platen_IN_OUT();
void Clear_CH4_Flag(void);
void Fixture_CH4_StatuesInfo(void);

//通道三，串口命令解析
CPU_INT08U CH4_UartCMD_Analyze(void);

#endif