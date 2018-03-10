#include  "bsp_lcd1602.h"

#define   SCK_1       PCout(2)
#define   SDA_1       PCout(1)

#define   SCL_4       PFout(8)
#define   SDA_4       PFout(9)

CPU_INT08U LCD_Speed=35;
CPU_INT08U  Reflesh_number;

void Write_CMD(uint8_t CMD,uint8_t Channel)
{
   	int8_t i;	
        //cmd
        SCK_1=0;
        delay_Nus(LCD_Speed);
        if(Channel == 1)     { LCD_CS1=0;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;}
        else if(Channel == 2){ LCD_CS1=1;LCD_CS2=0;LCD_CS3=1;LCD_CS4=1;}
        else if(Channel == 3){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=0;LCD_CS4=1;}
        else if(Channel == 4){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=0;}
        delay_Nus(10); //片选 10us
        SDA_1 = 0;
	delay_Nus(LCD_Speed);
        SCK_1=1;
        delay_Nus(LCD_Speed);
        SCK_1=0;
	delay_Nus(LCD_Speed);
	for(i=7;i>=0;i--)
        {
          SCK_1=0;
          delay_Nus(LCD_Speed);
          SDA_1  = (CMD>>i)&0x01;
          SCK_1=1;
          delay_Nus(LCD_Speed);
        }  
        SCK_1=1;
        delay_Nus(LCD_Speed);
}

void Write_DAT_one(uint8_t DAT1,uint8_t Channel)
{ 
        int8_t i;	
        //cmd
        SCK_1=0;
        delay_Nus(LCD_Speed);   
        if(Channel == 1)     { LCD_CS1=0;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;}
        else if(Channel == 2){ LCD_CS1=1;LCD_CS2=0;LCD_CS3=1;LCD_CS4=1;}
        else if(Channel == 3){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=0;LCD_CS4=1;}
        else if(Channel == 4){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=0;}
        delay_Nus(10); //片选 10us
        SDA_1 = 1;
        delay_Nus(LCD_Speed);
        SCK_1=1;
        delay_Nus(LCD_Speed);
        SCK_1=0;
        delay_Nus(LCD_Speed);
        for(i=7;i>=0;i--)
        {
          SCK_1=0;
          delay_Nus(LCD_Speed);
          SDA_1  = (DAT1>>i)&0x01; 
          SCK_1=1;
          delay_Nus(LCD_Speed);
        }  	
        SCK_1=1;
        delay_Nus(LCD_Speed);
}

//写入字符串函数
/*------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,uint8_t* DAT1,uint8_t* DAT2,uint8_t* DAT3,uint8_t* DAT4) 
{            							

    if (y == 0) 
    {     
      if(Reflesh_number==1){ LCD_CS1=0;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;delay_Nus(10);Write_CMD(0x80+x,1);} 
      if(Reflesh_number==2){ LCD_CS1=1;LCD_CS2=0;LCD_CS3=1;LCD_CS4=1;delay_Nus(10);Write_CMD(0x80+x,2);}  
      if(Reflesh_number==3){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=0;LCD_CS4=1;delay_Nus(10);Write_CMD(0x80+x,3);}   
      if(Reflesh_number==4){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=0;delay_Nus(10);Write_CMD(0x80+x,4);}     
    }
    else 
    {      
      if(Reflesh_number==1){ LCD_CS1=0;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;delay_Nus(10);Write_CMD(0xC0+x,1);}
      if(Reflesh_number==2){ LCD_CS1=1;LCD_CS2=0;LCD_CS3=1;LCD_CS4=1;delay_Nus(10);Write_CMD(0xC0+x,2);}
      if(Reflesh_number==3){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=0;LCD_CS4=1;delay_Nus(10);Write_CMD(0xC0+x,3);}
      if(Reflesh_number==4){ LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=0;delay_Nus(10);Write_CMD(0xC0+x,4);}    
    }
    
    if(Reflesh_number==1){
      while(*DAT1!='\0')	
      {	
        Write_DAT_one(*DAT1,1);     
        DAT1++;
      }
    }
    else if(Reflesh_number==2){
      while(*DAT2!='\0')	
      {	
        Write_DAT_one(*DAT2,2);     
        DAT2++;
      }
    }
    else if(Reflesh_number==3){
      while(*DAT3!='\0')	
      {	
        Write_DAT_one(*DAT3,3);     
        DAT3++;
      } 
    }
    else if(Reflesh_number==4){
      Reflesh_number=0;
      while(*DAT4!='\0')	
      {	
        Write_DAT_one(*DAT4,4);     
        DAT4++;
      } 
    }
    
    LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1; 
}
/*---------------------------------------------------*/
void Initialize()
{
        LCD_CS1=0;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;
        delay_Nms(5);
        
        Write_CMD(0x38,1);
        Write_CMD(0x38,1);
        Write_CMD(0x38,1);
        delay_Nms(5); 
        Write_CMD(0x38,1);
        delay_Nms(1);
        Write_CMD(0x01,1);
        delay_Nms(5);
        Write_CMD(0x0C,1);
        delay_Nms(5);
        
        LCD_CS1=1;LCD_CS2=0;LCD_CS3=1;LCD_CS4=1;
        delay_Nms(5);
        Write_CMD(0x38,2);
        Write_CMD(0x38,2);
        Write_CMD(0x38,2);
        delay_Nms(5); 
        Write_CMD(0x38,2);
        delay_Nms(1);
        Write_CMD(0x01,2);
        delay_Nms(5);
        Write_CMD(0x0C,2);
        delay_Nms(5);
        
        LCD_CS1=1;LCD_CS2=1;LCD_CS3=0;LCD_CS4=1;
        delay_Nms(5);
        Write_CMD(0x38,3);
        Write_CMD(0x38,3);
        Write_CMD(0x38,3);
        delay_Nms(5); 
        Write_CMD(0x38,3);
        delay_Nms(1);
        Write_CMD(0x01,3);
        delay_Nms(5);
        Write_CMD(0x0C,3);
        delay_Nms(5);
        
        LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=0;
        delay_Nms(5);
        Write_CMD(0x38,4);
        Write_CMD(0x38,4);
        Write_CMD(0x38,4);
        delay_Nms(5); 
        Write_CMD(0x38,4);
        delay_Nms(1);
        Write_CMD(0x01,4);
        delay_Nms(5);
        Write_CMD(0x0C,4);
        delay_Nms(5);
        
        //失能所有LCD
        LCD_CS1=1;LCD_CS2=1;LCD_CS3=1;LCD_CS4=1;
}
