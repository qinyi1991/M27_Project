#include "bsp_Readad127.h"

signed short int  sumttest;

signed short int filtersum[30];
signed short int filtertemp;
signed int filtersumvalue=0;

//unsigned int ADConvterValue;
unsigned char UpdataFlag=0;
void Sortarry(signed short int a[],uint32_t n);

///读取DRRDY信号
void ReadstandbySignal(void);

void EXTI_Configuration(void)
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);     

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  		  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_Init(GPIOC,&GPIO_InitStructure);
        

        // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;	 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource2);	 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 ;		
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	 
	EXTI_Init(&EXTI_InitStructure);
        
	EXTI_ClearITPendingBit(EXTI_Line2);
        
        BSP_IntVectSet(BSP_INT_ID_EXTI2,ReadstandbySignal);
        BSP_IntEn(BSP_INT_ID_EXTI2);    
}

//电平变化可以进中断
void ReadstandbySignal(void)
{
	static uint8_t  testresult1=0;
	static uint8_t  testresult2=0;
	static uint8_t  testresult3=0;
        
	signed short int ch1max=0;
	signed short int ch1min=1000;
        
	static signed int filtervalue[4];
	static uint8_t m=0;
	static unsigned char j;
        
        unsigned short int TempData=0;
        
        unsigned char k,i;

        
	//stati c unsigned int ADValue[50];
	sumttest=0;
	testresult1=SPI1_ReadWrite(0x00);
	testresult2=SPI1_ReadWrite(0x00);
	testresult3=SPI1_ReadWrite(0x00);
        
	TempData|=testresult1<<8|testresult2;
        
        sumttest=(signed short int)TempData;
     
	if(j<4)
	{
		filtervalue[j]=sumttest;
		j++;
	}
        
	if(j==4)
	{
                ch1max=filtervalue[0];
                ch1min=filtervalue[0];
		for(k=1;k<4;k++)
		{
				if(filtervalue[k]>filtervalue[0])
					ch1max=filtervalue[k];
				if(filtervalue[k]<filtervalue[0])
					ch1min=filtervalue[k];
		}
                
		filtertemp=(filtervalue[0]+filtervalue[1]+filtervalue[2]+filtervalue[3]-ch1max-ch1min)/2;
		j=0;
                
		if(m<30)
		{
			filtersum[m]=filtertemp;
			m++;
			if(m>=30)
			{
				m=0;
				 Sortarry(filtersum,30); //排序
				
				for(i=10;i<20;i++)
				{
					filtersumvalue=filtersumvalue+filtersum[i];
				} 
				ADC1_ConvertedValue=filtersumvalue;
                                
 				filtersumvalue=0;
				UpdataFlag=1;

			}
		}
	}
	EXTI_ClearITPendingBit(EXTI_Line2); 
}
 
void Sortarry(signed short int a[],uint32_t n)
{
	uint16_t i,j,t;
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(a[j]>a[j+1])
			{
				t=a[j];
				a[j]=a[j+1];
				a[j+1]=t;
			}
		}
	}
}