#ifndef __APP_CH2_ACTION_H
#define __APP_CH2_ACTION_H

#include <includes.h>

void  CH2_Platen_Drop(void);
void  CH2_Platen_Rise(void);
void  CH2_Fixture_Action_IN(void);
void  CH2_Fixture_Action_OUT(void);
//打开关闭vbus
void CH2_POWER_ON(void);
void CH2_POWER_OFF(void);
//强制加载
void CH2_FORCEDLD_OPEN(void);
void CH2_FORCEDLD_CLOSE(void);
//平台退出，推入
void CH2_Platform_IN(void);
void CH2_Platform_OUT(void);
void CH2_Platform_Stop(void);
void CH2_Action_Platen_IN_OUT(); //平台进出触发
//计数器
void CH2_CNT_Trigger(void);
//状态显示灯
void CH2_PlatformLED_FLD(CPU_INT08U Statues);
void CH2_PlatformLED_ERROR(CPU_INT08U Statues);
//单步按钮
void CH2_Singal_Step_Button(void);
//检测异常信息
void Check_Fixture_CH2_AbnormalInfo(void);
void CH2_Action_Platen_IN_OUT();
void Clear_CH2_Flag(void);
void Fixture_CH2_StatuesInfo(void); 

//通道二，串口命令解析
CPU_INT08U CH2_UartCMD_Analyze(void);
#endif