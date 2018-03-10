#include <modbus.h>




/*======================================================================================================*/
CPU_INT08U	localAddr =1;	     //单片机控制板的抵
CPU_INT08U	receCount;	     //接收到的字节个
CPU_INT08U      sendBuf[100],receBuf[100]; //发送接收缓冲
CPU_INT08U      redrxBuf[20];
CPU_INT08U	redrxCount;	     //接收到的字节个
CPU_INT08U	checkoutError;	     // ==2 偶校验错  
CPU_INT08U	receTimeOut;         //接收超时
/*======================================================================================================*/

Coil_Dis   Coil;
Regis_Dis  Rdis;

/*======================================================================================================*/
CPU_INT08U   CoilData[800];        //modbus 线圈状态   1--400 存入EEPRAM
CPU_INT16S   RegData[200];	   //modbus 寄存器状态 1-100  存入EEPRAM

/*======================================================================================================*/
//字地址 0 - 255 (只取低8位)
//位地址 0 - 255 (只取低8位)

/* CRC 高位字节值表 */ 
const CPU_INT08U  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

/* CRC低位字节值表*/ 
const CPU_INT08U  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

/***************************************************************************
;** 函数名称: void ReceiveOver(void)
;** 功能描述:  接收超时
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期: 2014-7-19
;** 修改原因：
;** 说    明: 参考
;***************************************************************************/
void ReceiveOver(void)
{
        if(receTimeOut>0)
        {
            receTimeOut--;
            if(receTimeOut==0 && receCount>0)   //判断通讯接收是否超时
            {
                receCount = 0;                  //将接收地址偏移寄存器清零
                checkoutError = 0;	         //校验出错
                
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
;** 作　  者:   
;** 日　  期: 2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16U crc16(CPU_INT08U *puchMsg, CPU_INT16U usDataLen) 
{ 
	CPU_INT08U  uchCRCHi = 0xFF ; /* 高CRC字节初始化 */ 
	CPU_INT08U  uchCRCLo = 0xFF ; /* 低CRC 字节初始化 */ 
	CPU_INT32U uIndex ;        /* CRC循环中的索引 */ 

	while (usDataLen--) /* 传输消息缓冲区 */ 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; /* 计算CRC */ 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}

/***************************************************************************
;** 函数名称:  
;** 功能描述:  开始发送
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void beginSend(CPU_INT16U sendCount)
{
    CPU_INT16U i;
    Tx485_En();
    for(i=0;i<=sendCount;i++)
    {
           BSP_Usart4_WrByte(sendBuf[i]);      	
    }
	receCount = 0;   //清接收地址偏移寄存器
	checkoutError = 0;
    Rx485_En();
}

/***************************************************************************
;** 函数名称:  
;** 功能描述:  取线圈状态 返回0表示成功
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16S getCoilVal(CPU_INT16U addr,CPU_INT16S *tempData)
{
    CPU_INT16S result = 0;
    CPU_INT16U tempAddr;
    CPU_INT08U  *p;
    
    tempAddr = addr & 0xfff;
    p=CoilData;
    *tempData = *(p+tempAddr);
	
    return result;
}
/***************************************************************************
;** 函数名称: 
;** 功能描述: 读线圈状态  /操作码01
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void readCoil(void)
{
    CPU_INT08U  i,k,byteCount,exit=0;
    CPU_INT16U addr,tempAddr,CoilNum,crcData;
    CPU_INT16S tempData=0;
	
    addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
    tempAddr = addr;

    CoilNum = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);
    byteCount = (CPU_INT08U)((CoilNum+7)/8);         //读取的位个数【（N+7）/8】
										
	for(k=0;k<byteCount;k++)//字节位置
	{
		sendBuf[k+3] = 0;
		for(i=0;i<8;i++) //每8 个线圈合为一个字节，最后一个若不足8 位，未定义部分填0.前8 个线圈在第一个字节，最地址最小的线圈在最低位。依次类推
		{
			getCoilVal(tempAddr,&tempData);			
			sendBuf[k+3] |= (CPU_INT08U)(tempData << i);
			tempAddr++;

			if(tempAddr >= (addr+CoilNum))
			{	//读完
				exit = 1;
				break;
			}	
		}
		if(exit == 1)
			break;
	}
	
	sendBuf[0] = localAddr;
	sendBuf[1] = 0x01;	
	sendBuf[2] = byteCount;
	byteCount += 3;
	crcData = crc16(sendBuf,byteCount);	//
	sendBuf[byteCount] = crcData >> 8;
	byteCount++;
	sendBuf[byteCount] = crcData & 0xff;

	beginSend(byteCount);  //数据返回	
}

/***************************************************************************
;** 函数名称:  
;** 功能描述: 取寄存器值 返回0表示成功 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16S getRegisterVal(CPU_INT16U addr,CPU_INT16S *tempData)
{
	CPU_INT16U tempAddr;
	CPU_INT16S  *p,result = 0;
	
	tempAddr = addr & 0xfff;//4096
	p = RegData;
        
        *tempData=*(p+tempAddr);
	
	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  读寄存器  /操作码03
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void readRegisters(void)
{
	CPU_INT16U readCount,byteCount;
        CPU_INT16U addr,tempAddr,crcData;
	//CPU_INT08U  finsh;	//1完成  0出错
	CPU_INT16U i;
	CPU_INT16S  tempData = 0;	
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
        tempAddr = addr;

        readCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//要读的个数	
	byteCount = readCount * 2;
	
	for(i=0;i<byteCount;i+=2,tempAddr++)
	{
		getRegisterVal(tempAddr,&tempData);				
		sendBuf[i+3] = (CPU_INT08U)(tempData >> 8);				   		
		sendBuf[i+4] = (CPU_INT08U)(tempData & 0xff);			
	}
	
	sendBuf[0] = localAddr;
	sendBuf[1] = 3;
	sendBuf[2] = byteCount;
	byteCount += 3;
	crcData = crc16(sendBuf,byteCount);
	sendBuf[byteCount] = crcData >> 8;
	byteCount++;
	sendBuf[byteCount] = crcData & 0xff;

	beginSend(byteCount);      //返回数据
}
/***************************************************************************
;** 函数名称:  
;** 功能描述: 设定线圈状态 返回0表示成功 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16S setCoilVal(CPU_INT16U addr,CPU_INT16S tempData)
{
	CPU_INT16S  result = 0;
	CPU_INT16U tempAddr;
        CPU_INT08U  *p;

	p = CoilData;
	tempAddr = addr & 0xfff;
	
	*(p+tempAddr)=(CPU_INT08U)tempData;
        
        IIC_Channel=1;
//	if(tempAddr<400)	 //1--400地址 写入EEPRAM
//	{IIC_Write_Data(ModbusCoilAdd+tempAddr,(CPU_INT08U)tempData);}
             
//    Coil.Add  = tempAddr;	//返回需要处理的地址
//    Coil.Data = (CPU_INT08U)tempData;	//返回需要处理的data 

	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  强制单个线圈 /操作码05
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void forceSingleCoil(void)
{
	CPU_INT16U addr,tempAddr;
	CPU_INT16S  tempData=0;
	CPU_INT16U onOff;
	CPU_INT08U  i;
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
        tempAddr = addr;

	onOff = ((CPU_INT16U)(receBuf[4]<<8)) | ((CPU_INT16U)receBuf[5]);//要读的个数		
	if(onOff == 0xff00) 
	{	//设为ON
		tempData = 1;
	} 
	else if(onOff == 0x0000)
	{	//设为OFF
		tempData = 0;
	}

        Coil.Add  = tempAddr;	//返回需要处理的地址
	Coil.num  = 1;	       //返回需要处理个数 

	setCoilVal(tempAddr,tempData);	
	
	for(i=0;i<receCount;i++)
	{
		sendBuf[i] = receBuf[i];
	}
        
        if(receCount>0)
        {receCount--;}
	beginSend(receCount); //返回数据	
}

/***************************************************************************
;** 函数名称:  
;** 功能描述: 写单个寄存器 返回0表示成功 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16S SetCoilReg(CPU_INT16U addr,CPU_INT16S tempData)
{
	CPU_INT16S  result = 0,*p;
	static CPU_INT16U tempAddr;
	  
        p = RegData;
	tempAddr = addr & 0xfff;
	*(p+tempAddr)=tempData;

        IIC_Channel=1;
	if(tempAddr<100){
            //1--100 address write in EEPRAM
            IIC_Write_2Byte(OperateAdd(ModbusRegAdd,tempAddr),&tempData);
        }

	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  写单个寄存器 /操作码06
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:  
;***************************************************************************/
void WritetSingleRegister(void)
{
	CPU_INT16U addr,tempAddr;
	CPU_INT16S tempData=0;
	CPU_INT16S ReVal;
	CPU_INT08U  i;
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
        tempAddr = addr;

	ReVal = ((CPU_INT16S)(receBuf[4]<<8)) | ((CPU_INT16S)receBuf[5]);//寄存器值		
	tempData = ReVal;

	Rdis.Add = tempAddr;  //返回地址
	Rdis.num = 1;         //返回寄存器数量

	SetCoilReg(tempAddr,tempData);	
	
	for(i=0;i<receCount;i++)
	{
		sendBuf[i] = receBuf[i];
	}

        if(receCount>0)
        {receCount--;}
	beginSend(receCount); //返回数据
}
/***************************************************************************
;** 函数名称:  
;** 功能描述: 设定多线圈中的单个线圈， 返回0表示成功 
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:  L-W 
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16U SetMultipleCoilVal(CPU_INT16U addr,CPU_INT08U tempData)
{
	CPU_INT16U  result = 0;
	CPU_INT16U  tempAddr;
       CPU_INT08U   *p;

	p = CoilData;
	tempAddr = addr & 0xfff;
	
	*(p+tempAddr)=tempData;

//	if(tempAddr<400)	 //1--400地址 写入EEPRAM
//	{IIC_Write_Data(ModbusCoilAdd+tempAddr,(CPU_INT08U)tempData);}
	
	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  强制多个线圈  /操作码15
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:  L-W 
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void ForceMultipleCoils(void)
{
	CPU_INT16U addr,tempAddr,crcData;
	CPU_INT16U setCount;
	CPU_INT08U  byteCount,i,j=0,k,w;

	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
	tempAddr = addr;

	setCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//线圈数量
	
        byteCount = receBuf[6];	//字节数 
	
	w = byteCount + 7;	//高字节在前，低字节在后

        Coil.Add  = tempAddr;	//返回需要处理的地址
	Coil.num  = setCount;	//返回需要处理个数 

	for(i=0;i<setCount;i++)
	{				
		k=(receBuf[w]>>j)&0x01;
		SetMultipleCoilVal(tempAddr,k);
		tempAddr++;
		j++;
		if(j>=7)
		{
			j=0;
			w--;   //高字节在前，低字节在后
		}
	}

	sendBuf[0] = localAddr;
	sendBuf[1] = 15;
	sendBuf[2] = addr >> 8;
	sendBuf[3] = addr & 0xff;
	sendBuf[4] = setCount >> 8;
	sendBuf[5] = setCount & 0xff;
	crcData = crc16(sendBuf,6);
	sendBuf[6] = crcData >> 8;
	sendBuf[7] = crcData & 0xff;
	beginSend(7);	
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  保存寄存器值到EEPROM
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:  L-W 
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16U SaveRegisterToEEPROM(CPU_INT16U Addr)
{
	CPU_INT16U result = 0;
//	CPU_INT16U tempAddr;  
//	CPU_INT08U	 i;
//
//	tempAddr = Addr & 0xfff;
//	if((tempAddr>0)&&(tempAddr<100))
//	{
//
//	}

	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  设置寄存器值 返回0表示成功
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
CPU_INT16S setRegisterVal(CPU_INT16U addr,CPU_INT16S tempData)
{
	CPU_INT16S result = 0,*p;//,Tdata;
	CPU_INT16U tempAddr;

	p=RegData;
	tempAddr = addr & 0xfff; //4096
	*(p+tempAddr)=tempData;
        
        IIC_Channel=1;
	if(tempAddr<200)		  //1--100 address write in EEPRAM
	{
           //存储数据错位
           IIC_Write_2Byte(ModbusRegAdd+tempAddr,&tempData);
        //EXT1_BSP_Ser_Printf("%x",tempData);
           
         
        } 
	
	return result;
}

CPU_INT16S setMultiRegisterVal(CPU_INT16U addr,CPU_INT16S tempData,CPU_INT16U counter)
{
	CPU_INT16S result = 0,*p;//,Tdata;
	CPU_INT16U tempAddr;

	p=RegData;
	tempAddr = addr & 0xfff; //4096
	*(p+tempAddr)=tempData;
        
        IIC_Channel=1;
        
	if(tempAddr<200)		  //1--100 address write in EEPRAM
	{
           //存储数据错位
           
           if(counter>1){tempAddr++;}
           IIC_Write_2Byte(ModbusRegAdd+tempAddr,&tempData);
           
          // EXT1_BSP_Ser_Printf("%x",tempData); 
          
         
        } 
	
	return result;
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  设置多个寄存器  /操作码16
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void presetMultipleRegisters(void)
{
	CPU_INT16U addr,tempAddr,crcData;
	CPU_INT16S  tempData;
        CPU_INT16U i;
	CPU_INT16U setCount,byteCount;
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //存地址	
	tempAddr = addr;

	setCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//寄存器数量
	
         byteCount = receBuf[6];	//字节数  值：N*2
         byteCount = byteCount;

	Rdis.Add = tempAddr;  //返回地址
	Rdis.num = setCount;  //返回寄存器数量

	for(i=0;i<setCount;i++)
	{
		tempData = (((CPU_INT16S)(receBuf[i*2+7]<<8)) | ((CPU_INT16S)receBuf[i*2+8]));	
		setMultiRegisterVal(tempAddr,tempData,setCount);	
		tempAddr++;		
	}
	sendBuf[0] = localAddr;
	sendBuf[1] = 16;
	sendBuf[2] = addr >> 8;
	sendBuf[3] = addr & 0xff;
	sendBuf[4] = setCount >> 8;
	sendBuf[5] = setCount & 0xff;
	crcData = crc16(sendBuf,6);
	sendBuf[6] = crcData >> 8;
	sendBuf[7] = crcData & 0xff;
	beginSend(7);	
}

/***************************************************************************
;** 函数名称:  
;** 功能描述:  检查uart0数据
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
void checkComm0Modbus(void)
{
	CPU_INT16U crcData,TcrcD;
	CPU_INT16U tempData;
	
	if(receCount > 4)
	{
		switch(receBuf[1])
		{
			case 1://读取线圈状态(读取点 16位以内)
			case 3://读取保持寄存器(一个或多个)
			case 5://强制单个线圈
			case 6://设置单个寄存器
					if(receCount >= 8)
					{//接收完成一组数据
						//应该关闭接收中断						
						if(receBuf[0]==localAddr && checkoutError==0)
						{
                                                        crcData = crc16(receBuf,6);
                                                        TcrcD = (CPU_INT16U)(receBuf[6]<<8) | (CPU_INT16U)receBuf[7];
							if(crcData == TcrcD)
							{ //校验正确
                                
								if(receBuf[1] == 1)
								{ //读取线圈状态(读取点 16位以内)
									readCoil();								
								}
								else if(receBuf[1] == 3)
								{ //读取寄存器(多个)
									readRegisters(); 
								}
								else if(receBuf[1] == 5)
								{ //强制单个线圈
									forceSingleCoil();								
								}
								else if(receBuf[1] == 6)
								{  //写单寄存器
									WritetSingleRegister();								
								}

							}
						}						
						receCount = 0;	
						checkoutError = 0;
					}
					break;
			
			case 15://设置多个线圈
					tempData = receBuf[6]; 
					tempData += 9;	//数据个数
					if(receCount >= tempData)
					{
						if(receBuf[0]==localAddr && checkoutError==0)
						{
							crcData = crc16(receBuf,tempData-2);
							if(crcData == (receBuf[tempData-2]<<8)+ receBuf[tempData-1])
							{
								ForceMultipleCoils();			
							}
						}	
						receCount = 0;
						checkoutError = 0;
					}
					break;
			
			case 16://设置多个寄存器
					tempData = ((CPU_INT16U)receBuf[4]<<8) | (CPU_INT16U)receBuf[5];
					tempData = tempData * 2;	//数据个数
					tempData += 9;
					if(receCount >= tempData)
					{
						if(receBuf[0]==localAddr && checkoutError==0)
						{
                            crcData = crc16(receBuf,tempData-2);
							if(crcData == ((CPU_INT16U)receBuf[tempData-2]<<8) | (CPU_INT16U)receBuf[tempData-1])
							{
								presetMultipleRegisters();			
							}
						}	
						receCount = 0;
						checkoutError = 0;
					}
					break;
								
			default:
					break;			
		}
	}
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  写16位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:  高位在前，低位在后     reback 0 is right
;***************************************************************************/
//CPU_INT16U Write_2byte(CPU_INT16U Add,void *d)
//{
//	CPU_INT16S   *pt;
//	CPU_INT16U  tempAdd,result=0;
//
//	tempAdd = Add & 0xfff;
//
//	if(pt!=NULL)
//	{
//
//	}	
//	else
//	{
//		result=1;
//	}	
//	return result;			
//}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  写32位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2013-8-6
;** 修改原因：	  与显控不同
;** 说    明:	数据顺序是从低到高，但是每每两个byte 高位在前，低位在后
                如：123456789(75bcd15) 对应是：cd15075b
;***************************************************************************/
CPU_INT16U Write_4byte(void *Add,void *d)
{
	int      i;
	CPU_INT16U  result=0;
    
	for(i=0;i<4;i++)
	{
		*((char *)Add+i)= *((char *)d+i);
	}
						
	return result;			
}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  写leng位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:
;***************************************************************************/
//CPU_INT16U Write_Nbyte(CPU_INT16U Add,void *d,CPU_INT08U leng)
//{
//	CPU_INT08U i,j;
//	CPU_INT16U tempAdd,result=0;
//	node     *pt;
//
//	tempAdd = Add & 0xfff;
//
//	pt = find_num_linklist(headlink,tempAdd);
//	if(pt!=NULL)
//	{
//		if(pt->num<leng)
//		{j=leng;}
//		else
//		{j=pt->num;}
//		for(i=0;i<j;i++)
//		{
//			pt->array[i]=*((CPU_INT08U *)d+(i+1));
//			i++;
//			pt->array[i]=*((CPU_INT08U *)d+(i-1));
//		}
//	}
//	else
//	{
//		result=1;
//	}	
//	return result;			
//}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  读16位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:	 
;***************************************************************************/
//CPU_INT16U Read_2byte(CPU_INT16U Add,void *d)
//{
//	node *pt;
//	CPU_INT16U tempAdd,result=0;
//
//	tempAdd = Add & 0xfff;
//
//	pt = find_num_linklist(headlink,tempAdd);
//	if(pt!=NULL)
//	{
//		*((CPU_INT08U *)d) = pt->array[1];
//		*((CPU_INT08U *)d+1) = pt->array[0];	
//	}
//	else
//	{
//		result=1;
//	}	
//
//	return result;			
//}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  读32位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2014-7-19
;** 修改原因：
;** 说    明:	如：123456789(75bcd15) 对应是：cd15075b
;***************************************************************************/
//CPU_INT16U Read_4byte(CPU_INT16U Add,void *d)
//{
//	CPU_INT08U  i;
//	CPU_INT16U tempAdd,result=0;
//	node     *pt;
//
//	tempAdd = Add & 0xfff;
//	pt = find_num_linklist(headlink,tempAdd);
//	if(pt!=NULL)
//	{
//		for(i=0;i<pt->num;i++)
//		{
//			*((CPU_INT08U *)d+i) = pt->array[i+1];
//			i++;
//			*((CPU_INT08U *)d+i) = pt->array[i-1];
//		}
//	}
//	else
//	{
//		result=1;
//	}			
//	return result;			
//}
/***************************************************************************
;** 函数名称:  
;** 功能描述:  读leng位
;** 输入参数: 
;** 返 回 值: 
;** 全局变量: 
;** 调用模块: 
;** 作　  者:   L-W
;** 日　  期:  2013-6-13
;** 修改原因：
;** 说    明:	 如：123456789(75bcd15) 对应是：cd15075b
;***************************************************************************/
//CPU_INT16U Read_Nbyte(CPU_INT16U Add,void *d,CPU_INT08U leng)
//{
//	CPU_INT08U  i,j;
//	CPU_INT16U tempAdd,result=0;
//	node     *pt;
//
//	tempAdd = Add & 0xfff;
//	pt = find_num_linklist(headlink,tempAdd);
//	if(pt!=NULL)
//	{
//		if(pt->num<leng)
//		{j=leng;}
//		else
//		{j=pt->num;}
//		for(i=0;i<j;i++)
//		{
//			*((CPU_INT08U *)d+i) = pt->array[i+1];
//			i++;
//			*((CPU_INT08U *)d+i) = pt->array[i-1];
//		}
//	}
//	else
//	{
//		result=1;
//	}		
//	return result;			
//}








