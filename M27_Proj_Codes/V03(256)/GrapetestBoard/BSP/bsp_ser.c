/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                          (c) Copyright 2003-2010; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               This BSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                       SERIAL (UART) INTERFACE
*
* Filename      : bsp_ser.c
* Version       : V1.00
* Programmer(s) : EHS
*                 SR
*                 AA
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_SER_MODULE
#include <bsp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
//CPU_INT08U  recCount;	          //接收到的字节个
//CPU_INT08U  recBuf[100];          //发送接收缓冲
//
//CPU_INT08U  Usart2RxCount;	  //接收到的字节个
//CPU_INT08U  Usart2RxBuf[10];      //发送接收缓冲
//
//CPU_INT08U  Usart3RxCount;	  //接收到的字节个
//CPU_INT08U  Usart3RxBuf[10];      //发送接收缓冲
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
//static  void        EXT1_BSP_Ser_ISR_Handler(void);
//static  void        EXT2_BSP_Ser_ISR_Handler(void);
//static  void        EXT3_BSP_Ser_ISR_Handler(void);
static  void        EXT4_BSP_Ser_ISR_Handler(void);
/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                          EXT1_BSP_Ser_Init()
*
* Description : Initialize a serial port for communication.
*
* Argument(s) : baud_rate           The desire RS232 baud rate.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
//void BSP_Usart1_Init(CPU_INT32U  baud_rate)
//{
//    FlagStatus              tc_status;
//    GPIO_InitTypeDef        gpio_init;
//    USART_InitTypeDef       usart_init;
//    USART_ClockInitTypeDef  usart_clk_init;
//    NVIC_InitTypeDef   NVIC_InitStructure;	
//    
//#if (BSP_CFG_SER_CMD_HISTORY_LEN > 0u)
//    BSP_SerCmdHistory[0] = (CPU_INT08U)'\0';
//#endif
//
//                                                                /* ----------------- INIT USART STRUCT ---------------- */
//    usart_init.USART_BaudRate            = baud_rate;
//    usart_init.USART_WordLength          = USART_WordLength_8b;
//    usart_init.USART_StopBits            = USART_StopBits_1;
//    usart_init.USART_Parity              = USART_Parity_No ;
//    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//
//    usart_clk_init.USART_Clock           = USART_Clock_Disable;
//    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
//    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
//    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;
//
//
//    BSP_PeriphEn(BSP_PERIPH_ID_USART1);
//    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);
//    BSP_PeriphEn(BSP_PERIPH_ID_AFIO); 
//                                                                 /* ----------------- SETUP USART2 GPIO ---------------- */
//                                                                /* Configure GPIOD.9 as push-pull.                      */
//    gpio_init.GPIO_Pin   = GPIO_Pin_9;
//    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &gpio_init);
//
//                                                                /* Configure GPIOD.10 as input floating.                 */
//    gpio_init.GPIO_Pin   = GPIO_Pin_10;
//    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &gpio_init);
//
//                                                                /* ------------------ SETUP USART2 -------------------- */
//    USART_Init(USART1, &usart_init);
//    USART_ClockInit(USART1, &usart_clk_init);
//    USART_Cmd(USART1, ENABLE);
//    
//    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//    tc_status  = USART_GetFlagStatus(USART1, USART_FLAG_TC);
//    
//    while (tc_status == SET) {
//        USART_ClearITPendingBit(USART1, USART_IT_TC);
//        USART_ClearFlag(USART1, USART_IT_TC);
//        //BSP_OS_TimeDlyMs(10); //任务控制权被释放
//        tc_status = USART_GetFlagStatus(USART1, USART_FLAG_TC);        
//    }
//           
//    BSP_IntVectSet(BSP_INT_ID_USART1,EXT1_BSP_Ser_ISR_Handler);
//    BSP_IntEn(BSP_INT_ID_USART1); //使能中断
//     
//   // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//0位用于pre-emption priority 4位用于subpriority
//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0f; 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;	 
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);	
//}
//
//void BSP_Usart2_Init(CPU_INT32U  baud_rate)
//{
//    FlagStatus              tc_status;
//    GPIO_InitTypeDef        gpio_init;
//    USART_InitTypeDef       usart_init;
//    USART_ClockInitTypeDef  usart_clk_init;
//    NVIC_InitTypeDef   NVIC_InitStructure;	
//    
//#if (BSP_CFG_SER_CMD_HISTORY_LEN > 0u)
//    BSP_SerCmdHistory[0] = (CPU_INT08U)'\0';
//#endif
//   /* ----------------- INIT USART STRUCT ---------------- */
//    usart_init.USART_BaudRate            = baud_rate;
//    usart_init.USART_WordLength          = USART_WordLength_8b;
//    usart_init.USART_StopBits            = USART_StopBits_1;
//    usart_init.USART_Parity              = USART_Parity_No ;
//    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//
//    usart_clk_init.USART_Clock           = USART_Clock_Disable;
//    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
//    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
//    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;
//
//
//    BSP_PeriphEn(BSP_PERIPH_ID_USART2);
//    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);
//    BSP_PeriphEn(BSP_PERIPH_ID_AFIO); 
//                                                                 /* ----------------- SETUP USART2 GPIO ---------------- */
//                                                                /* Configure GPIOD.9 as push-pull.                      */
//    gpio_init.GPIO_Pin   = GPIO_Pin_2;
//    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &gpio_init);
//
//                                                                /* Configure GPIOD.10 as input floating.                 */
//    gpio_init.GPIO_Pin   = GPIO_Pin_3;
//    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOA, &gpio_init);
//
//                                                                /* ------------------ SETUP USART2 -------------------- */
//    USART_Init(USART2, &usart_init);
//    USART_ClockInit(USART2, &usart_clk_init);
//    USART_Cmd(USART2, ENABLE);
//    
//    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//    tc_status  = USART_GetFlagStatus(USART2, USART_FLAG_TC);
//    
//    while (tc_status == SET) {
//        USART_ClearITPendingBit(USART2, USART_IT_TC);
//        USART_ClearFlag(USART2, USART_IT_TC);
//        tc_status = USART_GetFlagStatus(USART2, USART_FLAG_TC);        
//    }
//           
//    BSP_IntVectSet(BSP_INT_ID_USART2,EXT2_BSP_Ser_ISR_Handler);
//    BSP_IntEn(BSP_INT_ID_USART2); //使能中断
//     
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;	 
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);	
//}
//
//void BSP_Usart3_Init(CPU_INT32U  baud_rate)
//{
//    FlagStatus              tc_status;
//    GPIO_InitTypeDef        gpio_init;
//    USART_InitTypeDef       usart_init;
//    USART_ClockInitTypeDef  usart_clk_init;
//    NVIC_InitTypeDef   NVIC_InitStructure;	
//    
//#if (BSP_CFG_SER_CMD_HISTORY_LEN > 0u)
//    BSP_SerCmdHistory[0] = (CPU_INT08U)'\0';
//#endif
//   /* ----------------- INIT USART STRUCT ---------------- */
//    usart_init.USART_BaudRate            = baud_rate;
//    usart_init.USART_WordLength          = USART_WordLength_8b;
//    usart_init.USART_StopBits            = USART_StopBits_1;
//    usart_init.USART_Parity              = USART_Parity_No ;
//    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
//
//    usart_clk_init.USART_Clock           = USART_Clock_Disable;
//    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
//    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
//    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;
//
//
//    BSP_PeriphEn(BSP_PERIPH_ID_USART3);
//    BSP_PeriphEn(BSP_PERIPH_ID_IOPB);
//    BSP_PeriphEn(BSP_PERIPH_ID_AFIO); 
//                                                                 /* ----------------- SETUP USART2 GPIO ---------------- */
//                                                                /* Configure GPIOD.9 as push-pull.                      */
//    gpio_init.GPIO_Pin   = GPIO_Pin_10;
//    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOB, &gpio_init);
//
//                                                                /* Configure GPIOD.10 as input floating.                 */
//    gpio_init.GPIO_Pin   = GPIO_Pin_11;
//    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(GPIOB, &gpio_init);
//
//                                                                /* ------------------ SETUP USART2 -------------------- */
//    USART_Init(USART3, &usart_init);
//    USART_ClockInit(USART3, &usart_clk_init);
//    USART_Cmd(USART3, ENABLE);
//    
//    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
//    tc_status  = USART_GetFlagStatus(USART3, USART_FLAG_TC);
//    
//    while (tc_status == SET) {
//        USART_ClearITPendingBit(USART3, USART_IT_TC);
//        USART_ClearFlag(USART3, USART_IT_TC);
//        tc_status = USART_GetFlagStatus(USART3, USART_FLAG_TC);        
//    }
//           
//    BSP_IntVectSet(BSP_INT_ID_USART3,EXT3_BSP_Ser_ISR_Handler);
//    BSP_IntEn(BSP_INT_ID_USART3); //使能中断
//     
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
//    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;	 
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure); 
//}

