
#include <bsp_iicbusDrive.h>
//==========================================================================

CPU_INT08U IIC_Channel;    

//==========================================================================


/***************************************************************************
////US��ʱ
;***************************************************************************/
void delay_Nus(CPU_INT16U us)
{
	CPU_INT16U d;

	while(us--)
	{
		d=6;
		while(d--);
	}
}

/***************************************************************************
///MS ��ʱ
;***************************************************************************/
void delay_Nms( CPU_INT16U ms)
{
	CPU_INT16U d;

	while(ms--)
	{
		d=1200;
		while(d--);
	}
}


/***************************************************************************
;** ��������: 	
;** ��������:  	i2c I/O����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void IIC_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;                         //�������
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);         //ʱ��ʹ��
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_10;        //SCL			
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //�������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);		             //PB  
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1|GPIO_Pin_SDA2;   //SDA			
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             //��������
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);		             //PB 						  
}



/***************************************************************************
;** ��������: 	
;** ��������:  	SDA ��Ϊ�����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void SDA_IN(void)  
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //�������
	switch(IIC_Channel){
        case 1:{
              GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1;      //SDA			
              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //��������
              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
              GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
        }break;
        
        case 2:{
              GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA2;      //SDA			
              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //��������
              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
              GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
        }
  }
}
/***************************************************************************
;** ��������: 	
;** ��������:  	SDA ��Ϊ�����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void SDA_OUT(void) 
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //�������
	switch(IIC_Channel){
            case 1:{
                  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1;   //SDA			
                  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
                  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                  GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
            }break;
            case 2:{
                  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA2;   //SDA			
                  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
                  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                  GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
            }
	}
}

 /***************************************************************************
;** ��������: 	
;** ��������:  	i2c start
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:��1�����Ƚ�SDA����Ϊ���
             ��2����SCLΪ�ߵ�ƽ�ڼ䣬SDA���½��صı仯
;***************************************************************************/
void Set_IIC_SDA(void)
{
    switch(IIC_Channel){
        case 1:{
          Set_IIC_SDA1;
        }break;
        case 2:{
          Set_IIC_SDA2;
        }
    }
		
}
void Clr_IIC_SDA(void)
{
    switch(IIC_Channel){
        case 1:{
          Clr_IIC_SDA1;
        }break;
        case 2:{
          Clr_IIC_SDA2;
        }
    }	
	
}
 /***************************************************************************
;** ��������: 	
;** ��������:  	
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:       
;***************************************************************************/
void Clr_IIC_SCL(void)
{
    switch(IIC_Channel){
        case 1:{
          Clr_IIC_SCL1;
        }break;
        case 2:{
          Clr_IIC_SCL2;
        }
    }
}

void Set_IIC_SCL(void)
{
    switch(IIC_Channel){
        case 1:{
          Set_IIC_SCL1;
        }break;
        case 2:{
          Set_IIC_SCL2;
        }
    }
}

