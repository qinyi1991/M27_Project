#include "app_ser_cmd.h"

static void Press_Calibration_handling();
static CPU_INT08U Computer_CMD_handling(CPU_INT08U Fixture_ID);
//static void Motor_parameter__handling();
static CPU_INT08U Fixture_ID_handling();


#define IDLENG 4

//大小写转换函数
int LetterToBigString(char *str)
{
    unsigned char i,len;
    
    len=Str_Len_N((const CPU_CHAR*)str,99);
    for(i=0;i<len;i++)
    {
	    if((str[i]>='a')&&(str[i]<='z'))
	    {
		    str[i]=str[i]-0x20; 
		  }
	  }
	 return 0;
}

//串口命令处理函数
void Ser_CMD_handling()
{
      CPU_INT08U  clear_nub,Fixture_ID;

      AppTask_Fixture_Status.F_RXERRO=0;
      //大小写转换
      LetterToBigString((CPU_CHAR*)recBuf);

      if(Str_Cmp((const CPU_CHAR*)"FIXINFO",(const CPU_CHAR*)recBuf)==0){
        //返回夹具信息
        AppTask_Fixture_Status.F_FixInfo=1;
//        BSP_Usart1_WrStr("\r\nFIXINFO SN:HW1TE00722\r\n");
//        BSP_Usart1_WrStr("\r\nFIXINFO NAME:MHA CAMERA\r\n");
//        BSP_Usart1_WrStrBSP_Usart1_WrStrBSP_Usart1_WrStr ("\r\nFIXINFO FACTORY:SR\r\n");             
//        BSP_Usart1_WrStrBSP_Usart1_WrStr ("\r\nFIXINFO SOLFTWARE VERSION:V01\r\n");
//        BSP_Usart1_WrStr ("\r\nFIXINFO HARDTWARE VERSION:SR_Main Board,V01\r\n");
//        BSP_Usart1_WrStr("\r\nFIXINFO DATE:20160803\r\n");
//        BSP_Usart1_WrStr("\r\nOK\r\n"); 
      }
      else if(Str_Cmp((const CPU_CHAR*)"HELP",(const CPU_CHAR*)recBuf)==0){
        //返回HELP信息
        AppTask_Fixture_Status.F_FixInfo=1;
//        BSP_Usart1_WrStrBSP_Usart1_WrStr("\r\n<id> DUT STAT                            //query dut status\r\n");             
//       // EXT1_BSP_Ser_Printf("\r\n<id> DUT STAT                            //query dut status\r\n"); 
//        BSP_Usart1_WrStr("\r\n<id> POWER?                              //query power status\r\n");             
//        EXT1_BSP_Ser_Printf("\r\n<id> RESULT=PASS/FAIL                    // set dut test result\r\n"); 
//        EXT1_BSP_Ser_Printf("\r\nFIXINFO                                  //query fixture information\r\n");             
//        EXT1_BSP_Ser_Printf("\r\n<id>ACTION= OPEN/CLOSE/RESET             //set test action\r\n"); 
//        EXT1_BSP_Ser_Printf("\r\n<id>ACTION= FORCEDLD_OPEN/FORCEDLD_CLOSE //set test action\r\n");  
//        EXT1_BSP_Ser_Printf("\r\n<id> TSTCNT                              //query test number\r\n");             
//        EXT1_BSP_Ser_Printf("\r\n<id> BUTTON=LOCK/UNLOCK                  //set button status\r\n");
//        EXT1_BSP_Ser_Printf("\r\nOK\r\n"); 
      }else{ 
        /*  if(AppTask_Sensor_Status.F_x17 == 1){
            EXT1_BSP_Ser_Printf("\r\n%d\r\n",ChannelADC_Value[0]);
            EXT1_BSP_Ser_Printf("\r\n%d\r\n",ChannelADC_Value[1]);
            EXT1_BSP_Ser_Printf("\r\n%d\r\n",ChannelADC_Value[2]);
            EXT1_BSP_Ser_Printf("\r\n%d\r\n",ChannelADC_Value[3]);
          }*/

      }

      //区分上位机命令&ID号
    /*  if(AppTask_Fixture_Status.F_FixInfo == 0 && AppTask_Sensor_Status.F_x17 == 0 && AppTask_Sensor_Status.F_x9 == 0){
          Fixture_ID = Fixture_ID_handling();
          if(Fixture_ID == 0)
          {
 //           Motor_parameter__handling();
            Press_Calibration_handling();
          }else{
            Computer_CMD_handling( Fixture_ID );
          }
      }*/
      
      AppTask_Fixture_Status.F_FixInfo=0;
      
      //清除缓冲区
      for(clear_nub=BUFFERSIZE-1;clear_nub>0;clear_nub--){
        recBuf[clear_nub]='\0';
      }
      
      recCount=0;  
}