void BSP_Usart4_Init(CPU_INT32U  baud_rate)
{
    FlagStatus              tc_status;
    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
    NVIC_InitTypeDef   NVIC_InitStructure;	
    
#if (BSP_CFG_SER_CMD_HISTORY_LEN > 0u)
    BSP_SerCmdHistory[0] = (CPU_INT08U)'\0';
#endif
   /* ----------------- INIT USART STRUCT ---------------- */
    usart_init.USART_BaudRate            = baud_rate;
    usart_init.USART_WordLength          = USART_WordLength_8b;
    usart_init.USART_StopBits            = USART_StopBits_1;
    usart_init.USART_Parity              = USART_Parity_No ;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;


    BSP_PeriphEn(BSP_PERIPH_ID_USART4);
    BSP_PeriphEn(BSP_PERIPH_ID_IOPC);
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO); 
                                                                 /* ----------------- SETUP USART2 GPIO ---------------- */
                                                                /* Configure GPIOD.9 as push-pull.                      */
    gpio_init.GPIO_Pin   = GPIO_Pin_10;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &gpio_init);

                                                                /* Configure GPIOD.10 as input floating.                 */
    gpio_init.GPIO_Pin   = GPIO_Pin_11;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC, &gpio_init);

                                                                /* ------------------ SETUP USART2 -------------------- */
    USART_Init(UART4, &usart_init);
    USART_ClockInit(UART4, &usart_clk_init);
    USART_Cmd(UART4, ENABLE);
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
    USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
    tc_status  = USART_GetFlagStatus(UART4, USART_FLAG_TC);
    
    while (tc_status == SET) {
        USART_ClearITPendingBit(UART4, USART_IT_TC);
        USART_ClearFlag(UART4, USART_IT_TC);
        tc_status = USART_GetFlagStatus(UART4, USART_FLAG_TC);        
    }
           
    BSP_IntVectSet(BSP_INT_ID_USART4,EXT4_BSP_Ser_ISR_Handler);
    BSP_IntEn(BSP_INT_ID_USART4); //使能中断
     
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;	 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
    Rx485_En();

}
/*
*********************************************************************************************************
*                                                BSP_Ser_WrByte()
*
* Description : Writes a single byte to a serial port.
*
* Argument(s) : tx_byte     The character to output.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
//void BSP_Usart1_WrByte(CPU_INT08U  c)
//{
//      USART_SendData(USART1, c); 
//      while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
//      USART_ClearFlag(USART1,USART_FLAG_TC); 	
//}
//void BSP_Usart2_WrByte(CPU_INT08U  c)
//{
//      USART_SendData(USART2, c); 
//      while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
//      USART_ClearFlag(USART2,USART_FLAG_TC); 	
//}
//void BSP_Usart3_WrByte(CPU_INT08U  c)
//{
//      USART_SendData(USART3, c); 
//      while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
//      USART_ClearFlag(USART3,USART_FLAG_TC); 	
//}
void BSP_Usart4_WrByte(CPU_INT08U  c)
{
      USART_SendData(UART4, c); 
      while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
      USART_ClearFlag(UART4,USART_FLAG_TC); 	
}

/*
*********************************************************************************************************
*                                                BSP_Ser_WrStr()
*
* Description : Transmits a string.
*
* Argument(s) : p_str       Pointer to the string that will be transmitted.
*
* Caller(s)   : Application.
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/
//void  BSP_Usart1_WrStr (CPU_INT08U  *p_str)
//{
//  
//    if (p_str == (CPU_INT08U *)0) {
//        return;
//    }
//       
//    while ((*p_str) != (CPU_INT08U )0) {
////        if (*p_str == ASCII_CHAR_LINE_FEED) {
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_CARRIAGE_RETURN);
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_LINE_FEED);
////            p_str++;
////        } else {
//            BSP_Usart1_WrByte(*p_str++);
////        }
//    }
//    
//}
//void  BSP_Usart2_WrStr(CPU_INT08U  *p_str)
//{
//  
//    if (p_str == (CPU_INT08U *)0) {
//        return;
//    }
//       
//    while ((*p_str) != (CPU_INT08U )0) {
////        if (*p_str == ASCII_CHAR_LINE_FEED) {
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_CARRIAGE_RETURN);
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_LINE_FEED);
////            p_str++;
////        } else {
//            BSP_Usart2_WrByte(*p_str++);
////        }
//    }   
//}
//void  BSP_Usart3_WrStr(CPU_INT08U  *p_str)
//{
//  
//    if (p_str == (CPU_INT08U *)0) {
//        return;
//    }
//       
//    while ((*p_str) != (CPU_INT08U )0) {
////        if (*p_str == ASCII_CHAR_LINE_FEED) {
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_CARRIAGE_RETURN);
////            BSP_Ser_WrByteUnlocked(ASCII_CHAR_LINE_FEED);
////            p_str++;
////        } else {
//            BSP_Usart3_WrByte(*p_str++);
////        }
//    }  
//}

void  BSP_Usart4_WrStr(CPU_INT08U  *p_str)
{
  
    if (p_str == (CPU_INT08U *)0) {
        return;
    }
       
    while ((*p_str) != (CPU_INT08U )0) {
//        if (*p_str == ASCII_CHAR_LINE_FEED) {
//            BSP_Ser_WrByteUnlocked(ASCII_CHAR_CARRIAGE_RETURN);
//            BSP_Ser_WrByteUnlocked(ASCII_CHAR_LINE_FEED);
//            p_str++;
//        } else {
            BSP_Usart4_WrByte(*p_str++);
//        }
    }
    
}

/*
*********************************************************************************************************
*                                         BSP_Ser_Printf()
*
* Description : Print formatted data to the output serial port.
*
* Argument(s) : format      String that contains the text to be written.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function output a maximum of BSP_SER_PRINTF_STR_BUF_SIZE number of bytes to the
*                   serial port.  The calling function hence has to make sure the formatted string will
*                   be able fit into this string buffer or hence the output string will be truncated.
*********************************************************************************************************
*/