CPU_INT08U READ_SDA(void)
{
  CPU_INT08U flag=0;
      switch(IIC_Channel){
          case 1:{
            flag=READ_SDA1;
          }break;
          case 2:{
            flag=READ_SDA2;
          }break;
  }
  return flag;
}
 /***************************************************************************
;** ��������: 	
;** ��������:  	i2c start
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:��1�����Ƚ�SDA����Ϊ���
             ��2����SCLΪ�ߵ�ƽ�ڼ䣬SDA���½��صı仯
;***************************************************************************/
void IIC_Start(void)
{
	 SDA_OUT();     //sda�����
	 Set_IIC_SDA();      
	 Set_IIC_SCL();
	 delay_Nus(5);
	 Clr_IIC_SDA();  //START:when CLK is high,DATA change form high to low 
	 delay_Nus(5);
	 Clr_IIC_SCL();  //ǯסI2C���ߣ�׼�����ͻ�������� 
} 
 /***************************************************************************
;** ��������: 	
;** ��������:  	i2c Stop
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
  ��1�����Ƚ�SDA����Ϊ���
  ��2����SCLΪ�ߵ�ƽ�ڼ䣬SDA�������صı仯
;***************************************************************************/
void IIC_Stop(void)
{
	 SDA_OUT();//sda�����

	 Clr_IIC_SDA();//STOP:when CLK is high DATA change form low to high
	 Set_IIC_SCL();	 
	 delay_Nus(5);
	 Set_IIC_SDA();//����I2C���߽����ź�
	 delay_Nus(5);  
	 Set_IIC_SCL(); 	          
}
 /***************************************************************************
;** ��������: 	
;** ��������:  	recelve_ ck
;** �������: 
;** �� �� ֵ: 	1������Ӧ��ʧ�ܣ�0������Ӧ��ɹ�
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
  ��1�����Ƚ�SDA����Ϊ����
  ��2���������0����ʾ����Ӧ��ɹ������Ϊ1������Ӧ��ʧ��
  ��3��ע����IIC_STOP()�� ,��SDA��Ϊ�����������������250�����󣬲���IICֹͣ�ߺţ�
    �������SDA��SDA����ֵ��Ϊ0�ģ����Բ��˳�ѭ����
  ��4���˺���ΪSTM32�����ȴ��ӻ���ACK����
;***************************************************************************/
 CPU_INT08U IIC_Recelve_Ack(void)
{
	 unsigned int ucErrTime=0;
	
	 SDA_IN();      //SDA����Ϊ����  
	 Set_IIC_SDA();
	 Clr_IIC_SCL();
	 delay_Nus(5);    
	 Set_IIC_SCL();
	 delay_Nus(5);  
	 while(READ_SDA())  //6T=1us
	 {
              ucErrTime++;
              if(ucErrTime>=6000)   //600/6=100uS
              {
                   Clr_IIC_SCL();//ʱ�����0
                   return 1;
              }
	 }
	 Clr_IIC_SCL();//ʱ�����0     
	 return 0;  
} 
/***************************************************************************
;** ��������: 	
;** ��������:  	ack
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
  ��1�����Ƚ�SDA����Ϊ���
  ��2����SCL��һ��ʱ�������ڼ䣬SDAΪ�� 
  ��3���˺���ΪSTM32������ӻ�����ACK����
;***************************************************************************/
void IIC_Ack(void)
{
	 Clr_IIC_SCL();
	 SDA_OUT();
	 Clr_IIC_SDA();
	 delay_Nus(5);
	 Set_IIC_SCL();
	 delay_Nus(5);
	 Clr_IIC_SCL();
}
/***************************************************************************
;** ��������: 	void IIC_Nack(void)
;** ��������:  	Nack  ������ACKӦ��	
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void IIC_Nack(void)
{
	 Clr_IIC_SCL();
	 SDA_OUT();
	 Set_IIC_SDA();
	 delay_Nus(5);
	 Set_IIC_SCL();
	 delay_Nus(5);
	 Clr_IIC_SCL();
}
/***************************************************************************
;** ��������: 	
;** ��������:  	WRITE BYTE
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
  ��1�����Ƚ�SDA����Ϊ���
  ��2����SCLΪ�͵�ƽ�ڼ䣬��λSDA������Ϊ0����1����ʱ������SCL,ȡ�����ݣ�ѭ��8��
;***************************************************************************/
void IIC_WRITE_BYTE(CPU_INT08U txd)
{
    CPU_INT08U  t;  
	 
    SDA_OUT();      
    Clr_IIC_SCL();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              	        
	  if(((txd&0x80)>>7)==1)  
	  {Set_IIC_SDA();}
	  else 
	  {Clr_IIC_SDA();}
	  txd<<=1;    
	  delay_Nus(5);   
	  Set_IIC_SCL();
	  delay_Nus(5); 
	  Clr_IIC_SCL(); 
	  delay_Nus(5);
    }  
}
/***************************************************************************
;** ��������: 	
;** ��������:  	Read Byte
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
  ��1�����Ƚ�SDA����Ϊ����
  ��2����SCL��һ��ʱ�����ں󣬶�SDA�����ݣ�������receive��
  ��3����1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  
;***************************************************************************/
CPU_INT08U IIC_Read_Byte(CPU_INT08U ack)
{
	CPU_INT08U i,receive=0;

	SDA_IN();//SDA����Ϊ����
	for(i=0;i<8;i++ )
	{
	  Clr_IIC_SCL(); 
	  delay_Nus(5);
	  Set_IIC_SCL();
	  delay_Nus(5);
	  receive<<=1;
	  if(READ_SDA()==1)
	  {receive|=0x01;}   
	  delay_Nus(5); 
	}  
    if (!ack)
	{IIC_Nack();}//����NaCK    	
    else
    {IIC_Ack();} //����ACK       
	return receive;
}