//计算机命令处理
//测试VBUS切换，强制加载功能
//***************CH1
//BSP_SW1_VBUS=~BSP_SW1_VBUS; BSP_SW1_FLOAD=~BSP_SW1_FLOAD;
//***************CH2
//BSP_SW2_VBUS=~BSP_SW2_VBUS; BSP_SW2_FLOAD=~BSP_SW2_FLOAD;
//***************CH3
//BSP_SW3_VBUS=~BSP_SW3_VBUS; BSP_SW3_FLOAD=~BSP_SW3_FLOAD;
//***************CH4
//BSP_SW4_VBUS=~BSP_SW4_VBUS; BSP_SW4_FLOAD=~BSP_SW4_FLOAD;
static CPU_INT08U Computer_CMD_handling(CPU_INT08U Fixture_ID)
{
    if(Fixture_ID == 1){
 //     return CH1_UartCMD_Analyze();
    }
    else if(Fixture_ID == 2){
 //     return CH2_UartCMD_Analyze();
    }
    else if(Fixture_ID == 3){
 //     return CH3_UartCMD_Analyze();
    }
    else if(Fixture_ID == 4){
 //     return CH4_UartCMD_Analyze();
    }
    return 0;
}

//处理ID号
static CPU_INT08U Fixture_ID_handling()
{
      CPU_INT08U ID=0;
      if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"01",2) == 0 ) {ID=1;} 
      else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"02",2) == 0 ) {ID=2;} 
      else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"03",2) == 0 ) {ID=3;} 
      else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"04",2) == 0 ) {ID=4;} 
      else {ID=0;}
      
      return ID;
}

