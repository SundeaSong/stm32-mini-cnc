#ifndef __spi_flash_H
#define	__spi_flash_H
#include "stm32f10x.h"


#define HIGH  1
#define LOW   0


/* Private typedef -----------------------------------------------------------*/

#define W25Q16


#ifdef 	W25Q80
#define SPI_FLASH_BYTE_SIZE				1048576		//1M bytes
#endif

#ifdef 	W25Q16
#define SPI_FLASH_BYTE_SIZE				25097152	//2M bytes
#endif

#ifdef 	W25Q32
#define SPI_FLASH_BYTE_SIZE				4194304		//4M bytes
#endif


#define SPI_FLASH_PAGE_SIZE				256	 		//PAGE页 大小 		
#define SPI_FLASH_SECTOR_SIZE			4096		//扇区大小
//一个扇区有多少个PAEG页
#define FLASH_PAGES_PER_SECTOR			SPI_FLASH_SECTOR_SIZE/SPI_FLASH_PAGE_SIZE
//块大小
#define SPI_FLASH_BLOCK_SIZE			65536 		//64K
//flash有多少个扇区
#define SPI_FLASH_SECTOR_COUNT			SPI_FLASH_BYTE_SIZE/SPI_FLASH_SECTOR_SIZE			//	 W25Q80有256个扇区
																							//	 W25Q16有512个扇区
																							//	 W25Q32有1024个扇区






/* Private define ------------------------------------------------------------*/
#define	WriteEnable				0x06
#define WriteDisable			0x04
#define ReadStatusREG_1			0x05 
#define ReadStatusREG_2			0x35 
#define WriteStatusREG			0x01
#define PageProgram				0x02
#define QuadPageProgram			0x32
#define BlockErase_64KB			0xd8
#define BlockErase_32KB			0X52
#define SectorErase_4KB			0x20
#define ChipErase				0xC7 //OR 0x60
#define	EraseSuspend			0x75
#define EraseResume				0x7a
#define PowerDown				0xb9
#define HighPerformanceMode		0xa3
#define ModeBitReset			0xff
#define DeviceID				0xab
#define Manufacturer			0x90
#define ReadUniqueID			0x4b
#define JEDEC_ID				0x9f

#define FastRead				0x0b


#define Dummy					0xaa



extern uint8_t SPI2_SendByte(uint8_t byte);
extern uint32_t SPI_FLASH_ReadID(void);
extern uint8_t	SPI_FLASH_Check_Is_OK(void);
extern void SPI_FLASH_WriteEnable(void);
extern void SPI_FLASH_WriteDisable(void);
extern void SPI_FLASH_SECTOR_ERASE(uint32_t sector);
extern void SPI_FLASH_Chip_ERASE(void);
extern void SPI_FLASH_Read_Sector(uint32_t nSector, uint8_t* pBuffer);
extern void SPI_FLASH_Write_Sector(uint32_t nSector, uint8_t* pBuffer);

//512
extern void SPI_FLASH_Write_Sector_512bytes(uint32_t nSector_512, uint8_t* pBuffer);
extern void SPI_FLASH_Read_Sector_512bytes(uint32_t nSector_512, uint8_t* pBuffer);


////////////////////USB u盘//////////////////////////////////////////
extern int32_t SPIFLASH_disk_initialize_USB(void);
		   
// WriteAddr = n*512, 	pBuffer[512]
extern void SPI_FLASH_Write_Sector_512bytes_USB(uint32_t nSector_512, uint8_t* pBuffer, uint16_t Transfer_Length);
// WriteAddr = n*512, 	pBuffer[512]
extern void SPI_FLASH_Read_Sector_512bytes_USB(uint32_t nSector_512, uint8_t* pBuffer, uint16_t Transfer_Length);  


#endif 