/***************************************************************************
;** ��������: 	
;** ��������:  	д����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	 AT24C016 ��ַ��0--32768 
;***************************************************************************/
/***************************************************************************
;** ��������: 	
;** ��������:  	д����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	 AT24C016 ��ַ��0--32768 
;***************************************************************************/
void IIC_Write_Data(CPU_INT16U RomAddress,CPU_INT08U data)
{
	CPU_INT08U flag=0;
	
	do
	{
              IIC_Start();
              switch(IIC_Channel)
              {
                    case 1:IIC_WRITE_BYTE(WriteDeviceIIC);break; //д����   
                    case 2:IIC_WRITE_BYTE(WriteDeviceIIC);  
              }
              
              if(IIC_Recelve_Ack()==0){  //�ж�ACK      
                
                    IIC_WRITE_BYTE((CPU_INT16U)RomAddress>>8);	  //дEEPRAM H��ַ   
                    
                    if(IIC_Recelve_Ack()==0){  //�ж�ACK  
                      
                          IIC_WRITE_BYTE((CPU_INT16U)RomAddress);	  //дEEPRAM L��ַ
                          if(IIC_Recelve_Ack()==0){      //�ж�ACK
                              IIC_WRITE_BYTE(data);
                              if(IIC_Recelve_Ack()==0){  //�ж�ACK
                                  flag=0;
                              }else{                             
                                  flag=1;
                              }
                          }else{
                              flag=1;
                          }
                          
                          
                    }else{                
                              flag=1;
                    }			
              }else{	             
                    flag=1;
              }
          
        }while(flag==1);
        
	IIC_Stop();      //ֹͣ
//	delay_Nms(1);
//	OSTimeDlyHMSM(0, 0, 0, 10);	
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);

}
/***************************************************************************
;** ��������: 	
;** ��������:  	������
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	 AT24C256 ��ַ��0--32768
;***************************************************************************/
CPU_INT08U IIC_Read_Data(CPU_INT16U RomAddress)
{
	CPU_INT08U flag,data=0;

	do
	{
		  IIC_Start();
			switch(IIC_Channel)
			{
				case 1:IIC_WRITE_BYTE(WriteDeviceIIC);break;	 //д����   
				case 2:IIC_WRITE_BYTE(WriteDeviceIIC);  
			}
			
			if(IIC_Recelve_Ack()==0)            //�ж�ACK
			{
				IIC_WRITE_BYTE((CPU_INT16U)RomAddress>>8);		//дEEPRAM H��ַ 
                                
				if(IIC_Recelve_Ack()==0)    //�ж�ACK
				{
                                        IIC_WRITE_BYTE((CPU_INT16U)RomAddress);		//дEEPRAM L��ַ 
                                        if(IIC_Recelve_Ack()==0)
                                        {
                                              IIC_Start();  
                                              
                                              switch(IIC_Channel)
                                              {
                                                      case 1:IIC_WRITE_BYTE(ReadDeviceIIC);break;	 //������
                                                      case 2:IIC_WRITE_BYTE(ReadDeviceIIC);              //������
                                              }					
                                              
                                              if(IIC_Recelve_Ack()==0){         //�ж�ACK                                            
                                                      data=IIC_Read_Byte(0);
                                                      flag=0;				
                                              }else{                                        
                                                      flag=1;
                                              }
                                              
                                        }else{
                                              flag=0;                                    
                                        }
				}else{
                                        flag=1;
                                } 	
			}else{
			        flag=1;
                        }

	}while(flag==1);
	IIC_Stop();   //ֹͣ
	return data;
}
/***************************************************************************
;** ��������: 	
;** ��������:  	д1byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	   ��λ��ǰ����λ�ں�
;***************************************************************************/
void IIC_Write_1Byte(CPU_INT16U RomAddress, void *udata)
{
        IIC_Write_Data(RomAddress,*((CPU_INT08U *)udata)); 
}