//电机参数处理
/*static void Motor_parameter__handling()
{
          //CH1，高端
          if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 HSPEED ",21) == 0 ) {//CH1速度H       
            CH1_Mtion_HSpeed = Conversion_fraction("CH1 SETMOTOR1 HSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd7),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 HSpeed:%.1f mm/s\r\n",CH1_Mtion_HSpeed/10.0);             
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 HDISTANCE ",24) == 0 ) {//CH1位置H
            CH1_Mtion_HDistance = Conversion_fraction("CH1 SETMOTOR1 HDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd5),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 HDistance:%.1f mm\r\n",CH1_Mtion_HDistance/10.0);                
          }   
          
          //CH1，中端 CH1_Mtion_MSpeed
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 MSPEED ",21) == 0 ) {//CH1速度M
            CH1_Mtion_MSpeed = Conversion_fraction("CH1 SETMOTOR1 MSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd11),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 MSpeed:%.1f mm/s\r\n",CH1_Mtion_MSpeed/10.0);               
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 MDISTANCE ",24) == 0 ) {//CH1位置M
            CH1_Mtion_MDistance = Conversion_fraction("CH1 SETMOTOR1 MDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd9),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 MDistance:%.1f mm\r\n",CH1_Mtion_MDistance/10.0);               
          }
          
          //CH1，低端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 LSPEED ",21) == 0 ) {//CH1速度L  
            CH1_Mtion_LSpeed = Conversion_fraction("CH1 SETMOTOR1 LSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd15),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 LSpeed:%.1f mm/s\r\n",CH1_Mtion_LSpeed/10.0);                
          }       
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 LDISTANCE ",24) == 0 ) {//CH1位置L
            CH1_Mtion_LDistance = Conversion_fraction("CH1 SETMOTOR1 LDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd13),2000);
            EXT1_BSP_Ser_Printf("CH1 SetMotor1 LDistance:%.1f mm\r\n",CH1_Mtion_LDistance/10.0);               
            
          }
          
          //CH2，高端
          if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 HSPEED ",21) == 0 ) {//速度
            CH2_Mtion_HSpeed = Conversion_fraction("CH2 SETMOTOR2 HSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd19),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 HSpeed:%.1f mm/s\r\n",CH2_Mtion_HSpeed/10.0);               
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 HDISTANCE ",24) == 0 ) {//位置
            CH2_Mtion_HDistance = Conversion_fraction("CH2 SETMOTOR2 HDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd17),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 HDistance:%.1f mm\r\n",CH2_Mtion_HDistance/10.0);      
          }
          
          //CH2，中端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 MSPEED ",21) == 0 ) {
            CH2_Mtion_MSpeed = Conversion_fraction("CH2 SETMOTOR2 MSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd23),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 MSpeed:%.1f mm/s\r\n",CH2_Mtion_MSpeed/10.0);                
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 MDISTANCE ",22) == 0 ) {
            CH2_Mtion_MDistance = Conversion_fraction("CH2 SETMOTOR2 MDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd21),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 MDistance:%.1f mm\r\n",CH2_Mtion_MDistance/10.0);                 
          }
          
          //CH2，低端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 LSPEED ",21) == 0 ) {
            CH2_Mtion_LSpeed = Conversion_fraction("CH2 SETMOTOR2 LSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd27),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 LSpeed:%.1f mm/s\r\n",CH2_Mtion_LSpeed/10.0);               
            
          }       
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 LDISTANCE ",24) == 0 ) {
            CH2_Mtion_LDistance = Conversion_fraction("CH2 SETMOTOR2 LDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd25),2000);
            EXT1_BSP_Ser_Printf("CH2 SetMotor2 LDistance:%.1f mm\r\n",CH2_Mtion_LDistance/10.0);             
            
          }             
          //CH3，高端
          if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 HSPEED ",21) == 0 ) {//速度
            CH3_Mtion_HSpeed = Conversion_fraction("CH3 SETMOTOR3 HSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd31),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 HSpeed:%.1f mm/s\r\n",CH3_Mtion_HSpeed/10.0);                
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 HDISTANCE ",24) == 0 ) {//位置
            CH3_Mtion_HDistance = Conversion_fraction("CH3 SETMOTOR3 HDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd29),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 HDistance:%.1f mm\r\n",CH3_Mtion_HDistance/10.0);               
            
          }   
          
          //CH3，中端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 MSPEED ",21) == 0 ) {
            CH3_Mtion_MSpeed = Conversion_fraction("CH3 SETMOTOR3 MSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd35),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 MSpeed:%.1f mm/s\r\n",CH3_Mtion_MSpeed/10.0);             
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 MDISTANCE ",24) == 0 ) {
            CH3_Mtion_MDistance = Conversion_fraction("CH3 SETMOTOR3 MDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd33),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 MDistance:%.1f mm\r\n",CH3_Mtion_MDistance/10.0);               
            
          }
          
          //CH3，低端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 LSPEED ",21) == 0 ) {
            CH3_Mtion_LSpeed = Conversion_fraction("CH3 SETMOTOR3 LSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd39),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 LSpeed:%.1f mm/s\r\n",CH3_Mtion_LSpeed/10.0);               
            
          }       
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 LDISTANCE ",24) == 0 ) {
            CH3_Mtion_LDistance = Conversion_fraction("CH3 SETMOTOR3 LDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd37),2000);
            EXT1_BSP_Ser_Printf("CH3 SetMotor3 LDistance:%.1f mm\r\n",CH3_Mtion_LDistance/10.0);             
            
          }
        
          //CH4，高端
          if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 HSPEED ",21) == 0 ) {//速度
            CH4_Mtion_HSpeed = Conversion_fraction("CH4 SETMOTOR4 HSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd43),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 HSpeed:%.1f mm/s\r\n",CH4_Mtion_HSpeed/10.0);               
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 HDISTANCE ",24) == 0 ) {//位置
            CH4_Mtion_HDistance = Conversion_fraction("CH4 SETMOTOR4_HDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd41),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 HDistance:%.1f mm\r\n",CH4_Mtion_HDistance/10.0);  
            
          }   
          
          //CH4，中端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 MSPEED ",21) == 0 ) {
            CH4_Mtion_MSpeed = Conversion_fraction("CH4 SETMOTOR4 MSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd47),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 MSpeed:%.1f mm/s\r\n",CH4_Mtion_MSpeed/10.0);              
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 MDISTANCE ",24) == 0 ) {
            CH4_Mtion_MDistance = Conversion_fraction("CH4 SETMOTOR4 MDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd45),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 MDistance:%.1f mm\r\n",CH4_Mtion_MDistance/10.0);          
          }
          
          //CH4，低端
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 LSPEED ",21) == 0 ) {
            CH4_Mtion_LSpeed = Conversion_fraction("CH4 SETMOTOR4 LSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd51),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 LSpeed:%.1f mm/s\r\n",CH4_Mtion_LSpeed/10.0);          
            
          }       
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 LDISTANCE ",24) == 0 ) {
            
            CH4_Mtion_LDistance = Conversion_fraction("CH4 SETMOTOR4 LDISTANCE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd49),2000);
            EXT1_BSP_Ser_Printf("CH4 SetMotor4 LDistance:%.1f mm\r\n",CH4_Mtion_LDistance/10.0);      
          }
              
          //复位最大行程
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"RESET DISTANCE MAX ",19) == 0 ) {

              Reset_Distance_Max = Conversion_fraction("RESET DISTANCE MAX ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd106),10000);
              EXT1_BSP_Ser_Printf("Reset Distance Max:%.1f mm\r\n",Reset_Distance_Max/10.0);     

          }   

          //CH1复位速度
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETMOTOR1 RSPEED ",21) == 0 ) {

              CH1_Mtion_ResetSpeed = Conversion_fraction("CH1 SETMOTOR1 RSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd108),3000);
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 RSpeed:%.1f mm/s\r\n",CH1_Mtion_ResetSpeed/10.0);     

          }
          //CH2复位速度
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETMOTOR2 RSPEED ",21) == 0 ) {

              CH2_Mtion_ResetSpeed = Conversion_fraction("CH2 SETMOTOR2 RSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd110),3000);
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 RSpeed:%.1f mm/s\r\n",CH2_Mtion_ResetSpeed/10.0);     

          }
          //CH3复位速度
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETMOTOR3 RSPEED ",19) == 0 ) {

              CH3_Mtion_ResetSpeed = Conversion_fraction("CH3 SETMOTOR3 RSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd112),3000);
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 RSpeed:%.1f mm/s\r\n",CH3_Mtion_ResetSpeed/10.0);     

          } 
          //CH4复位速度
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETMOTOR4 RSPEED ",19) == 0 ) {
      
              CH4_Mtion_ResetSpeed = Conversion_fraction("CH4 SETMOTOR4 RSPEED ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd114),3000);
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 RSpeed:%.1f mm/s\r\n",CH4_Mtion_ResetSpeed/10.0);     
      
          }
           
          //Test_Motor
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 TEST MOTOR UP",18) == 0 ) {
            
            CH1_Mtion_ABS_POS_Relative=800;
            Motor1Starting_PWM1(DIR_CCW,200);
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 TEST MOTOR DW",18) == 0 ) {
            
            CH1_Mtion_ABS_POS_Relative=800;
            Motor1Starting_PWM1(DIR_CW,200);
            
          } 
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 TEST MOTOR UP",18) == 0 ) {
            
            CH2_Mtion_ABS_POS_Relative=800;
            Motor2Starting_PWM2(DIR_CCW,200);
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 TEST MOTOR DW",18) == 0 ) {
            
            CH2_Mtion_ABS_POS_Relative=800;
            Motor2Starting_PWM2(DIR_CW,200);
            
          }  
          
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 TEST MOTOR UP",18) == 0 ) {
            
            CH3_Mtion_ABS_POS_Relative=800;
            Motor3Starting_PWM3(DIR_CCW,200);
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 TEST MOTOR DW",18) == 0 ) {
            
            CH3_Mtion_ABS_POS_Relative=800;
            Motor3Starting_PWM3(DIR_CW,200);
            
          }  
          
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 TEST MOTOR UP",18) == 0 ) {
            
            CH4_Mtion_ABS_POS_Relative=800;
            Motor4Starting_PWM4(DIR_CCW,200);
            
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 TEST MOTOR DW",18) == 0 ) {
            
            CH4_Mtion_ABS_POS_Relative=800;
            Motor4Starting_PWM4(DIR_CW,200);
            
          }  
          
          //TEST CY
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 TEST CY IN",14) == 0 ) {
            CH1_Platform_IN();
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 TEST CY OUT",15) == 0 ) {      
            CH1_Platform_OUT();
          }  
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 TEST CY IN",14) == 0 ) {
            CH2_Platform_IN();
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 TEST CY OUT",15) == 0 ) {      
            CH2_Platform_OUT();
          }  
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 TEST CY IN",14) == 0 ) {
            CH3_Platform_IN();
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 TEST CY OUT",15) == 0 ) {      
            CH3_Platform_OUT();
          }  
          
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 TEST CY IN",14) == 0 ) {
            CH4_Platform_IN();
          }
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 TEST CY OUT",15) == 0 ) {      
            CH4_Platform_OUT();
          }  
          
          //复位速度
          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CHECK FIXTURE PARAMETER",23) == 0 ) {
            
              //CH1
              EXT1_BSP_Ser_Printf("--------------马达参数设置-------------------------------\r\n");
              EXT1_BSP_Ser_Printf("--------------通道一-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 HSpeed:%.1f mm/s\r\n",CH1_Mtion_HSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 MSpeed:%.1f mm/s\r\n",CH1_Mtion_MSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 LSpeed:%.1f mm/s\r\n",CH1_Mtion_LSpeed/10.0);
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 HDistance:%.1f mm\r\n",CH1_Mtion_HDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 MDistance:%.1f mm\r\n",CH1_Mtion_MDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 LDistance:%.1f mm\r\n",CH1_Mtion_LDistance/10.0);
              EXT1_BSP_Ser_Printf("CH1 SetMotor1 RSpeed:%.1f mm\r\n",CH1_Mtion_ResetSpeed/10.0); 
              
              EXT1_BSP_Ser_Printf("--------------通道二-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 HSpeed:%.1f mm/s\r\n",CH2_Mtion_HSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 MSpeed:%.1f mm/s\r\n",CH2_Mtion_MSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 LSpeed:%.1f mm/s\r\n",CH2_Mtion_LSpeed/10.0);
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 HDistance:%.1f mm\r\n",CH2_Mtion_HDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 MDistance:%.1f mm\r\n",CH2_Mtion_MDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 LDistance:%.1f mm\r\n",CH2_Mtion_LDistance/10.0); 
              EXT1_BSP_Ser_Printf("CH2 SetMotor2 RSpeed:%.1f mm\r\n",CH2_Mtion_ResetSpeed/10.0); 
              
              EXT1_BSP_Ser_Printf("--------------通道三-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 HSpeed:%.1f mm/s\r\n",CH3_Mtion_HSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 MSpeed:%.1f mm/s\r\n",CH3_Mtion_MSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 LSpeed:%.1f mm/s\r\n",CH3_Mtion_LSpeed/10.0);
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 HDistance:%.1f mm\r\n",CH3_Mtion_HDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 MDistance:%.1f mm\r\n",CH3_Mtion_MDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 LDistance:%.1f mm\r\n",CH3_Mtion_LDistance/10.0); 
              EXT1_BSP_Ser_Printf("CH3 SetMotor3 RSpeed:%.1f mm\r\n",CH3_Mtion_ResetSpeed/10.0);  
              
              EXT1_BSP_Ser_Printf("--------------通道四-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 HSpeed:%.1f mm/s\r\n",CH4_Mtion_HSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 MSpeed:%.1f mm/s\r\n",CH4_Mtion_MSpeed/10.0);   
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 LSpeed:%.1f mm/s\r\n",CH4_Mtion_LSpeed/10.0);
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 HDistance:%.1f mm\r\n",CH4_Mtion_HDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 MDistance:%.1f mm\r\n",CH4_Mtion_MDistance/10.0);   
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 LDistance:%.1f mm\r\n",CH4_Mtion_LDistance/10.0); 
              EXT1_BSP_Ser_Printf("CH4 SetMotor4 RSpeed:%.1f mm\r\n",CH4_Mtion_ResetSpeed/10.0);
              
              //复位距离
              EXT1_BSP_Ser_Printf("Reset Distance Max:%.1f mm\r\n",Reset_Distance_Max/10.0);   
              EXT1_BSP_Ser_Printf("--------------压力传感器参数设置-------------------------\r\n");
              EXT1_BSP_Ser_Printf("--------------通道一-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH1_SetValueAD_Zero:%d \r\n",CH1_SetValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH1_CalValueAD_Zero:%d \r\n",CH1_CalValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH1_SetValueAD_5KG:%d \r\n",CH1_SetValueAD_5KG);
              EXT1_BSP_Ser_Printf("CH1_CalValueAD_5KG:%d \r\n",CH1_CalValueAD_5KG);   
              EXT1_BSP_Ser_Printf("CH1_SetPress_Value:%.1f N\r\n",CH1_SetPress_Value);   
              EXT1_BSP_Ser_Printf("CH1_SetDev_Value:%.1f N\r\n",CH1_SetDev_Value);  
              EXT1_BSP_Ser_Printf("CH1_SetMaxPress_Value:%.1f N\r\n",CH1_SetMaxPress_Value); 
              
              EXT1_BSP_Ser_Printf("--------------通道二-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH2_SetValueAD_Zero:%d \r\n",CH2_SetValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH2_CalValueAD_Zero:%d \r\n",CH2_CalValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH2_SetValueAD_5KG:%d \r\n",CH2_SetValueAD_5KG);
              EXT1_BSP_Ser_Printf("CH2_CalValueAD_5KG:%d \r\n",CH2_CalValueAD_5KG);   
              EXT1_BSP_Ser_Printf("CH2_SetPress_Value:%.1f N\r\n",CH2_SetPress_Value);   
              EXT1_BSP_Ser_Printf("CH2_SetDev_Value:%.1f N\r\n",CH2_SetDev_Value);  
              EXT1_BSP_Ser_Printf("CH2_SetMaxPress_Value:%.1f N\r\n",CH2_SetMaxPress_Value);
              
              EXT1_BSP_Ser_Printf("--------------通道三-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH3_SetValueAD_Zero:%d \r\n",CH3_SetValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH3_CalValueAD_Zero:%d \r\n",CH3_CalValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH3_SetValueAD_5KG:%d \r\n",CH3_SetValueAD_5KG);
              EXT1_BSP_Ser_Printf("CH3_CalValueAD_5KG:%d \r\n",CH3_CalValueAD_5KG);   
              EXT1_BSP_Ser_Printf("CH3_SetPress_Value:%.1f N\r\n",CH3_SetPress_Value);   
              EXT1_BSP_Ser_Printf("CH3_SetDev_Value:%.1f N\r\n",CH3_SetDev_Value); 
              EXT1_BSP_Ser_Printf("CH3_SetMaxPress_Value:%.1f N\r\n",CH3_SetMaxPress_Value);  
              
              EXT1_BSP_Ser_Printf("--------------通道四-------------------------------------\r\n");
              EXT1_BSP_Ser_Printf("CH4_SetValueAD_Zero:%d \r\n",CH4_SetValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH4_CalValueAD_Zero:%d \r\n",CH4_CalValueAD_Zero);   
              EXT1_BSP_Ser_Printf("CH4_SetValueAD_5KG:%d \r\n",CH4_SetValueAD_5KG);
              EXT1_BSP_Ser_Printf("CH4_CalValueAD_5KG:%d \r\n",CH4_CalValueAD_5KG);   
              EXT1_BSP_Ser_Printf("CH4_SetPress_Value:%.1f N\r\n",CH4_SetPress_Value);   
              EXT1_BSP_Ser_Printf("CH4_SetDev_Value:%.1f N\r\n",CH4_SetDev_Value);  
              EXT1_BSP_Ser_Printf("CH4_SetMaxPress_Value:%.1f N\r\n",CH3_SetMaxPress_Value); 
              
              EXT1_BSP_Ser_Printf("\r\n");
              
          }
  
}*/

