#ifndef  CF300_H
#define  CF300_H
#include <bsp.h>
extern CPU_FP32  ChVal[2];
extern void ReadMeVal(void);//读主测量值
extern void CheckMessage(void);//两通道数据存在数组中
extern float Left_Back_Cell;//右压力值
extern float Right_Front_Cell;//左压力值
extern void RightLoadCellCheck(char *ptr);
extern void LeftLoadCellCheck(char *ptr);
#endif