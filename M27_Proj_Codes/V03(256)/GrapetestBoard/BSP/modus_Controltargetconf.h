/***********************定义app全局变量***************************************************************************/
#ifndef __MODUS_CONTROLTARGETCONF_H
#define __MODUS_CONTROLTARGETCONF_H

/*
*********************************************************************************************************
*                                               MODBUS DEFINE
*********************************************************************************************************
*/

/*----------------------  register address ----------------------------------*/   
#define    SRegisAdd1       1 	//L SPEED
#define    SRegisAdd3       3	//R SPEED
#define    SRegisAdd5       5   //L Distance
#define    SRegisAdd6       6   //
#define    SRegisAdd7       7   //R Distance
#define    SRegisAdd8       8   //

#define    SRegisAdd9       9   //L Pressure
#define    SRegisAdd10      10  //L Pressure

#define    SRegisAdd11      11 	//R Pressure
#define    SRegisAdd12      12 	//R Pressure

#define    SRegisAdd13      13 	//L location1 
#define    SRegisAdd14      14	//

#define    SRegisAdd17      17 	//L location2
#define    SRegisAdd18      18 	// 

#define    SRegisAdd21      21	//R location1  
#define    SRegisAdd22      22	//

#define    SRegisAdd25      25 //R location2 
#define    SRegisAdd26      26 	//

#define    SRegisAdd29      29  //右最大拉力报警值
#define    SRegisAdd30      30 	//

#define    SRegisAdd33      33  //右压力报警值
#define    SRegisAdd34      34 	//

#define    SRegisAdd37      37  //右拆机拉力报警值
#define    SRegisAdd38      38	//

#define    SRegisAdd41      41  //左最大拉力报警值
#define    SRegisAdd42      42	//

#define    SRegisAdd45      45  //左压力报警值
#define    SRegisAdd46      46 	//

#define    SRegisAdd49      49  //左拆机拉力报警值
#define    SRegisAdd50      50 	//

#define    SRegisAdd53      53 	//加热时间

#define    SRegisAdd55      55 	//拆机拉力报警停机时间

#define    SRegisAdd57      57 	//拆机完成等待时间
  
#define    SRegisAdd59      59  //左电机自动速度

#define    SRegisAdd61      61  //右电机自动速度

#define    SRegisAdd63      63 //左拆机速度

#define    SRegisAdd65      65 //右拆机速度

#define    SRegisAdd67      67 //runtime

#define    SRegisAdd69      69  //产量
#define    SRegisAdd70      70 	

#define    SRegisAdd75      75
#define    SRegisAdd76      76


#define    SRegisAdd79      79
#define    SRegisAdd80      80

#define    SRegisAdd83      83

#define    SRegisAdd200     200  //温控器信息

/*---------------------- coils  address ----------------------------------*/
//coil 1--400 write in eepram 400--800 not save
//保持线圈
#define    SCoilAdd1        1   
#define    SCoilAdd2        2  
#define    SCoilAdd3        3   
#define    SCoilAdd4        4   
#define    SCoilAdd5        5   
#define    SCoilAdd6        6  
#define    SCoilAdd7        7   
#define    SCoilAdd8        8   
#define    SCoilAdd9        9   
#define    SCoilAdd10       10 
#define    SCoilAdd11       11   
#define    SCoilAdd12       12   
#define    SCoilAdd13       13
#define    SCoilAdd14       14   
#define    SCoilAdd15       15 
#define    SCoilAdd16       16   
#define    SCoilAdd17       17   
#define    SCoilAdd18       18  
#define    SCoilAdd19       19
#define    SCoilAdd20       20  
#define    SCoilAdd21       21   
#define    SCoilAdd22       22
#define    SCoilAdd23       23  
#define    SCoilAdd24       24
//-----------传感器状态地址----------------
//401--800 write in eepram 400--800 not save
//动态线圈
#define    NCoilAdd401       401      //预留
#define    NCoilAdd402       402      //1# 
#define    NCoilAdd403       403      //2# 
#define    NCoilAdd404       404      //3#
#define    NCoilAdd405       405      //4# 
#define    NCoilAdd406       406      //5# 
#define    NCoilAdd407       407      //6# 
#define    NCoilAdd408       408      //7# 
#define    NCoilAdd409       409      //8# 

