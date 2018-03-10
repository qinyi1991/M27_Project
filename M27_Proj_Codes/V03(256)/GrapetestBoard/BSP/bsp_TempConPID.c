#include "bsp_TempConPID.h"

unsigned char TimerFlag1s=0;
unsigned int TimerCount1ms=0;
unsigned int PWMCount=0;//,PWM2Count=0,PWM3Count=0,PWM4Count=0;
unsigned  int PWMNewValue[4]={0};
unsigned char PWMFlag[4]={0};
//unsigned char PWMOff[4]={(CONTL1_OFF),(CONTL1_OFF),(CONTL1_OFF),(CONTL1_OFF)};
unsigned char Fist_Flag=0;
float PARA_SV[4],PARA_P[4],PARA_I[4],PARA_D[4];

float LastPV[4],NewPV[4],SVVALUE[4];
float PVError[4]={0};
float IntegralValue[4]={0};
float ErrorValue[4]={0};
float PARA_PM=6.5;
void CONTROL_OUT(void);
void PWMOUT(unsigned char pwmchannel);
void PIDVALUE(unsigned char ctrlch);
//===================================================
void CONTROL_OUT(void)
{
      unsigned char j=0;
      PARA_P[0]=Work_KP;
      PARA_I[0]=Work_TI;
      PARA_D[0]=Work_TD;
      PARA_SV[0]=SET_Temperature; 
      NewPV[0]=RT_Temperature;;
      
      //初始化一次
      if(Fist_Flag == 0) {Fist_Flag=1;SVVALUE[0]=RT_Temperature;}
        
      if(TimerFlag1s==1)		
      {
            TimerFlag1s=0;
            TimerCount1ms=0;		
//            for(j=0;j<4;j++)
//            {
              PIDVALUE(j);		
//            }
       }
      if(PWMCount>=1000)   
      {
              PWMCount=0;
//              for(j=0;j<4;j++)
//              {
                PWMOUT(j);
//              }
      }
      
//      for(j=0;j<4;j++)
//      {
            if(PWMCount>=PWMNewValue[j])   
            {  
              PWMFlag[j]=1;
            }
            else
            {
              PWMFlag[j]=0;
            }
//      }
         OPEN_Relay=PWMFlag[0];
//          OUT1==PWMFlag[0];				
//          OUT2=PWMFlag[1];
//          OUT3=PWMFlag[2];
//          OUT4=PWMFlag[3];
	
}
//=================================================//////////////////
void PWMOUT(unsigned char j)	//????OUT1????±è
{
  if((ErrorValue[j]+IntegralValue[j])<0)
  {
    PWMNewValue[j]=0;
    //CONTL1_OFF;
  }
  else if((ErrorValue[j]+IntegralValue[j])>=1000)
  {			
    PWMNewValue[j]=1000;
    //CONTL1_ON;
  }
  else
  {
    PWMNewValue[j]=(unsigned int)(ErrorValue[j]+IntegralValue[j]+0.5);
    //CONTL1_ON;
  }
}
//==============================????3?D?μ?ê?3?á?=======================
void PIDVALUE(unsigned char ctrlch)		
{
      unsigned char i=0;
      i=ctrlch;
      PVError[i]=NewPV[i]-LastPV[i];
      SVVALUE[i]=(PARA_PM*NewPV[i]+SVVALUE[i]*PARA_D[i]+PVError[i]*PARA_PM*PARA_D[i])/(PARA_PM+PARA_D[i]);
      LastPV[i]=NewPV[i];	
      ErrorValue[i]=PARA_SV[i]-SVVALUE[i];
      if(ErrorValue[i]>=PARA_P[i])
      {
          IntegralValue[i]=0;
          ErrorValue[i]=1000;
      }
      else if(ErrorValue[i]+PARA_P[i]<0)
      {
          IntegralValue[i]=0;
          ErrorValue[i]=-1000;
      }
      else
      {
          ErrorValue[i]=ErrorValue[i]*1000/PARA_P[i];
          IntegralValue[i]=IntegralValue[i]+(ErrorValue[i]/PARA_I[i]);
          if(IntegralValue[i]<0)		
          {
            IntegralValue[i]=0;
          }
          if(IntegralValue[i]>=1000)	
          {
            IntegralValue[i]=1000;
          }
      }
}



	
	
