#ifndef  CF300_H
#define  CF300_H
#include <bsp.h>
extern CPU_FP32  ChVal[2];
extern void ReadMeVal(void);//��������ֵ
extern void CheckMessage(void);//��ͨ�����ݴ���������
extern float Left_Back_Cell;//��ѹ��ֵ
extern float Right_Front_Cell;//��ѹ��ֵ
extern void RightLoadCellCheck(char *ptr);
extern void LeftLoadCellCheck(char *ptr);
#endif