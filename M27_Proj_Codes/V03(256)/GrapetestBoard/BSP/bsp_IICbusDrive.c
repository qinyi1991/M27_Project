
#include <bsp_iicbusDrive.h>
//==========================================================================

CPU_INT08U IIC_Channel;    

//==========================================================================


/***************************************************************************
////US延时
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
///MS 延时
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
;** 函数名称: 	
;** 功能描述:  	i2c I/O配置
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
;***************************************************************************/
void IIC_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;                         //定义变量
        
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);         //时钟使能
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_10;        //SCL			
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //推挽输出
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);		             //PB  
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1|GPIO_Pin_SDA2;   //SDA			
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             //上拉输入
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);		             //PB 						  
}



/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	SDA 做为输入口
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
;***************************************************************************/
void SDA_IN(void)  
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //定义变量
	switch(IIC_Channel){
        case 1:{
              GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1;      //SDA			
              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //上拉输入
              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
              GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
        }break;
        
        case 2:{
              GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA2;      //SDA			
              GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	   //上拉输入
              GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
              GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
        }
  }
}
/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	SDA 做为输出口
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
;***************************************************************************/
void SDA_OUT(void) 
{	
	GPIO_InitTypeDef GPIO_InitStructure;  //定义变量
	switch(IIC_Channel){
            case 1:{
                  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA1;   //SDA			
                  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
                  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                  GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
            }break;
            case 2:{
                  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SDA2;   //SDA			
                  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
                  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
                  GPIO_Init(GPIOB, &GPIO_InitStructure);		 //PB 
            }
	}
}

 /***************************************************************************
;** 函数名称: 	
;** 功能描述:  	i2c start
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:（1）首先将SDA配置为输出
             （2）在SCL为高电平期间，SDA有下降沿的变化
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
;** 函数名称: 	
;** 功能描述:  	
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:       
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
;** 函数名称: 	
;** 功能描述:  	i2c start
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:（1）首先将SDA配置为输出
             （2）在SCL为高电平期间，SDA有下降沿的变化
;***************************************************************************/
void IIC_Start(void)
{
	 SDA_OUT();     //sda线输出
	 Set_IIC_SDA();      
	 Set_IIC_SCL();
	 delay_Nus(5);
	 Clr_IIC_SDA();  //START:when CLK is high,DATA change form high to low 
	 delay_Nus(5);
	 Clr_IIC_SCL();  //钳住I2C总线，准备发送或接收数据 
} 
 /***************************************************************************
;** 函数名称: 	
;** 功能描述:  	i2c Stop
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
  （1）首先将SDA配置为输出
  （2）在SCL为高电平期间，SDA有上升沿的变化
;***************************************************************************/
void IIC_Stop(void)
{
	 SDA_OUT();//sda线输出

	 Clr_IIC_SDA();//STOP:when CLK is high DATA change form low to high
	 Set_IIC_SCL();	 
	 delay_Nus(5);
	 Set_IIC_SDA();//发送I2C总线结束信号
	 delay_Nus(5);  
	 Set_IIC_SCL(); 	          
}
 /***************************************************************************
;** 函数名称: 	
;** 功能描述:  	recelve_ ck
;** 输入参数: 
;** 返 回 值: 	1，接收应答失败；0，接收应答成功
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
  （1）首先将SDA配置为输入
  （2）如果返回0，表示接收应答成功；如果为1，接收应答失败
  （3）注意在IIC_STOP()中 ,将SDA置为了输出，所以再满足250条件后，产中IIC停止线号，
    对输出线SDA读SDA输入值是为0的，所以才退出循环。
  （4）此函数为STM32主机等待从机的ACK函数
;***************************************************************************/
 CPU_INT08U IIC_Recelve_Ack(void)
{
	 unsigned int ucErrTime=0;
	
	 SDA_IN();      //SDA设置为输入  
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
                   Clr_IIC_SCL();//时钟输出0
                   return 1;
              }
	 }
	 Clr_IIC_SCL();//时钟输出0     
	 return 0;  
} 
/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	ack
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
  （1）首先将SDA配置为输出
  （2）在SCL的一个时钟周期期间，SDA为低 
  （3）此函数为STM32主机向从机发送ACK函数
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
;** 函数名称: 	void IIC_Nack(void)
;** 功能描述:  	Nack  不产生ACK应答	
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
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
;** 函数名称: 	
;** 功能描述:  	WRITE BYTE
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
  （1）首先将SDA配置为输出
  （2）在SCL为低电平期间，置位SDA的数据为0或者1，延时后拉高SCL,取走数据，循环8次
