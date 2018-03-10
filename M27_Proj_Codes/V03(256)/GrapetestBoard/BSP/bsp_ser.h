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
* Filename      : bsp_ser.h
* Version       : V1.00
* Programmer(s) : EHS
*                 SR
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This BSP serial header file is protected from multiple pre-processor inclusion through
*               use of the BSP serial present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_SER_PRESENT                                        /* See Note #1.                                         */
#define  BSP_SER_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_SER_MODULE
#define  BSP_SER_EXT
#else
#define  BSP_SER  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/

#define  BSP_SER_COMM_UART_NONE                0xFF
#define  BSP_SER_COMM_UART_02                     2

#define  BSP_SER_PRINTF_STR_BUF_SIZE             80u


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/

extern CPU_INT08U  recCount;	          //接收到的字节个
extern CPU_INT08U  recBuf[100];              //发送接收缓冲

/*
*********************************************************************************************************
*                                   BACKWARD COMPATIBILITY MACRO'S
*
* Note(s) : (1) Those macro's are provided to preserve backward compatibility with earlier versions of
*               this BSP.
*********************************************************************************************************
*/

                                                                /* See Note #1.                                         */
#define  Ser_Init                       BSP_Ser_Init
#define  Ser_WrByte                     BSP_Ser_WrByte
#define  Ser_WrStr                      BSP_Ser_WrStr
#define  Ser_RdByte                     BSP_Ser_RdByte
#define  Ser_RdStr                      BSP_Ser_RdStr
#define  Ser_Printf                     BSP_Ser_Printf
extern CPU_INT08U  Usart3RxCount;	      //接收到的字节个
extern CPU_INT08U  Usart3RxBuf[10];          //发送接收缓冲

/*
*********************************************************************************************************
*                                              DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* --------------- RS-232 SERVICE FNCTS --------------- */
//void        BSP_Usart1_Init (CPU_INT32U   baud_rate); //usart1
//void        BSP_Usart2_Init (CPU_INT32U  baud_rate);  //usart2
//void        BSP_Usart3_Init (CPU_INT32U   baud_rate); //usart3
void        BSP_Usart4_Init (CPU_INT32U  baud_rate);  //usart4

//void        BSP_Usart1_WrByte(CPU_INT08U  c);
//void        BSP_Usart2_WrByte(CPU_INT08U  c);
//void        BSP_Usart3_WrByte(CPU_INT08U  c);
void        BSP_Usart4_WrByte(CPU_INT08U  c);

//void        BSP_Usart1_WrStr (CPU_INT08U  *p_str);
//void        BSP_Usart2_WrStr (CPU_INT08U  *p_str);
//void        BSP_Usart3_WrStr (CPU_INT08U  *p_str);
void        BSP_Usart4_WrStr (CPU_INT08U  *p_str);

//void        BSP_Usart1_Printf (CPU_INT08U  *format, ...);
//void        BSP_Usart2_Printf (CPU_INT08U  *format, ...);
//void        BSP_Usart3_Printf (CPU_INT08U  *format, ...);
void        BSP_Usart4_Printf (CPU_INT08U  *format, ...);

//extern void Usart2SendCmd(uint8_t *BufferPtr,uint8_t sendCount);
//extern void Usart3SendCmd(uint8_t *BufferPtr,uint8_t sendCount);

/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

//#ifndef  BSP_CFG_SER_COMM_SEL
//#error  "BSP_CFG_SER_COMM_SEL                 not #define'd in 'app_cfg.h'        "
//#error  "                               [MUST be BSP_SER_COMM_UART_02]            "
//
//#elif   (BSP_CFG_SER_COMM_SEL != BSP_SER_COMM_UART_02)
//#error  "BSP_CFG_SER_COMM_SEL           illegally #define'd in 'app_cfg.h'        "
//#error  "                               [MUST be BSP_SER_COMM_UART_02]            "
//#endif


/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'bsp_ser.h  MODULE'.
*********************************************************************************************************
*/

#endif                                                          /* End of BSP ser module include.                       */