//压力系数校正处理函数
static void Press_Calibration_handling()
{         
           
          //CH1零点 0kg
//          if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1SET PRE L ",13) == 0 ) {
//            CH1_SetValueAD_Zero = Conversion_fraction("CH1SET PRE L ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd74),1000);
//            CH1_CalValueAD_Zero = ChannelADC_Value[0];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd78),&CH1_CalValueAD_Zero);   
//            EXT1_BSP_Ser_Printf("CH1_SetValueAD_Zero:%d\r\nCH1_CalValueAD_Zero:%d\r\n",CH1_SetValueAD_Zero,ChannelADC_Value[0]); 
//          }
//          //CH1设定满量程 5.0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1SET PRE H ",13) == 0 ) {
//            CH1_SetValueAD_5KG = Conversion_fraction("CH1SET PRE H ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd76),1000); 
//            CH1_CalValueAD_5KG = ChannelADC_Value[0];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd80),&CH1_CalValueAD_5KG); 
//            EXT1_BSP_Ser_Printf("CH1_SetValueAD_5KG:%d\r\nCH1_CalValueAD_5KG:%d\r\n",CH1_SetValueAD_5KG,ChannelADC_Value[0]); 
//          }
//          //CH2零点 0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2SET PRE L ",13) == 0 ) {
//            CH2_SetValueAD_Zero = Conversion_fraction("CH2SET PRE L ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd82),1000);
//            CH2_CalValueAD_Zero = ChannelADC_Value[1];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd86),&CH2_CalValueAD_Zero);   
//            EXT1_BSP_Ser_Printf("CH2_SetValueAD_Zero:%d\r\nCH2_CalValueAD_Zero:%d\r\n",CH2_SetValueAD_Zero,ChannelADC_Value[1]); 
//          }
//          //CH2设定满量程 5.0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2SET PRE H ",13) == 0 ) {
//            CH2_SetValueAD_5KG = Conversion_fraction("CH2SET PRE H ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd84),1000);
//            CH2_CalValueAD_5KG = ChannelADC_Value[1];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd88),&CH2_CalValueAD_5KG); 
//            EXT1_BSP_Ser_Printf("CH2_SetValueAD_5KG:%d\r\nCH2_CalValueAD_5KG:%d\r\n",CH2_SetValueAD_5KG,ChannelADC_Value[1]); 
//          }
//          //CH3零点 0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3SET PRE L ",13) == 0 ) {
//            CH3_SetValueAD_Zero = Conversion_fraction("CH3SET PRE L ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd90),1000);
//            CH3_CalValueAD_Zero = ChannelADC_Value[2];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd94),&CH3_CalValueAD_Zero);   
//            EXT1_BSP_Ser_Printf("CH3_SetValueAD_Zero:%d\r\nCH3_CalValueAD_Zero:%d\r\n",CH3_SetValueAD_Zero,ChannelADC_Value[2]); 
//          }
//          //CH3设定满量程 5.0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3SET PRE H ",13) == 0 ) {
//            CH3_SetValueAD_5KG = Conversion_fraction("CH3SET PRE H ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd92),1000);
//            CH3_CalValueAD_5KG = ChannelADC_Value[2];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd96),&CH3_CalValueAD_5KG); 
//            EXT1_BSP_Ser_Printf("CH3_SetValueAD_5KG:%d\r\nCH3_CalValueAD_5KG:%d\r\n",CH3_SetValueAD_5KG,ChannelADC_Value[2]); 
//          }
//          //CH4零点 0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4SET PRE L ",13) == 0 ) {
//            CH4_SetValueAD_Zero=Conversion_fraction("CH4SET PRE L ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd98),1000);
//            CH4_CalValueAD_Zero=ChannelADC_Value[3];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd102),&CH4_CalValueAD_Zero);   
//            EXT1_BSP_Ser_Printf("CH4_SetValueAD_Zero:%d\r\nCH4_CalValueAD_Zero:%d\r\n",CH4_SetValueAD_Zero,ChannelADC_Value[3]); 
//          }
//          //CH4设定满量程 5.0 kg
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4SET PRE H ",13) == 0 ) {
//            CH4_SetValueAD_5KG = Conversion_fraction("CH4SET PRE H ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd100),1000);
//            CH4_CalValueAD_5KG = ChannelADC_Value[3];
//            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,SRegisAdd104),&CH4_CalValueAD_5KG); 
//            EXT1_BSP_Ser_Printf("CH4_SetValueAD_5KG:%d\r\nCH4_CalValueAD_5KG:%d\r\n",CH4_SetValueAD_5KG,ChannelADC_Value[3]); 
//          }
//          
//          //设定值
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETPRESS VALUE ",19) == 0 ){
//              CH1_SetPress_Value = 0.1*Conversion_fraction("CH1 SETPRESS VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd53),1000);
//              EXT1_BSP_Ser_Printf("CH1 SetPress Value:%.1f N\r\n",CH1_SetPress_Value);           
//          } 
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETPRESS VALUE ",19) == 0 ) {
//              CH2_SetPress_Value = 0.1*Conversion_fraction("CH2 SETPRESS VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd55),1000);
//              EXT1_BSP_Ser_Printf("CH2 SetPress Value:%.1f N\r\n",CH2_SetPress_Value);               
//          }
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETPRESS VALUE ",19) == 0 ) {
//              CH3_SetPress_Value = 0.1*Conversion_fraction("CH3 SETPRESS VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd57),1000);
//              EXT1_BSP_Ser_Printf("CH3 SetPress Value:%.1f N\r\n",CH3_SetPress_Value);  
//          }
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETPRESS VALUE ",19) == 0 ) {
//              CH4_SetPress_Value = 0.1*Conversion_fraction("CH4 SETPRESS VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd59),1000);
//              EXT1_BSP_Ser_Printf("CH4 SetPress Value:%.1f N\r\n",CH4_SetPress_Value);  
//          }
//            
//          //偏差值
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH1 SETDEV VALUE ",16) == 0 ) {
//              CH1_SetDev_Value = 0.1*Conversion_fraction("CH1 SETDEV VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd61),1000);
//              EXT1_BSP_Ser_Printf("CH1 SetDev Value:%.1f N\r\n",CH1_SetDev_Value);  
//          } 
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH2 SETDEV VALUE ",16) == 0 ) {
//              CH2_SetDev_Value = 0.1*Conversion_fraction("CH2 SETDEV VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd63),1000);
//              EXT1_BSP_Ser_Printf("CH2 SetDev Value:%.1f N\r\n",CH2_SetDev_Value);  
//          }
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH3 SETDEV VALUE ",16) == 0 ) {
//              CH3_SetDev_Value = 0.1*Conversion_fraction("CH3 SETDEV VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd65),1000);
//              EXT1_BSP_Ser_Printf("CH3 SetDev Value:%.1f N\r\n",CH3_SetDev_Value);  
//          }
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CH4 SETDEV VALUE ",16) == 0 ) {
//              CH4_SetDev_Value = 0.1*Conversion_fraction("CH4 SETDEV VALUE ",receBuf,OperateAdd(ModbusRegAdd,SRegisAdd67),1000);
//              EXT1_BSP_Ser_Printf("CH4 SetDev Value:%.1f N\r\n",CH4_SetDev_Value);  
//          }    
//          
//          //CHECK PRESS
//          else if(Str_Cmp_N((const CPU_CHAR*)receBuf,(const CPU_CHAR*)"CHECK PRESS VALUE",17) == 0 ) {
//               EXT1_BSP_Ser_Printf("CH1 Current Value:%.1f N\r\n",CH1_CurrentValue); 
//               EXT1_BSP_Ser_Printf("CH2 Current Value:%.1f N\r\n",CH2_CurrentValue);
//               EXT1_BSP_Ser_Printf("CH3 Current Value:%.1f N\r\n",CH3_CurrentValue);
//               EXT1_BSP_Ser_Printf("CH4 Current Value:%.1f N\r\n",CH4_CurrentValue);
//          }  

}

