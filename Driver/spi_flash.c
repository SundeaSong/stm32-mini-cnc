
#include "spi_flash.h"
#include "SPI2_Init.h"

//static uint8_t SPI_FLASH_SendByte(uint8_t byte);
static uint32_t SPI_FLASH_ReadBusy(void);


/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint32_t SPI_FLASH_ReadID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CE(LOW);

  /* Send "RDID " instruction */
  SPI2_SendByte(JEDEC_ID);

  /* Read a byte from the FLASH */
  Temp0 = SPI2_SendByte(Dummy);

  /* Read a byte from the FLASH */
  Temp1 = SPI2_SendByte(Dummy);

  /* Read a byte from the FLASH */
  Temp2 = SPI2_SendByte(Dummy);

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CE(HIGH);

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

uint8_t	SPI_FLASH_Check_Is_OK(void)
{
	uint8_t Temp = 0;
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(JEDEC_ID);
	Temp = SPI2_SendByte(Dummy);
	SPI_FLASH_CE(HIGH);
	return Temp;
}

uint32_t SPI_FLASH_ReadBusy(void)
{
	uint32_t busy;
	SPI_FLASH_CE(LOW);
	do{
			busy = SPI2_SendByte(ReadStatusREG_1);
		}while((busy&0x01)!= 0);
	
	SPI_FLASH_CE(HIGH);
	return busy;
}
/* To set write enable latch (WEL) which is required before page program, sector erase, chip erase and
   write status register */
void SPI_FLASH_WriteEnable(void)
{
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(WriteEnable);
	SPI_FLASH_CE(HIGH);
	SPI_FLASH_ReadBusy();
}

/* To disable write enable latch */
void SPI_FLASH_WriteDisable(void)
{
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(WriteDisable);
	SPI_FLASH_CE(HIGH);
	SPI_FLASH_ReadBusy();
}


void SPI_FLASH_Chip_ERASE(void)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(ChipErase);
	SPI_FLASH_CE(HIGH);
	SPI_FLASH_ReadBusy();
}




void SPI_FLASH_SECTOR_ERASE(uint32_t sector)
{
	sector *= SPI_FLASH_SECTOR_SIZE;
	SPI_FLASH_WriteEnable();
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(SectorErase_4KB);
	SPI2_SendByte((sector >> 16) & 0xff);
	SPI2_SendByte((sector >> 8) & 0xff);
	SPI2_SendByte(sector & 0xff);
	SPI_FLASH_CE(HIGH);
	SPI_FLASH_ReadBusy();
}



void SPI_FLASH_Read_Sector(uint32_t nSector, uint8_t* pBuffer)
{	
	uint32_t i;
	//扇区号转为地址
	nSector *= SPI_FLASH_SECTOR_SIZE;
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(FastRead);  											
	SPI2_SendByte((nSector>>16) & 0xff);									
	SPI2_SendByte((nSector>>8) & 0xff);
	SPI2_SendByte(nSector & 0xff); 
	SPI2_SendByte(Dummy); 	
	for(i=0;i<SPI_FLASH_SECTOR_SIZE;i++)
	{	
		pBuffer[i] = SPI2_SendByte(0xff);
	}
	SPI_FLASH_CE(HIGH);
	SPI_FLASH_ReadBusy();
}

void SPI_FLASH_Write_Sector(uint32_t nSector, uint8_t* pBuffer)
{	
	uint32_t i,j;
	//扇区号转为地址
	nSector *= SPI_FLASH_SECTOR_SIZE;		
	//一个扇区需要几个页
	for(j=0;j<FLASH_PAGES_PER_SECTOR;j++)
	{
		SPI_FLASH_WriteEnable();
		SPI_FLASH_CE(LOW);
		SPI2_SendByte(PageProgram);  											//page program 命令
		SPI2_SendByte((nSector>>16) & 0xff);									//page 地址
		SPI2_SendByte((nSector>>8) & 0xff);
		SPI2_SendByte(nSector & 0xff); 
		for(i=0; i<SPI_FLASH_PAGE_SIZE; i++)
		{
		 	SPI2_SendByte(pBuffer[i]);
		}
		pBuffer += SPI_FLASH_PAGE_SIZE;
		nSector += SPI_FLASH_PAGE_SIZE;
		SPI_FLASH_CE(HIGH);
		SPI_FLASH_ReadBusy();
	}
}





static uint8_t SPI_FLASH_BUF[4096];
		   
