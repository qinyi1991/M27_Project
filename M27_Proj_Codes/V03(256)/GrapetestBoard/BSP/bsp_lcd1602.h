/***********************定义app全局变量***************************************************************************/
#ifndef __BSP_LCD1602_H
#define __BSP_LCD1602_H

#define   LCD_CS1        PCout(0)
#define   LCD_CS2        PFout(10) 

#define   LCD_CS3        PFout(6)
#define   LCD_CS4        PFout(7)

#include <includes.h>
extern CPU_INT08U  LCD_Speed;
extern CPU_INT08U  Reflesh_number;

void Display_Strings(uint8_t start,uint8_t end,uint8_t *ptr);		 //set DDRAM address
void Initialize();
void LCD_Write_String(unsigned char x,unsigned char y,uint8_t* DAT1,uint8_t* DAT2,uint8_t* DAT3,uint8_t* DAT4);

#endif