//字符处理函数
uint32_t Conversion_fraction(uint8_t* Target, uint8_t* Source,uint8_t ADDR, uint32_t end_value)
 {
    
      uint8_t Len_Source=0,Len_Target=0;
      int8_t Index=0,Width=0,Decimals[10];
      uint8_t *ptr_Target,*ptr_Source;
      uint32_t current_value=0;
      
      
      ptr_Target=Target;
      ptr_Source=Source;
      
      while(*ptr_Target++!='\0'){Len_Target++;}
      while(*ptr_Source++!='\0'){Len_Source++;}
      
      ptr_Target=Target;
      ptr_Source=Source;
      
      Width=Len_Source-Len_Target;	
      if(Width<=9) {			
        Width=Width-1;		
        Index=Width;
    
        do
        {
              Decimals[Width]=(ptr_Source[Len_Target+Width]-0x30);
              Width--;
            }while(Width>=0);				
            
            for(Width=0;Width<=Index;Width++)
            {
              current_value+=Decimals[Width]*pow(10,Index-Width);
            }
            Width=0;
            
            if(current_value>=end_value) current_value=end_value;	
            
            /**********************************************************************************/						
            if(current_value<=end_value){	
              IIC_Channel=1;									
              IIC_Write_2Byte(ADDR,&current_value);		
            }
            /**********************************************************************************/
        
      }
      return (current_value);	
 }