// WriteAddr = n*512, 	pBuffer[512]
void SPI_FLASH_Write_Sector_512bytes(uint32_t nSector_512, uint8_t* pBuffer)   
{ 
	uint32_t Sector4096;
	uint32_t SectorOffs;  
 	uint32_t i;    
	
	Sector4096 = (nSector_512 *512)/4096; 				//扇区地址,在哪个扇区范围内
	SectorOffs = nSector_512 *512;
	SectorOffs = SectorOffs%4096; 						//在扇区内的偏移

	SPI_FLASH_Read_Sector(Sector4096, SPI_FLASH_BUF);	//读出整个扇区的内容

	for(i=0;i<512;i++)	   								//此扇区是否可以直接写入数据
	{
	 	if (0xff != SPI_FLASH_BUF[SectorOffs+i])
		{
			SPI_FLASH_SECTOR_ERASE(Sector4096);			//不可以,擦除这个扇区
			break;		   
		}
	}

	for(i=0;i<512;i++)	   	//复制
	{
		SPI_FLASH_BUF[SectorOffs+i] = pBuffer[i];	  
	}
	SPI_FLASH_Write_Sector(Sector4096, SPI_FLASH_BUF);	 
	//测试
//	SPI_FLASH_Read_Sector(Sector4096, SPI_FLASH_BUF);	//读出整个扇区的内容
//	for(i=0;i<512;i++)	   	//对比
//	{
//	 	if (pBuffer[i] != SPI_FLASH_BUF[SectorOffs+i])
//		{
//			for(;;);
//			break;
//		}
//	}		 
}

// WriteAddr = n*512, 	pBuffer[512]
void SPI_FLASH_Read_Sector_512bytes(uint32_t nSector_512, uint8_t* pBuffer)  
{
	uint32_t Readaddr; 
 	uint32_t i;  
	Readaddr = nSector_512*512;
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(FastRead);  											
	SPI2_SendByte((Readaddr>>16) & 0xff);									
	SPI2_SendByte((Readaddr>>8) & 0xff);
	SPI2_SendByte(Readaddr & 0xff); 
	SPI2_SendByte(Dummy); 	
	for(i=0;i<512;i++)
	{	
		pBuffer[i] = SPI2_SendByte(0xff);
	}
	SPI_FLASH_CE(HIGH);
}


////////////////////USB u盘//////////////////////////////////////////


int32_t SPIFLASH_disk_initialize_USB(void)
{
	SPI_FLASH_SPI_Init();
	return 0;
}
		   
// WriteAddr = n*512, 	pBuffer[512]
void SPI_FLASH_Write_Sector_512bytes_USB(uint32_t nSector_512, uint8_t* pBuffer, uint16_t Transfer_Length)   
{ 
	uint32_t Sector4096;
	uint32_t SectorOffs;  
 	uint32_t i;    
	//Sector4096 = (nSector_512 * 512)/4096; 				//扇区地址,在哪个扇区范围内
	//SectorOffs = nSector_512 *512;
	//SectorOffs = SectorOffs%4096; 						//在扇区内的偏移

	Sector4096 = nSector_512/4096;
	SectorOffs = nSector_512%4096;

	SPI_FLASH_Read_Sector(Sector4096, SPI_FLASH_BUF);		//读出整个扇区的内容
	SPI_FLASH_SECTOR_ERASE(Sector4096);						//不可以,擦除这个扇区

	for(i=0;i<Transfer_Length;i++)	   	//复制
	{
		SPI_FLASH_BUF[SectorOffs+i] = pBuffer[i];	  
	}
	SPI_FLASH_Write_Sector(Sector4096, SPI_FLASH_BUF);	 
	
//	//check
//	SPI_FLASH_Read_Sector(Sector4096, SPI_FLASH_BUF);		//读出整个扇区的内容
//	for(i=0;i<Transfer_Length;i++)	   	//复制
//	{
//		if (SPI_FLASH_BUF[SectorOffs+i] != pBuffer[i])	
//			for(;;);  
//	}	 
}

// WriteAddr = n*512, 	pBuffer[512]
void SPI_FLASH_Read_Sector_512bytes_USB(uint32_t nSector_512, uint8_t* pBuffer, uint16_t Transfer_Length)  
{
	uint32_t Readaddr; 
 	uint32_t i;  
	Readaddr = nSector_512;//nSector_512*512;
	SPI_FLASH_CE(LOW);
	SPI2_SendByte(FastRead);  											
	SPI2_SendByte((Readaddr>>16) & 0xff);									
	SPI2_SendByte((Readaddr>>8) & 0xff);
	SPI2_SendByte(Readaddr & 0xff); 
	SPI2_SendByte(Dummy); 	
	for(i=0;i<Transfer_Length;i++)
	{	
		pBuffer[i] = SPI2_SendByte(0xff);
	}
	SPI_FLASH_CE(HIGH);
}

