#include <bsp.h>
#define BUSY  1
#define READY 0
float Left_Back_Cell=0.00;//��ѹ��ֵ
float Right_Front_Cell=0.00;//��ѹ��ֵ
uint8_t *srp=(uint8_t *)&Right_Front_Cell;
uint8_t *slp=(uint8_t *)&Left_Back_Cell;
CPU_FP32  ChVal[2];
CPU_INT08U Adr=0,CHNum,BusStatus=READY;
void ReadMeVal(void)//��������ֵ
{
  if((READY==BusStatus)&&(0==Adr))//�ӻ�1�Ĳ���ֵ
  {
   //��485����ʹ��
   BSP_Usart3_Printf ("#01\n"); 
   Adr=1;
   CHNum=1;
   BusStatus=BUSY;
   //��485����ʹ��
     
  }
  else if((READY==BusStatus)&&(2==Adr))//�ӻ�2�Ĳ���ֵ
  {
   //��485����ʹ��
   BSP_Usart3_Printf ("#02\n");
   Adr=1;
   CHNum=1;
   BusStatus=BUSY;
   //��485����ʹ�� 
  }
}
void CheckMessage(void)
{
  uint8_t ValH,ValM,ValL,ValD;
  ValH=Usart3RxBuf[1]-0x30;
  ValM=Usart3RxBuf[2]-0x30;
  ValL=Usart3RxBuf[3]-0x30;
  ValD=Usart3RxBuf[5]-0x30;
  if(Usart3RxBuf[1]=='+')
  {
    if(1==CHNum)//ͨ��1����
    {
     ChVal[0]=(CPU_FP32)(ValH*100+ValM*10+ValL+ValD/10);//
     BusStatus=READY;//���������� �ͷ�����
    }
    else        //ͨ��2����
    {
     ChVal[1]=(CPU_FP32)(ValH*100+ValM*10+ValL+ValD/10);// 
     BusStatus=READY;//���������� �ͷ�����
    }
  }
  else
  {
    if(1==CHNum)//ͨ��1����
    {
    ChVal[0]=-(CPU_FP32)(ValH*100+ValM*10+ValL+ValD/10);// 
    BusStatus=READY;//���������� �ͷ�����
    }
    else        //ͨ��2����
    {
    ChVal[1]=-(CPU_FP32)(ValH*100+ValM*10+ValL+ValD/10);// 
    BusStatus=READY;//���������� �ͷ�����
    } 
  }
}
void LeftLoadCellCheck(char *ptr)
{
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  if(ptr[1]=='+')
  {
  Left_Back_Cell=((ptr[2]-0x30)*10+(ptr[3]-0x30)*1+(ptr[5]-0x30)*0.1 +(ptr[6]-0x30)*0.01);//+00.01
  }
  else
  {
  Left_Back_Cell=((ptr[2]-0x30)*10+(ptr[3]-0x30)*1+(ptr[5]-0x30)*0.1 +(ptr[6]-0x30)*0.01)*(-1);//-00.01
  }
  *(Ptr_RegData+SRegisAdd9)=(slp[3]<<8)+slp[2];
  *(Ptr_RegData+SRegisAdd10)=(slp[1]<<8)+slp[0];
}

void RightLoadCellCheck(char *ptr)
{  
  CPU_INT16S   *Ptr_RegData;
  Ptr_RegData=RegData;
  if(ptr[1]=='+')
  {		
   Right_Front_Cell=((ptr[2]-0x30)*10+(ptr[3]-0x30)*1+(ptr[5]-0x30)*0.1 +(ptr[6]-0x30)*0.01);//+00.01
  }
  else
  {
   Right_Front_Cell=((ptr[2]-0x30)*10+(ptr[3]-0x30)*1+(ptr[5]-0x30)*0.1 +(ptr[6]-0x30)*0.01)*(-1);//-00.01
  }
  *(Ptr_RegData+SRegisAdd11)=(srp[3]<<8)+srp[2];
  *(Ptr_RegData+SRegisAdd12)=(srp[1]<<8)+srp[0];
}