//void  BSP_Usart1_Printf (CPU_INT08U  *format, ...)
//{
//    CPU_INT08U  buf_str[BSP_SER_PRINTF_STR_BUF_SIZE + 1u];
//    va_list   v_args;
//
//
//    va_start(v_args, format);
//   (void)vsnprintf((char       *)&buf_str[0],
//                   (size_t      ) sizeof(buf_str),
//                   (char const *) format,
//                                  v_args);
//    va_end(v_args);
//
//    BSP_Usart1_WrStr(buf_str);
//}
//
//void  BSP_Usart2_Printf (CPU_INT08U  *format, ...)
//{
//    CPU_INT08U  buf_str[BSP_SER_PRINTF_STR_BUF_SIZE + 1u];
//    va_list   v_args;
//
//
//    va_start(v_args, format);
//   (void)vsnprintf((char       *)&buf_str[0],
//                   (size_t      ) sizeof(buf_str),
//                   (char const *) format,
//                                  v_args);
//    va_end(v_args);
//
//   BSP_Usart2_WrStr(buf_str);
//}
//void  BSP_Usart3_Printf (CPU_INT08U  *format, ...)
//{
//    CPU_INT08U  buf_str[BSP_SER_PRINTF_STR_BUF_SIZE + 1u];
//    va_list   v_args;
//
//
//    va_start(v_args, format);
//   (void)vsnprintf((char       *)&buf_str[0],
//                   (size_t      ) sizeof(buf_str),
//                   (char const *) format,
//                                  v_args);
//    va_end(v_args);
//
//   BSP_Usart3_WrStr(buf_str);
//}

