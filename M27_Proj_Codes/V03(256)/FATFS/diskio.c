/**********************************************************
* @ File name -> diskio.c
* @ Version   -> V1.0
* @ Date      -> 02-11-2014
* @ Brief     -> fatfs???t?�̨�3��?IO�̡�2??y?��o����y
**********************************************************/

#include "diskio.h"

/**********************************************************
                     ��a2?o����y����???t                        
**********************************************************/

#include "bsp_MicroSD.h"
//#include "spisdcard.h"
//#include "w25q64.h"

#include  <lib_mem.h>
#include "sys.h"

/**********************************************************
                       ��a1����??��o��?����?
**********************************************************/

#define SD_CARD					0	//��a1����?SD?��
#define EX_FLASH				1	//��a1����?flash

/**********************************************************
                       ?����?����???o��?�䨮D?
**********************************************************/

#define FLASH_SECTOR_SIZE		512	//����??�䨮D??a512byte

#define FLASH_BLOCK_SIZE		8	//?????����D8??����??

u16 FLASH_SECTOR_COUNT=2048*6;	//6M��??��

/**********************************************************
* o����y1|?�� ---> ��a1����??��3?��??��
* ��??��2?��y ---> pdrv��o��a1����??��o?
* ����??��y?�� ---> 3?��??��3��1|��?��?����??
* 1|?��?��?�� ---> none
**********************************************************/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DRESULT res = RES_OK;
	u8 errorstatus;

	switch(pdrv)
	{
		case SD_CARD:	//��a1����?��?SD?��

				errorstatus = SD_Init();

				/* ����???����|D?o??aFatFs?�� */
//				if(errorstatus == 0)	res = RES_OK;
				if(errorstatus == SD_OK)	res = RES_OK;
				else	res = RES_NOTRDY;
				
//				res = RES_OK;
				 
				break;
				
		case EX_FLASH:	//��a1����?��?FLASH

//				FlashSPI_CS_Init();

//				if(SPI_Flash_ID == W25Q64)	FLASH_SECTOR_COUNT = 2048*6;
//				else	FLASH_SECTOR_COUNT = 2048*2;

				res = RES_OK;

				break;

		default:
				res = RES_ERROR;	//������??�̡䨪?������??
				break;
	}

	return res;
}
/**********************************************************
* o����y1|?�� ---> ??��?��??�����䨬?
* ��??��2?��y ---> pdrv��o��a1����??��o?
* ����??��y?�� ---> 3��1|��?��?����??
* 1|?��?��?�� ---> none
**********************************************************/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}
/**********************************************************
* o����y1|?�� ---> ?����?��??��
* ��??��2?��y ---> pdrv��o��a1����??��o?
*               *buff��o?����?��y?Y?o��?
*               sector��o����???e��?��??��
*               count��o?����?����??��y��?
* ����??��y?�� ---> 3��1|��?��?����??
* 1|?��?��?�� ---> none
**********************************************************/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res = RES_OK;
	u8 errorstatus;

	if(!count)	return RES_PARERR;	//?����?��?��?0??����??��??����Y?������??�䨪?��

	switch(pdrv)
	{
		case SD_CARD:	//��a1����?��?SD?��

				errorstatus = SD_ReadDisk((u8*)buff, sector, count);	//?����?��??��
				//��a??��|��e?��?aFatFs?��
				if(errorstatus == SD_OK)	res = RES_OK;
				else	res = RES_ERROR; 

				break;
				
		case EX_FLASH:	//��a1����?��?FLASH

//				for(;count != 0;count--)
//				{
//					W25Qxx_Flash_ReadData(sector*FLASH_SECTOR_SIZE, buff, FLASH_SECTOR_SIZE);
//					sector++;
//					buff += FLASH_SECTOR_SIZE;
//				}
				res = RES_OK;

				break;

		default:
				res = RES_ERROR;
				break;
	}

	return res;
}
/**********************************************************
* o����y1|?�� ---> D�䨨?��??��
* ��??��2?��y ---> pdrv��o��a1����??��o?
*               *buff��o?����?��y?Y?o��?
*               sector��o����???e��?��??��
*               count��oD�䨦��??��y��?
* ����??��y?�� ---> 3��1|��?��?����??
* 1|?��?��?�� ---> none
**********************************************************/

/* read only */

