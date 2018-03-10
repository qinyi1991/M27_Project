/**********************************************************
* @ File name -> diskio.c
* @ Version   -> V1.0
* @ Date      -> 02-11-2014
* @ Brief     -> fatfs???t?¦Ì¨ª3¦Ì?IO¦Ì¡Á2??y?¡¥o¡¥¨ºy
**********************************************************/

#include "diskio.h"

/**********************************************************
                     ¨ªa2?o¡¥¨ºy¨ª¡¤???t                        
**********************************************************/

#include "bsp_MicroSD.h"
//#include "spisdcard.h"
//#include "w25q64.h"

#include  <lib_mem.h>
#include "sys.h"

/**********************************************************
                       ¨ªa1¨°¡ä??¨¬o¨º?¡§¨°?
**********************************************************/

#define SD_CARD					0	//¨ªa1¨°¦Ì?SD?¡§
#define EX_FLASH				1	//¨ªa1¨°¦Ì?flash

/**********************************************************
                       ?¡§¨°?¨¦¨¨???o¡ä?¡ä¨®D?
**********************************************************/

#define FLASH_SECTOR_SIZE		512	//¨¦¨¨??¡ä¨®D??a512byte

#define FLASH_BLOCK_SIZE		8	//?????¨¬¨®D8??¨¦¨¨??

u16 FLASH_SECTOR_COUNT=2048*6;	//6M¡Á??¨²