/***************************************************************************
;** ��������: 	
;** ��������:  	��1byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void IIC_Read_1Byte(CPU_INT16U RomAddress,void *udata)
{
	CPU_INT08U BufData;
	BufData=IIC_Read_Data(RomAddress); 
        *((CPU_INT08U *)udata) = BufData;	
}


/***************************************************************************
;** ��������: 	
;** ��������:  	д2byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	   ��λ��ǰ����λ�ں�
;***************************************************************************/
void IIC_Write_2Byte(CPU_INT16U RomAddress, void *udata)
{
	CPU_INT08S i;
		
	for(i=1;i>=0;i--)
	{
	  IIC_Write_Data(RomAddress,*((CPU_INT08U *)udata+i)); 
	  RomAddress++; 
	}  
}
/***************************************************************************
;** ��������: 	
;** ��������:  	��2byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void IIC_Read_2Byte(CPU_INT16U RomAddress,void *udata)
{
	CPU_INT08S BufData,i;
		
	for(i=1;i>=0;i--)
	{
	  BufData=IIC_Read_Data(RomAddress); 
	  RomAddress++; 
	  *((CPU_INT08U *)udata+i)|=BufData;
	}
}
/***************************************************************************
;** ��������: 	
;** ��������:  	д4byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	   ��λ��ǰ����λ�ں�
;***************************************************************************/
void IIC_Write_4Byte(CPU_INT16U RomAddress,void *udata)
{
	CPU_INT08S i;

	for(i=3;i>=0;i--)
	{
	  IIC_Write_Data(RomAddress,*((CPU_INT08U *)udata+i)); 
	  RomAddress++; 
	}  
	
}
/***************************************************************************
;** ��������: 	
;** ��������:  	��4byte����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void IIC_Read_4Byte(CPU_INT16U RomAddress,void *udata)
{
	
            CPU_INT08U BufData;	  
	    CPU_INT08S  i;	
			for(i=3;i>=0;i--)
			{
				BufData=IIC_Read_Data(RomAddress); 
				RomAddress++;
				*((CPU_INT08U*)(udata)+i)=BufData;
			}
			
}
/***************************************************************************
;** ��������: 	
;** ��������:  	дdouble����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	doubleΪ 8 byte ,
;***************************************************************************/
void IIC_Write_double(CPU_INT16U RomAddress,double udata)
{
	CPU_INT08U i;
	void *p;

	p=&udata;	
	for(i=0;i<sizeof(double);i++)
	{IIC_Write_Data(RomAddress+i,*((char *)p+i));}
  
}
/***************************************************************************
;** ��������: 	
;** ��������:  	��double����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	doubleΪ 8 byte ,
;***************************************************************************/
double IIC_Read_double(CPU_INT16U RomAddress)
{
	CPU_INT08U i;
	double data=0.0;
	void *p;

	p=&data;
	for(i=0;i<sizeof(double);i++)
	{*((char *)p+i)=IIC_Read_Data(RomAddress+i);}	
	return data;
}
/***************************************************************************
;** ��������: 	
;** ��������:  	д��������ģ��
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	 ��λ��ǰ����λ�ں�
;***************************************************************************/
void IIC_Write_Nbyte(CPU_INT08U *pc,CPU_INT16U Addr,CPU_INT08U number) 
{
	CPU_INT08U i;		
    
	for(i=0;i<=number;i++)
	{
		IIC_Write_Data(Addr,pc[i]);
		Addr++;
	}
}
/***************************************************************************
;** ��������: 	
;** ��������:  	�������ַ�ģ��
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   Qinyi
;** �ա�  ��:  2012-6-29
;** �޸�ԭ��
;** ˵    ��:	 AT24C256 ��ַ��0--32768 
;***************************************************************************/
void IIC_Read_Nbyte(CPU_INT08U *pc,CPU_INT16U Addr,CPU_INT08U number) 
{
	CPU_INT08U i;
	
	for(i=0;i<number;i++)
	{
		pc[i]=IIC_Read_Data(Addr);
		Addr++;
	}		
} 
