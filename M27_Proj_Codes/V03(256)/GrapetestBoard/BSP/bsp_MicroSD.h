/***********************定义bsp_MicroSD全局变量***************************************************************************/
#ifndef __BSP_MICROSD_H
#define __BSP_MICROSD_H

#include <bsp.h>


/******************************************************************************************************************************************
管脚定义说明
Micro SD卡通信协议
//1, PD2_CMD
//2, PC12_CLK
//3，PC11_DAT3
//4，PC10_DAT2
//5，PC9_DAT2
//6，PC8_DAT0
*******************************************************************************************************************************************/
//通讯调节分频因子
//HCLK == 72 MHz,SDIOCLK == 72 MHz
//计算公式：SDIO_CK == SDIOCLK / (CLKDIV+3)

#define SDIO_INIT_CLK_DIV                  ((CPU_INT08U)0xb2)			//400KHz----> 0xb2(178)
#define SDIO_TRANSFER_CLK_DIV              ((CPU_INT08U)0x02)           //18 MHz

//24MHz ----> 0x01
//18MHz ----> 0x02
//12MHz ----> 0x04

/**********************************************************
         SDIO工作模式：

**********************************************************/

#define SD_DMA_MODE                                ((CPU_INT32U)0x00000000)
//#define SD_POLLING_MODE                          ((CPU_INT32U)0x00000002)


/**********************************************************
         相关宏定义
**********************************************************/

//SDIO Static flags, TimeOut, FIFO Address
//#define NULL				           0
#define SDIO_STATIC_FLAGS               ((CPU_INT32U)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((CPU_INT32U)0x00010000)
#define SDIO_DATATIMEOUT                ((CPU_INT32U)0x0001FFFF)

//Mask for errors Card Status R1 (OCR Register)
#define SD_OCR_ADDR_OUT_OF_RANGE        ((CPU_INT32U)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((CPU_INT32U)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((CPU_INT32U)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((CPU_INT32U)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((CPU_INT32U)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((CPU_INT32U)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((CPU_INT32U)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((CPU_INT32U)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((CPU_INT32U)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((CPU_INT32U)0x00200000)
#define SD_OCR_CC_ERROR                 ((CPU_INT32U)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((CPU_INT32U)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((CPU_INT32U)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((CPU_INT32U)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((CPU_INT32U)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((CPU_INT32U)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((CPU_INT32U)0x00004000)
#define SD_OCR_ERASE_RESET              ((CPU_INT32U)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((CPU_INT32U)0x00000008)
#define SD_OCR_ERRORBITS                ((CPU_INT32U)0xFDFFE008)