#if _FS_READONLY==0	//?��3?D��2������

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = RES_OK;
	u8 errorstatus;
	u8 retry=0x1f;		//D�䨨?����㨹��?����o��,??��?��?��y

	if(!count)	return RES_PARERR;	//D�䨨?��?��?0??����??��??����Y?������??�䨪?��

	switch(pdrv)
	{
		case SD_CARD:	//��a1����?��?SD?��
				while(retry)
				{
					errorstatus = SD_WriteDisk((u8*)buff, sector, count);	//D�䨨?��??��
					//��a??��|��e?��?aFatFs?��	
					if(errorstatus == SD_OK)	{	res = RES_OK;break;	}
					retry--;
				}
				if(retry == 0)	res = RES_ERROR;	//3?����
				
				break;
				
		case EX_FLASH:	//��a1����?��?FLASH

//				for(;count != 0;count--)
//				{
//					W25Qxx_Flash_WriteData(sector*FLASH_SECTOR_SIZE, (u8*)buff, FLASH_SECTOR_SIZE);
//					sector++;
//					buff += FLASH_SECTOR_SIZE;
//				}
				res = RES_OK;

				break;

		default:
				res = RES_ERROR;
				break;
	}

	return res;
}
/**********************************************************
* o����y1|?�� ---> FatFs??��?����??D??���????��?�꨺???2?D����a��?o����y
* ��??��2?��y ---> none
* ����??��y?�� ---> ?����?��?��?32bit����??D??��
* 1|?��?��?�� ---> 31 ~ 25��oyear(0 ~ 127 org.1980)
*               24 ~ 21��omonth(1 ~ 12)
*               20 ~ 16: date(1 ~ 31)
*               15 ~ 11: hour(00 ~ 23)
*               10 ~ 5:  minute(00 ~ 59)
*               4  ~ 0:  second(00 ~ 59)
**********************************************************/
DWORD get_fattime(void)
{
	DWORD time_buff=0;

//	PCF8563_ReadWrite_Time(1);	//??��?����??D??��
//
//	//??����??��?����????��a??3��BCD??
//	TimeValue.year = HX_to_DX(TimeValue.year);		//?��
//	TimeValue.month = HEX_to_BCD(TimeValue.month);	//??
//	TimeValue.date = HEX_to_BCD(TimeValue.date);	//��?
//	TimeValue.hour = HEX_to_BCD(TimeValue.hour);	//����
//	TimeValue.minute = HEX_to_BCD(TimeValue.minute);//��?
//	TimeValue.second = HEX_to_BCD(TimeValue.second);//??
//
//	//���??FatFs��?����????��?����o?
//	time_buff |= ((TimeValue.year - 1980)<<25);	//?��
//	time_buff |= (TimeValue.month<<21);			//??
//	time_buff |= (TimeValue.date<<16);			//��?
//	
//	time_buff |= (TimeValue.hour<<11);			//���� 
//	time_buff |= (TimeValue.minute<<5);			//��?
//	time_buff |= (TimeValue.second/2);			//??

	return time_buff;
}
				 
#endif	/* end readonly */

/**********************************************************
* o����y1|?�� ---> ??��?????2?��y
* ��??��2?��y ---> pdrv��o��a1����??��o?
*               cmd��o?????����?
*               *buff��o????��y?Y?o��?
* ����??��y?�� ---> 3��1|��?��?����??
* 1|?��?��?�� ---> none
**********************************************************/
#if _USE_IOCTL

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;

	if(pdrv == SD_CARD)	//????��a1����?SD?��
	{
		switch(cmd)
		{
			case CTRL_SYNC:	//?����??����?���䨬?

					res = RES_OK;

					break;

//			case GET_SECTOR_SIZE:	//??��?����??�䨮D?
//
//					res = RES_OK;
//					
//					break;

			case GET_BLOCK_SIZE:	//??��??���䨮D?

					*(WORD*)buff = SDCardInfo.CardBlockSize;	//512
					res = RES_OK;

					break;

			case GET_SECTOR_COUNT:	//����??������y

					*(WORD*)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
					res = RES_OK;

					break;

			default:
					res = RES_PARERR;

					break;
		}	//end switch
	}	//end SD card

	else if(pdrv == EX_FLASH)	//????��a1����?flash
	{
		switch(cmd)	//?????����?????
		{
			case CTRL_SYNC:

					res = RES_OK;

					break;

			case GET_SECTOR_SIZE:	//??��?����??�䨮D?

					*(WORD*)buff = FLASH_SECTOR_SIZE;
					res = RES_OK;

					break;

			case GET_BLOCK_SIZE:	//??��??���䨮D?

					*(WORD*)buff = FLASH_BLOCK_SIZE;
					res = RES_OK;
	
					break;

			case GET_SECTOR_COUNT:	//??��?����??�䨮D?

					*(WORD*)buff = FLASH_SECTOR_COUNT;
					res = RES_OK;
	
					break;

			default:
					res = RES_PARERR;
					break;
		}	//end switch
	}	//end EX FLASH

	else	res = RES_ERROR;	//????��?2??��3?

	return res;
}

#endif	/* end ioctr */

/**********************************************************
* o����y1|?�� ---> ?����?��????����?
* ��??��2?��y ---> size��o��????����?�䨮D?
* ����??��y?�� ---> none
* 1|?��?��?�� ---> none
**********************************************************/
void *ff_memalloc (UINT size)			
{
          
	//return  Mem_HeapAlloc(size,4,);
}
/**********************************************************
* o����y1|?�� ---> ������??����?
* ��??��2?��y ---> mf��o��a������?��?��??��????
* ����??��y?�� ---> none
* 1|?��?��?�� ---> none
**********************************************************/
void ff_memfree (void* mf)		 
{
	//myfree(SRAMIN,mf);
}