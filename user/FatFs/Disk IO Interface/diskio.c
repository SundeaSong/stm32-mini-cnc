#include "diskio.h"
#include "spi_flash.h"
#include "ffconf.h"	
#include "Config.h"


#define SPI_FLASH_ID 	0xC2

DSTATUS SPIFLASH_disk_initialize(void);


DSTATUS SPIFLASH_disk_initialize(void)
{
	DSTATUS res = 0;
	SPI_FLASH_SPI_Init();
	return res;
}


// Inidialize a Drive
DSTATUS disk_initialize(BYTE drv)		/* Physical drive nmuber(0..) */
{
	DSTATUS res;
	if(drv)
		res = STA_NOINIT;
	else
		res = (SPIFLASH_disk_initialize() ? STA_NOINIT : 0);

	return res;
}



DSTATUS disk_status(BYTE drv)			/* Physical drive nmuber(0..) */
{
	return 0;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read(
	BYTE drv,		/* Physical drive nmuber(0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address(LBA) */
	BYTE count		/* Number of sectors to read(1..255) */
)
{	
	UINT i;
	if (SPI_FLASH_ID != SPI_FLASH_Check_Is_OK())
	{
	   return RES_ERROR;	//FR_DISK_ERR
	}

	for(i=0;i<count;i++)
	{
		SPI_FLASH_Read_Sector_512bytes(sector, buff);
		sector ++;
		buff += _MAX_SS;
	}
	
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write(
	BYTE drv,			/* Physical drive nmuber(0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address(LBA) */
	BYTE count			/* Number of sectors to write(1..255) */
)
{
	int i;

	if (SPI_FLASH_ID != SPI_FLASH_Check_Is_OK())
	{
	   return RES_ERROR;	//FR_DISK_ERR
	}
	//写入文件就会对FLASH扇区进行擦写,有点危险，flash读写次数只有10W次
	for(i=0;i<count;i++)
	{	
		SPI_FLASH_Write_Sector_512bytes(sector, (BYTE *)buff);
		sector ++;
		buff += _MAX_SS;
	}
	
	return RES_OK;	
}
#endif /* _READONLY */




/************************************************************************************
CTRL_SYNC			Make sure that the disk drive has finished pending write process. 
					When the disk I/O module has a write back cache, flush the dirty sector immediately. 
					This command is not used in read-only configuration. 
GET_SECTOR_SIZE		Returns sector size of the drive into the WORD variable pointed by Buffer. 
					This command is not used in fixed sector size configuration, _MAX_SS is 512. 
GET_SECTOR_COUNT	Returns number of available sectors on the drive into the DWORD variable pointed by Buffer.
					This command is used by only f_mkfs function to determine the volume size to be created. 
GET_BLOCK_SIZE		Returns erase block size of the flash memory in unit of sector into the DWORD variable pointed by Buffer.
					The allowable value is 1 to 32768 in power of 2. 
					Return 1 if the erase block size is unknown or disk devices. 
					This command is used by only f_mkfs function and it attempts to align data area to the erase block boundary. 
CTRL_ERASE_SECTOR	Erases a part of the flash memory specified by a DWORD array {<start sector>, <end sector>} pointed by Buffer. 
					When this feature is not supported or not a flash memory media, this command has no effect.
					The FatFs does not check the result code and the file function is not affected even if the sectors are not erased well. 
					This command is called on removing a cluster chain when _USE_ERASE is 1. 
************************************************************************************/
DRESULT disk_ioctl(
	BYTE drv,		/* Physical drive nmuber(0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	switch(ctrl)
	{
		case CTRL_SYNC :
			res = RES_OK; 
			break;
	
	
		//扇区大小
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = _MAX_SS;//512;
			res = RES_OK;
			break;
		//总扇区个数
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = SPI_FLASH_SECTOR_COUNT * (SPI_FLASH_SECTOR_SIZE/_MAX_SS);
			res = RES_OK;
			break;	
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}

DWORD get_fattime(void)
{
/*
 t = Time_GetCalendarTime();
    t.tm_year -= 1980;  //年份改为1980年起
    t.tm_mon++;          //0-11月改为1-12月
    t.tm_sec /= 2;       //将秒数改为0-29
    
    date = 0;
    date = (t.tm_year << 25) | (t.tm_mon<<21) | (t.tm_mday<<16)|\
            (t.tm_hour<<11) | (t.tm_min<<5) | (t.tm_sec);

    return date;
*/
	DWORD date;
	    date = ((2015-1980) << 25) | (4<<21) | (2<<16)|\
            (0<<11) | (1<<5) | (10);
	return date;
}

//DWORD get_fattime (void)
//{
//return ((2010UL-1980) &lt;&lt; 25) /* Year = 2010 */
//| (11UL &lt;&lt; 21) /* Month = 11 */
//| (2UL &lt;&lt; 16) /* Day = 2 */
//| (15U &lt;&lt; 11) /* Hour = 15 */
//| (0U &lt;&lt; 5) /* Min = 0 */
//| (0U &gt;&gt; 1) /* Sec = 0 */
//;
//}