/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ¨ªa1¨°¡ä??¨¬3?¨º??¡¥
* ¨¨??¨²2?¨ºy ---> pdrv¡êo¨ªa1¨°¡ä??¨¬o?
* ¡¤¦Ì??¨ºy?¦Ì ---> 3?¨º??¡¥3¨¦1|¨®?¡¤?¡À¨º??
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DRESULT res = RES_OK;
	u8 errorstatus;

	switch(pdrv)
	{
		case SD_CARD:	//¨ªa1¨°¦Ì?¨º?SD?¡§

				errorstatus = SD_Init();

				/* ¡Á¡ã???¨¬¨®|D?o??aFatFs?¦Ì */
//				if(errorstatus == 0)	res = RES_OK;
				if(errorstatus == SD_OK)	res = RES_OK;
				else	res = RES_NOTRDY;
				
//				res = RES_OK;
				 
				break;
				
		case EX_FLASH:	//¨ªa1¨°¦Ì?¨º?FLASH

//				FlashSPI_CS_Init();

//				if(SPI_Flash_ID == W25Q64)	FLASH_SECTOR_COUNT = 2048*6;
//				else	FLASH_SECTOR_COUNT = 2048*2;

				res = RES_OK;

				break;

		default:
				res = RES_ERROR;	//¨¨¡À¨º??¦Ì¡ä¨ª?¨®¡¤¦Ì??
				break;
	}

	return res;
}
/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ??¨¨?¡ä??¨¬¡Á¡ä¨¬?
* ¨¨??¨²2?¨ºy ---> pdrv¡êo¨ªa1¨°¡ä??¨¬o?
* ¡¤¦Ì??¨ºy?¦Ì ---> 3¨¦1|¨®?¡¤?¡À¨º??
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}
/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ?¨¢¨¨?¡ä??¨¬
* ¨¨??¨²2?¨ºy ---> pdrv¡êo¨ªa1¨°¡ä??¨¬o?
*               *buff¡êo?¨¢¨¨?¨ºy?Y?o¡ä?
*               sector¡êo¨¦¨¨???e¨º?¦Ì??¡¤
*               count¡êo?¨¢¨¨?¨¦¨¨??¨ºy¨¢?
* ¡¤¦Ì??¨ºy?¦Ì ---> 3¨¦1|¨®?¡¤?¡À¨º??
* 1|?¨¹?¦Ì?¡Â ---> none
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

	if(!count)	return RES_PARERR;	//?¨¢¨¨?¦Ì?¨º?0??¨¦¨¨??¦Ì??¨²¨¨Y?¨°¡¤¦Ì??¡ä¨ª?¨®

	switch(pdrv)
	{
		case SD_CARD:	//¨ªa1¨°¦Ì?¨º?SD?¡§

				errorstatus = SD_ReadDisk((u8*)buff, sector, count);	//?¨¢¨¨?¡ä??¨¬
				//¡Áa??¨®|¡äe?¦Ì?aFatFs?¦Ì
				if(errorstatus == SD_OK)	res = RES_OK;
				else	res = RES_ERROR; 

				break;
				
		case EX_FLASH:	//¨ªa1¨°¦Ì?¨º?FLASH

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
* o¡¥¨ºy1|?¨¹ ---> D¡ä¨¨?¡ä??¨¬
* ¨¨??¨²2?¨ºy ---> pdrv¡êo¨ªa1¨°¡ä??¨¬o?
*               *buff¡êo?¨¢¨¨?¨ºy?Y?o¡ä?
*               sector¡êo¨¦¨¨???e¨º?¦Ì??¡¤
*               count¡êoD¡ä¨¦¨¨??¨ºy¨¢?
* ¡¤¦Ì??¨ºy?¦Ì ---> 3¨¦1|¨®?¡¤?¡À¨º??
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/

/* read only */

#if _FS_READONLY==0	//?¡ì3?D¡ä2¨´¡Á¡Â

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res = RES_OK;
	u8 errorstatus;
	u8 retry=0x1f;		//D¡ä¨¨?¨º¡ì¡ã¨¹¦Ì?¨º¡Ào¨°,??¨º?¡ä?¨ºy

	if(!count)	return RES_PARERR;	//D¡ä¨¨?¦Ì?¨º?0??¨¦¨¨??¦Ì??¨²¨¨Y?¨°¡¤¦Ì??¡ä¨ª?¨®

	switch(pdrv)
	{
		case SD_CARD:	//¨ªa1¨°¦Ì?¨º?SD?¡§
				while(retry)
				{
					errorstatus = SD_WriteDisk((u8*)buff, sector, count);	//D¡ä¨¨?¡ä??¨¬
					//¡Áa??¨®|¡äe?¦Ì?aFatFs?¦Ì	
					if(errorstatus == SD_OK)	{	res = RES_OK;break;	}
					retry--;
				}
				if(retry == 0)	res = RES_ERROR;	//3?¨º¡À
				
				break;
				
		case EX_FLASH:	//¨ªa1¨°¦Ì?¨º?FLASH

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
* o¡¥¨ºy1|?¨¹ ---> FatFs??¨¨?¨º¡À??D??¡é¡ê????¨¢?¡ê¨º???2?D¨¨¨°a¡ä?o¡¥¨ºy
* ¨¨??¨²2?¨ºy ---> none
* ¡¤¦Ì??¨ºy?¦Ì ---> ?¨¢¨¨?¦Ì?¦Ì?32bit¨º¡À??D??¡é
* 1|?¨¹?¦Ì?¡Â ---> 31 ~ 25¡êoyear(0 ~ 127 org.1980)
*               24 ~ 21¡êomonth(1 ~ 12)
*               20 ~ 16: date(1 ~ 31)
*               15 ~ 11: hour(00 ~ 23)
*               10 ~ 5:  minute(00 ~ 59)
*               4  ~ 0:  second(00 ~ 59)
**********************************************************/
DWORD get_fattime(void)
{
	DWORD time_buff=0;

//	PCF8563_ReadWrite_Time(1);	//??¨¨?¨º¡À??D??¡é
//
//	//??¨º¡À??¨º?¨¢¨´????¡Áa??3¨¦BCD??
//	TimeValue.year = HX_to_DX(TimeValue.year);		//?¨º
//	TimeValue.month = HEX_to_BCD(TimeValue.month);	//??
//	TimeValue.date = HEX_to_BCD(TimeValue.date);	//¨¨?
//	TimeValue.hour = HEX_to_BCD(TimeValue.hour);	//¨º¡À
//	TimeValue.minute = HEX_to_BCD(TimeValue.minute);//¡¤?
//	TimeValue.second = HEX_to_BCD(TimeValue.second);//??
//
//	//¡ã¡ä??FatFs¦Ì?¨º¡À????¨º?¡Á¨¦o?
//	time_buff |= ((TimeValue.year - 1980)<<25);	//?¨º
//	time_buff |= (TimeValue.month<<21);			//??
//	time_buff |= (TimeValue.date<<16);			//¨¨?
//	
//	time_buff |= (TimeValue.hour<<11);			//¨º¡À 
//	time_buff |= (TimeValue.minute<<5);			//¡¤?
//	time_buff |= (TimeValue.second/2);			//??

	return time_buff;
}
				 
#endif	/* end readonly */

/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ??¨¨?????2?¨ºy
* ¨¨??¨²2?¨ºy ---> pdrv¡êo¨ªa1¨°¡ä??¨¬o?
*               cmd¡êo?????¨¹¨¢?
*               *buff¡êo????¨ºy?Y?o¡ä?
* ¡¤¦Ì??¨ºy?¦Ì ---> 3¨¦1|¨®?¡¤?¡À¨º??
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/
#if _USE_IOCTL

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;

	if(pdrv == SD_CARD)	//????¨ªa1¨°¦Ì?SD?¡§
	{
		switch(cmd)
		{
			case CTRL_SYNC:	//?¨¢¨¨??¡§¦Ì?¡Á¡ä¨¬?

					res = RES_OK;

					break;

//			case GET_SECTOR_SIZE:	//??¨¨?¨¦¨¨??¡ä¨®D?
//
//					res = RES_OK;
//					
//					break;

			case GET_BLOCK_SIZE:	//??¨¨??¨¦¡ä¨®D?

					*(WORD*)buff = SDCardInfo.CardBlockSize;	//512
					res = RES_OK;

					break;

			case GET_SECTOR_COUNT:	//¨¦¨¨??¡Á¨¹¨ºy

					*(WORD*)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
					res = RES_OK;

					break;

			default:
					res = RES_PARERR;

					break;
		}	//end switch
	}	//end SD card

	else if(pdrv == EX_FLASH)	//????¨ªa1¨°¦Ì?flash
	{
		switch(cmd)	//?????¨¹¨¢?????
		{
			case CTRL_SYNC:

					res = RES_OK;

					break;

			case GET_SECTOR_SIZE:	//??¨¨?¨¦¨¨??¡ä¨®D?

					*(WORD*)buff = FLASH_SECTOR_SIZE;
					res = RES_OK;

					break;

			case GET_BLOCK_SIZE:	//??¨¨??¨¦¡ä¨®D?

					*(WORD*)buff = FLASH_BLOCK_SIZE;
					res = RES_OK;
	
					break;

			case GET_SECTOR_COUNT:	//??¨¨?¨¦¨¨??¡ä¨®D?

					*(WORD*)buff = FLASH_SECTOR_COUNT;
					res = RES_OK;
	
					break;

			default:
					res = RES_PARERR;
					break;
		}	//end switch
	}	//end EX FLASH

	else	res = RES_ERROR;	//????¦Ì?2??¡ì3?

	return res;
}

#endif	/* end ioctr */

/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ?¡¥¨¬?¡¤????¨²¡ä?
* ¨¨??¨²2?¨ºy ---> size¡êo¡¤????¨²¡ä?¡ä¨®D?
* ¡¤¦Ì??¨ºy?¦Ì ---> none
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/
void *ff_memalloc (UINT size)			
{
          
	//return  Mem_HeapAlloc(size,4,);
}
/**********************************************************
* o¡¥¨ºy1|?¨¹ ---> ¨º¨ª¡¤??¨²¡ä?
* ¨¨??¨²2?¨ºy ---> mf¡êo¨°a¨º¨ª¡¤?¦Ì?¦Ì??¡¤????
* ¡¤¦Ì??¨ºy?¦Ì ---> none
* 1|?¨¹?¦Ì?¡Â ---> none
**********************************************************/
void ff_memfree (void* mf)		 
{
	//myfree(SRAMIN,mf);
}