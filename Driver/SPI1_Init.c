
#include "config.h"

void WIZ_SPI_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);

/* Configure SPI1 pins: SCK, MISO and MOSI -------------*/
// SPI 1
/* Configure SPIy pins: SCK, MISO and MOSI */

	GPIO_InitStructure.GPIO_Pin = WIZ_SCLK | WIZ_MISO |WIZ_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = WIZ_MISO ;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = WIZ_SCS; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, WIZ_SCS);										//低
		  		
	// Port B output;	PB12
	GPIO_InitStructure.GPIO_Pin = WIZ_RESET; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_SetBits(GPIOG, WIZ_RESET);

		/* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial=7;

	SPI_Init(SPI1,&SPI_InitStructure);

	SPI_Cmd(SPI1,ENABLE);					//Enable  SPI2

}

// Connected to Data Flash
void WIZ_CS(uint8_t val)
{
	if (val == LOW) 
	{
   		GPIO_ResetBits(GPIOA, WIZ_SCS); 	 //低电平
	}
	else if (val == HIGH)
	{
   		GPIO_SetBits(GPIOA, WIZ_SCS); 		 //高电平
	}
}


uint8_t SPI1_SendByte(uint8_t byte)
{
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
         
	  SPI_I2S_SendData(SPI1, byte);
          
	  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
          
	  return SPI_I2S_ReceiveData(SPI1);
}