#define    NCoilAdd410       410      //9# 
#define    NCoilAdd411       411      //10# 
#define    NCoilAdd412       412      //11# 
#define    NCoilAdd413       413      //12# 
#define    NCoilAdd414       414      //13# 
#define    NCoilAdd415       415      //14# 
#define    NCoilAdd416       416      //15# 
#define    NCoilAdd417       417      //16# 

#define    NCoilAdd418       418      //17#    
#define    NCoilAdd419       419      //18#  
#define    NCoilAdd420       420      //19#    
#define    NCoilAdd421       421      //20#   
#define    NCoilAdd422       422      //21#    
#define    NCoilAdd423       423      //22#  
#define    NCoilAdd424       424      //23# 
#define    NCoilAdd425       425      //24# 

#define    NCoilAdd426       426      //25# 
#define    NCoilAdd427       427      //26#    
#define    NCoilAdd428       428      //27#  
#define    NCoilAdd429       429      //28#    
#define    NCoilAdd430       430      //29#   
#define    NCoilAdd431       431      //30#    
#define    NCoilAdd432       432      //31# 
#define    NCoilAdd433       433      //32# 

//输出测试按钮

#define    NCoilAdd434      434  //1# 
#define    NCoilAdd435      435  //2#     
#define    NCoilAdd436      436  //3#    
#define    NCoilAdd437      437  //4#    
#define    NCoilAdd438      438  //5#    
#define    NCoilAdd439      439  //6#    
#define    NCoilAdd440      440  //7#    
#define    NCoilAdd441      441  //8#  

#define    NCoilAdd442      442  //9#    
#define    NCoilAdd443      443  //10#    
#define    NCoilAdd444      444  //11#  
#define    NCoilAdd445      445  //12#  
#define    NCoilAdd446      446  //13#    
#define    NCoilAdd447      447  //14#    
#define    NCoilAdd448      448  //15#    
#define    NCoilAdd449      449  //16# 

#define    NCoilAdd450      450  //17#  
#define    NCoilAdd451      451  //18#    
#define    NCoilAdd452      452  //19#    
#define    NCoilAdd453      453  //20#    
#define    NCoilAdd454      454  //21#  
#define    NCoilAdd455      455  //22# 
#define    NCoilAdd456      456  //23#    
#define    NCoilAdd457      457  //24# 

#define    NCoilAdd458      458  //25#    
#define    NCoilAdd459      459  //26#  
#define    NCoilAdd460      460  //27#  
#define    NCoilAdd461      461  //28#    
#define    NCoilAdd462      462  //29#    
#define    NCoilAdd463      463  //30#    
#define    NCoilAdd464      464  //31#  
#define    NCoilAdd465      465  //32# 


//状态标志指示灯
//#define    NCoilAdd440        440    //归位中…
//#define    NCoilAdd441        441    //剥离中…
//#define    NCoilAdd442        442    //1段
//#define    NCoilAdd443        443    //2段
//#define    NCoilAdd444        444    //3段
//#define    NCoilAdd445        445    //反剥离…
//#define    NCoilAdd446        446    //剥离完毕
//#define    NCoilAdd447        447    //设备未复位
//#define    NCoilAdd448        448    //自动运行中
//#define    NCoilAdd449        449    //电机位置异常
//#define    NCoilAdd450        450    //电机异常
//#define    NCoilAdd451        451    //返程中
//#define    NCoilAdd452        452    //工作温度未到设定值
//#define    NCoilAdd453        453    //加热丝断开电源
//#define    NCoilAdd454        454    //负压异常
//#define    NCoilAdd455        455    //超温异常
//#define    NCoilAdd456        456    //产品是否在位
//#define    NCoilAdd457        457    //急停按下
//
//
//
////写入保持地址确认按钮
//#define    NCoilAdd458       458     //恢复出厂设置
//#define    NCoilAdd459       459     //电机前进（F）
//#define    NCoilAdd460       460     //电机前进（B）
//#define    NCoilAdd461       461     //复位
//#define    NCoilAdd462       462     //真空
//#define    NCoilAdd463       463     //自动循环
//#define    NCoilAdd464       464     //屏蔽报警SPK
//
//#define    ModbusAdd(n)    n*2-1  




#endif