;***************************************************************************/
void IIC_WRITE_BYTE(CPU_INT08U txd)
{
    CPU_INT08U  t;  
	 
    SDA_OUT();      
    Clr_IIC_SCL();//拉低时钟开始数据传输
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
;** 函数名称: 	
;** 功能描述:  	Read Byte
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
  （1）首先将SDA配置为输入
  （2）在SCL的一个时钟周期后，读SDA的数据，保存在receive中
  （3）读1个字节，ack=1时，发送ACK，ack=0，发送nACK  
;***************************************************************************/
CPU_INT08U IIC_Read_Byte(CPU_INT08U ack)
{
	CPU_INT08U i,receive=0;

	SDA_IN();//SDA设置为输入
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
	{IIC_Nack();}//发送NaCK    	
    else
    {IIC_Ack();} //发送ACK       
	return receive;
}

/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	写数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	 AT24C016 地址：0--32768 
;***************************************************************************/
/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	写数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	 AT24C016 地址：0--32768 
;***************************************************************************/
void IIC_Write_Data(CPU_INT16U RomAddress,CPU_INT08U data)
{
	CPU_INT08U flag=0;
	
	do
	{
              IIC_Start();
              switch(IIC_Channel)
              {
                    case 1:IIC_WRITE_BYTE(WriteDeviceIIC);break; //写操作   
                    case 2:IIC_WRITE_BYTE(WriteDeviceIIC);  
              }
              
              if(IIC_Recelve_Ack()==0){  //判断ACK      
                
                    IIC_WRITE_BYTE((CPU_INT16U)RomAddress>>8);	  //写EEPRAM H地址   
                    
                    if(IIC_Recelve_Ack()==0){  //判断ACK  
                      
                          IIC_WRITE_BYTE((CPU_INT16U)RomAddress);	  //写EEPRAM L地址
                          if(IIC_Recelve_Ack()==0){      //判断ACK
                              IIC_WRITE_BYTE(data);
                              if(IIC_Recelve_Ack()==0){  //判断ACK
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
        
	IIC_Stop();      //停止
//	delay_Nms(1);
//	OSTimeDlyHMSM(0, 0, 0, 10);	
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);
	delay_Nus(250);

}
/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	读数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	 AT24C256 地址：0--32768
;***************************************************************************/
CPU_INT08U IIC_Read_Data(CPU_INT16U RomAddress)
{
	CPU_INT08U flag,data=0;

	do
	{
		  IIC_Start();
			switch(IIC_Channel)
			{
				case 1:IIC_WRITE_BYTE(WriteDeviceIIC);break;	 //写操作   
				case 2:IIC_WRITE_BYTE(WriteDeviceIIC);  
			}
			
			if(IIC_Recelve_Ack()==0)            //判断ACK
			{
				IIC_WRITE_BYTE((CPU_INT16U)RomAddress>>8);		//写EEPRAM H地址 
                                
				if(IIC_Recelve_Ack()==0)    //判断ACK
				{
                                        IIC_WRITE_BYTE((CPU_INT16U)RomAddress);		//写EEPRAM L地址 
                                        if(IIC_Recelve_Ack()==0)
                                        {
                                              IIC_Start();  
                                              
                                              switch(IIC_Channel)
                                              {
                                                      case 1:IIC_WRITE_BYTE(ReadDeviceIIC);break;	 //读操作
                                                      case 2:IIC_WRITE_BYTE(ReadDeviceIIC);              //读操作
                                              }					
                                              
                                              if(IIC_Recelve_Ack()==0){         //判断ACK                                            
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
	IIC_Stop();   //停止
	return data;
}
/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	写1byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	   高位在前，低位在后
;***************************************************************************/
void IIC_Write_1Byte(CPU_INT16U RomAddress, void *udata)
{
        IIC_Write_Data(RomAddress,*((CPU_INT08U *)udata)); 
}

/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	读1byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
;***************************************************************************/
void IIC_Read_1Byte(CPU_INT16U RomAddress,void *udata)
{
	CPU_INT08U BufData;
	BufData=IIC_Read_Data(RomAddress); 
        *((CPU_INT08U *)udata) = BufData;	
}


/***************************************************************************
;** 函数名称: 	
;** 功能描述:  	写2byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	   高位在前，低位在后
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
;** 函数名称: 	
;** 功能描述:  	读2byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
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
;** 函数名称: 	
;** 功能描述:  	写4byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	   高位在前，低位在后
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
;** 函数名称: 	
;** 功能描述:  	读4byte数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:
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
;** 函数名称: 	
;** 功能描述:  	写double数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	double为 8 byte ,
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
;** 函数名称: 	
;** 功能描述:  	读double数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	double为 8 byte ,
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
;** 函数名称: 	
;** 功能描述:  	写连续数据模块
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	 高位在前，低位在后
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
;** 函数名称: 	
;** 功能描述:  	连续读字符模块
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   Qinyi
;** 日　  期:  2012-6-29
;** 修改原因：
;** 说    明:	 AT24C256 地址：0--32768 
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