//Masks for R6 Response
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((CPU_INT32U)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((CPU_INT32U)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((CPU_INT32U)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((CPU_INT32U)0x80100000)
#define SD_VOLTAGE_WINDOW_MMC           ((CPU_INT32U)0x80FF8000)
#define SD_HIGH_CAPACITY                ((CPU_INT32U)0x40000000)
#define SD_STD_CAPACITY                 ((CPU_INT32U)0x00000000)
#define SD_CHECK_PATTERN                ((CPU_INT32U)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((CPU_INT32U)0x0000FFFF)
#define SD_ALLZERO                      ((CPU_INT32U)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((CPU_INT32U)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((CPU_INT32U)0x00010000)
#define SD_CARD_LOCKED                  ((CPU_INT32U)0x02000000)

#define SD_DATATIMEOUT                  ((CPU_INT32U)0xFFFFFFFF)
#define SD_0TO7BITS                     ((CPU_INT32U)0x000000FF)
#define SD_8TO15BITS                    ((CPU_INT32U)0x0000FF00)
#define SD_16TO23BITS                   ((CPU_INT32U)0x00FF0000)
#define SD_24TO31BITS                   ((CPU_INT32U)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((CPU_INT32U)0x01FFFFFF)

#define SD_HALFFIFO                     ((CPU_INT32U)0x00000008)
#define SD_HALFFIFOBYTES                ((CPU_INT32U)0x00000020)

//Command Class Supported
#define SD_CCCC_LOCK_UNLOCK             ((CPU_INT32U)0x00000080)
#define SD_CCCC_WRITE_PROT              ((CPU_INT32U)0x00000040)
#define SD_CCCC_ERASE                   ((CPU_INT32U)0x00000020)

//Following commands are SD Card Specific commands.
//SDIO_APP_CMD should be sent before sending these commands.
#define SDIO_SEND_IF_COND               ((CPU_INT32U)0x00000008)


/**********************************************************
        SD卡初始化和读写卡的状态信息定义
**********************************************************/

typedef enum
{
	/* 初始过程中的错误信息定义 */
	  
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),

	 
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
}SD_Error;

//DMA的传输方向
#define DMA2_ReadDATA				0	
#define DMA2_WriteDATA				1	

//SDIO的传输方向
#define SDIO_To_Card				0	
#define Card_To_SDIO				1	

//SD命令的响应长短
#define SD_Response_No                    	0	//无响应
#define SD_Response_Short                 	1	//短响应
#define SD_Response_Long                  	3       //长响应

typedef enum
{
	SD_TRANSFER_OK   = 0,	 //传输成功
	SD_TRANSFER_BUSY = 1,	 //传输忙
	SD_TRANSFER_ERROR	 //传输错误	
}SDTransferState;

typedef enum
{
  SD_CARD_READY                  = ((CPU_INT32U)0x00000001),
  SD_CARD_IDENTIFICATION         = ((CPU_INT32U)0x00000002),
  SD_CARD_STANDBY                = ((CPU_INT32U)0x00000003),
  SD_CARD_TRANSFER               = ((CPU_INT32U)0x00000004),
  SD_CARD_SENDING                = ((CPU_INT32U)0x00000005),
  SD_CARD_RECEIVING              = ((CPU_INT32U)0x00000006),
  SD_CARD_PROGRAMMING            = ((CPU_INT32U)0x00000007),
  SD_CARD_DISCONNECTED           = ((CPU_INT32U)0x00000008),
  SD_CARD_ERROR                  = ((CPU_INT32U)0x000000FF)
}SDCardState;

/**********************************************************
                  卡信息寄存器结构
**********************************************************/

//CSD寄存器的定义
typedef struct
{
	__IO CPU_INT08U  CSDStruct;            /*!< CSD structure */
	__IO CPU_INT08U  SysSpecVersion;       /*!< System specification version */
	__IO CPU_INT08U  Reserved1;            /*!< Reserved */
	__IO CPU_INT08U  TAAC;                 /*!< Data read access-time 1 */
	__IO CPU_INT08U  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	__IO CPU_INT08U  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	__IO uint16_t CardComdClasses;         /*!< Card command classes */
	__IO CPU_INT08U  RdBlockLen;           /*!< Max. read data block length */
	__IO CPU_INT08U  PartBlockRead;        /*!< Partial blocks for read allowed */
	__IO CPU_INT08U  WrBlockMisalign;      /*!< Write block misalignment */
	__IO CPU_INT08U  RdBlockMisalign;      /*!< Read block misalignment */
	__IO CPU_INT08U  DSRImpl;              /*!< DSR implemented */
	__IO CPU_INT08U  Reserved2;            /*!< Reserved */
	__IO CPU_INT32U DeviceSize;            /*!< Device Size */
	__IO CPU_INT08U  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	__IO CPU_INT08U  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	__IO CPU_INT08U  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	__IO CPU_INT08U  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	__IO CPU_INT08U  DeviceSizeMul;        /*!< Device size multiplier */
	__IO CPU_INT08U  EraseGrSize;          /*!< Erase group size */
	__IO CPU_INT08U  EraseGrMul;           /*!< Erase group size multiplier */
	__IO CPU_INT08U  WrProtectGrSize;      /*!< Write protect group size */
	__IO CPU_INT08U  WrProtectGrEnable;    /*!< Write protect group enable */
	__IO CPU_INT08U  ManDeflECC;           /*!< Manufacturer default ECC */
	__IO CPU_INT08U  WrSpeedFact;          /*!< Write speed factor */
	__IO CPU_INT08U  MaxWrBlockLen;        /*!< Max. write data block length */
	__IO CPU_INT08U  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	__IO CPU_INT08U  Reserved3;            /*!< Reserded */
	__IO CPU_INT08U  ContentProtectAppli;  /*!< Content protection application */
	__IO CPU_INT08U  FileFormatGrouop;     /*!< File format group */
	__IO CPU_INT08U  CopyFlag;             /*!< Copy flag (OTP) */
	__IO CPU_INT08U  PermWrProtect;        /*!< Permanent write protection */
	__IO CPU_INT08U  TempWrProtect;        /*!< Temporary write protection */
	__IO CPU_INT08U  FileFormat;           /*!< File Format */
	__IO CPU_INT08U  ECC;                  /*!< ECC code */
	__IO CPU_INT08U  CSD_CRC;              /*!< CSD CRC */
	__IO CPU_INT08U  Reserved4;            /*!< always 1*/
}SD_CSD;

//CID寄存器结构
typedef struct
{
	__IO CPU_INT08U  ManufacturerID;       /*!< ManufacturerID */
	__IO uint16_t OEM_AppliID;             /*!< OEM/Application ID */
	__IO CPU_INT32U ProdName1;             /*!< Product Name part1 */
	__IO CPU_INT08U  ProdName2;            /*!< Product Name part2*/
	__IO CPU_INT08U  ProdRev;              /*!< Product Revision */
	__IO CPU_INT32U ProdSN;                /*!< Product Serial Number */
	__IO CPU_INT08U  Reserved1;            /*!< Reserved1 */
	__IO uint16_t ManufactDate;            /*!< Manufacturing Date */
	__IO CPU_INT08U  CID_CRC;              /*!< CID CRC */
	__IO CPU_INT08U  Reserved2;            /*!< always 1 */
}SD_CID;

//卡状态寄存器结构定义
typedef struct
{
	__IO CPU_INT08U DAT_BUS_WIDTH;
	__IO CPU_INT08U SECURED_MODE;
	__IO uint16_t SD_CARD_TYPE;
	__IO CPU_INT32U SIZE_OF_PROTECTED_AREA;
	__IO CPU_INT08U SPEED_CLASS;
	__IO CPU_INT08U PERFORMANCE_MOVE;
	__IO CPU_INT08U AU_SIZE;
	__IO uint16_t ERASE_SIZE;
	__IO CPU_INT08U ERASE_TIMEOUT;
	__IO CPU_INT08U ERASE_OFFSET;
}SD_CardStatus;

//卡信息汇总
typedef struct
{
	SD_CSD SD_csd;
	SD_CID SD_cid;
	long long CardCapacity;	        /*!< Card Capacity */
	CPU_INT32U CardBlockSize;	        /*!< Card Block Size */
	uint16_t RCA;
	CPU_INT08U CardType;
}SD_CardInfo;

extern SD_CardInfo SDCardInfo;

/**********************************************************
                   卡命令字节定义
                   定义十进制： 0 ~ 64
**********************************************************/

#define SD_CMD_GO_IDLE_STATE                       ((CPU_INT08U)0)
#define SD_CMD_SEND_OP_COND                        ((CPU_INT08U)1)
#define SD_CMD_ALL_SEND_CID                        ((CPU_INT08U)2)
#define SD_CMD_SET_REL_ADDR                        ((CPU_INT08U)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((CPU_INT08U)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((CPU_INT08U)5)
#define SD_CMD_HS_SWITCH                           ((CPU_INT08U)6)
#define SD_CMD_SEL_DESEL_CARD                      ((CPU_INT08U)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((CPU_INT08U)8)
#define SD_CMD_SEND_CSD                            ((CPU_INT08U)9)
#define SD_CMD_SEND_CID                            ((CPU_INT08U)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((CPU_INT08U)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((CPU_INT08U)12)
#define SD_CMD_SEND_STATUS                         ((CPU_INT08U)13)
#define SD_CMD_HS_BUSTEST_READ                     ((CPU_INT08U)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((CPU_INT08U)15)
#define SD_CMD_SET_BLOCKLEN                        ((CPU_INT08U)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((CPU_INT08U)17)
#define SD_CMD_READ_MULT_BLOCK                     ((CPU_INT08U)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((CPU_INT08U)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((CPU_INT08U)20) /*!< SD Card doesn't support it */
#define SD_CMD_SET_BLOCK_COUNT                     ((CPU_INT08U)23) /*!< SD Card doesn't support it */
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((CPU_INT08U)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((CPU_INT08U)25)
#define SD_CMD_PROG_CID                            ((CPU_INT08U)26) /*!< reserved for manufacturers */
#define SD_CMD_PROG_CSD                            ((CPU_INT08U)27)
#define SD_CMD_SET_WRITE_PROT                      ((CPU_INT08U)28)
#define SD_CMD_CLR_WRITE_PROT                      ((CPU_INT08U)29)
#define SD_CMD_SEND_WRITE_PROT                     ((CPU_INT08U)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((CPU_INT08U)32) /*!< To set the address of the first write
                                                                         block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((CPU_INT08U)33) /*!< To set the address of the last write block of the
                                                                         continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((CPU_INT08U)35) /*!< To set the address of the first write block to be erased.
                                                                         (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((CPU_INT08U)36) /*!< To set the address of the last write block of the
                                                                         continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((CPU_INT08U)38)
#define SD_CMD_FAST_IO                             ((CPU_INT08U)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((CPU_INT08U)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((CPU_INT08U)42)
#define SD_CMD_APP_CMD                             ((CPU_INT08U)55)
#define SD_CMD_GEN_CMD                             ((CPU_INT08U)56)
#define SD_CMD_NO_CMD                              ((CPU_INT08U)64)

//APP命令，先发送CMD55

#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((CPU_INT08U)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((CPU_INT08U)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((CPU_INT08U)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((CPU_INT08U)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((CPU_INT08U)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((CPU_INT08U)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((CPU_INT08U)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((CPU_INT08U)53) /*!< For SD I/O Card only */

//以下命令支持 SD Card

#define SD_CMD_SD_APP_GET_MKB                      ((CPU_INT08U)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((CPU_INT08U)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((CPU_INT08U)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((CPU_INT08U)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((CPU_INT08U)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((CPU_INT08U)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((CPU_INT08U)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((CPU_INT08U)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((CPU_INT08U)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((CPU_INT08U)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((CPU_INT08U)48) /*!< For SD Card only */

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((CPU_INT08U)0x01)
#define SD_NOT_PRESENT                             ((CPU_INT08U)0x00)

/**********************************************************
                     支持卡的类型定义
**********************************************************/

#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((CPU_INT32U)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((CPU_INT32U)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((CPU_INT32U)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((CPU_INT32U)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((CPU_INT32U)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((CPU_INT32U)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((CPU_INT32U)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((CPU_INT32U)0x00000007)

/**********************************************************
                     外部功能函数
**********************************************************/

void SD_DeInit(void);
SD_Error SD_Init(void);
SDTransferState SD_GetStatus(void);
SDCardState SD_GetState(void);
SD_Error SD_PowerON(void);
SD_Error SD_PowerOFF(void);
SD_Error SD_InitializeCards(void);
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo);
SD_Error SD_GetCardStatus(SD_CardStatus *cardstatus);
SD_Error SD_EnableWideBusOperation(CPU_INT32U WideMode);
SD_Error SD_SelectDeselect(CPU_INT32U addr);
SD_Error SD_ReadBlock(CPU_INT08U *readbuff, CPU_INT32U ReadAddr, uint16_t BlockSize);
SD_Error SD_ReadMultiBlocks(CPU_INT08U *readbuff, CPU_INT32U ReadAddr, uint16_t BlockSize, CPU_INT32U NumberOfBlocks);
SD_Error SD_WriteBlock(CPU_INT08U *writebuff, CPU_INT32U WriteAddr, uint16_t BlockSize);
SD_Error SD_WriteMultiBlocks(CPU_INT08U *writebuff, CPU_INT32U WriteAddr, uint16_t BlockSize, CPU_INT32U NumberOfBlocks);
SDTransferState SD_GetTransferState(void);
SD_Error SD_StopTransfer(void);
SD_Error SD_Erase(CPU_INT32U startaddr, CPU_INT32U endaddr);
SD_Error SD_SendStatus(CPU_INT32U *pcardstatus);
SD_Error SD_SendSDStatus(CPU_INT32U *psdstatus);
SD_Error SD_ProcessIRQSrc(void);
SD_Error SD_WaitReadOperation(void);
SD_Error SD_WaitWriteOperation(void);

SD_Error SD_ReadDisk(CPU_INT08U *readbuff, CPU_INT32U sector, uint16_t cnt);
SD_Error SD_WriteDisk(CPU_INT08U *writebuff, CPU_INT32U sector, uint16_t cnt);


#endif