void  BSP_Usart4_Printf (CPU_INT08U  *format, ...)
{
    CPU_INT08U  buf_str[BSP_SER_PRINTF_STR_BUF_SIZE + 1u];
    va_list   v_args;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

   BSP_Usart4_WrStr(buf_str);
}

//
//void Usart3SendCmd(uint8_t *BufferPtr,uint8_t sendCount)
//{
//    uint8_t i;
//    for(i=0;i<sendCount;i++)
//    {
//      BSP_Usart3_WrByte(BufferPtr[i]);      	
//    }
//}
//void Usart2SendCmd(uint8_t *BufferPtr,uint8_t sendCount)
//{
//    uint8_t i;
//    for(i=0;i<sendCount;i++)
//    {
//      BSP_Usart2_WrByte(BufferPtr[i]);      	
//    }
//}

/*
*********************************************************************************************************
*                                         BSP_Ser_ISR_Handler()
*
* Description : Serial ISR.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/
//void EXT2_BSP_Ser_ISR_Handler(void)
//{
//    FlagStatus rxne_status;
//    uint8_t Tmp_data;
//    
//    //使用时隙断句法，判断接受到一帧数据
//    rxne_status=USART_GetITStatus(USART2, USART_IT_RXNE);
//    if(rxne_status == SET){ 
//            Tmp_data=USART_ReceiveData(USART2);
//            
//            if((Tmp_data=='+')||(Tmp_data=='-'))
//		{
//				Usart2RxBuf[1]=Tmp_data;
//				Usart2RxCount=2;
//		}
//		else if( Usart2RxCount<sizeof(Usart2RxBuf)-1)
//		{
//			Usart2RxBuf[Usart2RxCount++]=Tmp_data;
//			if( ((Usart2RxBuf[1]=='+')||(Usart2RxBuf[1]=='-'))&&(Usart2RxCount==8))
//			{
//				RightLoadCellCheck((char *)Usart2RxBuf);
//                                Usart2RxCount=0; 
//			}
//		}
////		else
////		{
////			Usart3RxCount=0;
////			Usart3SendCmd(cmd, 4 );
////		} 
//            USART_ClearITPendingBit(USART3,USART_IT_RXNE);     
//    }    
//}
//
//
/*
*********************************************************************************************************
*                                         BSP_Ser_ISR_Handler()
*
* Description : Serial ISR.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/
//void EXT1_BSP_Ser_ISR_Handler(void)
//{
//    FlagStatus rxne_status;
//    OS_ERR err;
//    uint8_t Tmp_data;
//    
//    rxne_status=USART_GetITStatus(USART1, USART_IT_RXNE);
//    
//    if(rxne_status == SET){ 
//            
//            Tmp_data=USART_ReceiveData(USART1);
//            
//            if((Tmp_data == '\r')||(Tmp_data == '\n')){  //通知AppTaskSerDataHandling任务处理数据
//              if(Tmp_data == '\n')
//              OSTaskSemPost((OS_TCB *)&AppTaskSerCmdTCB,
//                            (OS_OPT  )OS_OPT_POST_NONE,
//                            (OS_ERR *)&err);   
//            }else{
//                recBuf[recCount] = Tmp_data;
//            }
//            
//            if(recCount<BUFFERSIZE){recCount++;}
//            else {
//                recCount=0;
//                AppTask_Fixture_Status.F_RXERRO=1;
//                for(;recCount<BUFFERSIZE;recCount++)
//                {recBuf[recCount] = 0;}
//                
//            }
//            
//      USART_ClearITPendingBit(USART1,USART_IT_RXNE);      
//    }    
//}
//
//void EXT3_BSP_Ser_ISR_Handler(void)
//{
//    FlagStatus rxne_status;
//    uint8_t Tmp_data;
//    
//    rxne_status=USART_GetITStatus(USART3, USART_IT_RXNE);
//    
//    if(rxne_status == SET){ 
//            Tmp_data=USART_ReceiveData(USART3);
//            
//            if((Tmp_data=='+')||(Tmp_data=='-'))
//		{
//				Usart3RxBuf[1]=Tmp_data;
//				Usart3RxCount=2;
//		}
//		else if( Usart3RxCount<sizeof(Usart3RxBuf)-1)
//		{
//			Usart3RxBuf[Usart3RxCount++]=Tmp_data;
//			if( ((Usart3RxBuf[1]=='+')||(Usart3RxBuf[1]=='-'))&&(Usart3RxCount==8))
//			{
//				LeftLoadCellCheck((char *)Usart3RxBuf);
//                                Usart3RxCount=0; 
//			}
//		}
////		else
////		{
////			Usart3RxCount=0;
////			Usart3SendCmd(cmd, 4 );
////		} 
//            USART_ClearITPendingBit(USART3,USART_IT_RXNE);     
//    } 
//}

void EXT4_BSP_Ser_ISR_Handler(void)
{
    FlagStatus rxne_status;
    OS_ERR err;

    
    rxne_status=USART_GetITStatus(UART4, USART_IT_RXNE);
    
    if(rxne_status == SET){ 
            
           receTimeOut = 18;  //时隙设置 
          
          receBuf[receCount] = USART_ReceiveData(UART4);
          
          if(receCount<100){receCount++;}         
          
          if(receCount>7){  //通知AppTaskSerDataHandling任务处理数据
                OSTaskSemPost((OS_TCB *)&AppTaskSerDataHandlingTCB,
                              (OS_OPT  )OS_OPT_POST_NONE,
                             (OS_ERR *)&err);   
          } 
          USART_ClearITPendingBit(UART4,USART_IT_RXNE);    
    } 
    
}