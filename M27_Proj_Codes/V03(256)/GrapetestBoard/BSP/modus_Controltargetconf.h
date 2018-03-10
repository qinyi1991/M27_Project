/***********************����appȫ�ֱ���***************************************************************************/
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

#define    SRegisAdd29      29  //�������������ֵ
#define    SRegisAdd30      30 	//

#define    SRegisAdd33      33  //��ѹ������ֵ
#define    SRegisAdd34      34 	//

#define    SRegisAdd37      37  //�Ҳ����������ֵ
#define    SRegisAdd38      38	//

#define    SRegisAdd41      41  //�������������ֵ
#define    SRegisAdd42      42	//

#define    SRegisAdd45      45  //��ѹ������ֵ
#define    SRegisAdd46      46 	//

#define    SRegisAdd49      49  //������������ֵ
#define    SRegisAdd50      50 	//

#define    SRegisAdd53      53 	//����ʱ��

#define    SRegisAdd55      55 	//�����������ͣ��ʱ��

#define    SRegisAdd57      57 	//�����ɵȴ�ʱ��
  
#define    SRegisAdd59      59  //�����Զ��ٶ�

#define    SRegisAdd61      61  //�ҵ���Զ��ٶ�

#define    SRegisAdd63      63 //�����ٶ�

#define    SRegisAdd65      65 //�Ҳ���ٶ�

#define    SRegisAdd67      67 //runtime

#define    SRegisAdd69      69  //����
#define    SRegisAdd70      70 	

#define    SRegisAdd75      75
#define    SRegisAdd76      76


#define    SRegisAdd79      79
#define    SRegisAdd80      80

#define    SRegisAdd83      83

#define    SRegisAdd200     200  //�¿�����Ϣ

/*---------------------- coils  address ----------------------------------*/
//coil 1--400 write in eepram 400--800 not save
//������Ȧ
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
//-----------������״̬��ַ----------------
//401--800 write in eepram 400--800 not save
//��̬��Ȧ
#define    NCoilAdd401       401      //Ԥ��
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

//������԰�ť

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


//״̬��־ָʾ��
//#define    NCoilAdd440        440    //��λ�С�
//#define    NCoilAdd441        441    //�����С�
//#define    NCoilAdd442        442    //1��
//#define    NCoilAdd443        443    //2��
//#define    NCoilAdd444        444    //3��
//#define    NCoilAdd445        445    //�����롭
//#define    NCoilAdd446        446    //�������
//#define    NCoilAdd447        447    //�豸δ��λ
//#define    NCoilAdd448        448    //�Զ�������
//#define    NCoilAdd449        449    //���λ���쳣
//#define    NCoilAdd450        450    //����쳣
//#define    NCoilAdd451        451    //������
//#define    NCoilAdd452        452    //�����¶�δ���趨ֵ
//#define    NCoilAdd453        453    //����˿�Ͽ���Դ
//#define    NCoilAdd454        454    //��ѹ�쳣
//#define    NCoilAdd455        455    //�����쳣
//#define    NCoilAdd456        456    //��Ʒ�Ƿ���λ
//#define    NCoilAdd457        457    //��ͣ����
//
//
//
////д�뱣�ֵ�ַȷ�ϰ�ť
//#define    NCoilAdd458       458     //�ָ���������
//#define    NCoilAdd459       459     //���ǰ����F��
//#define    NCoilAdd460       460     //���ǰ����B��
//#define    NCoilAdd461       461     //��λ
//#define    NCoilAdd462       462     //���
//#define    NCoilAdd463       463     //�Զ�ѭ��
//#define    NCoilAdd464       464     //���α���SPK
//
//#define    ModbusAdd(n)    n*2-1  




#endif