#ifndef _I2CEEPROM_H_	
#define _I2CEEPROM_H_

#ifdef __cplusplus		   				
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported macro ------------------------------------------------------------*/
#define ADDR_24CXX        0xA0
/*
	512K = 128Bytes*512Page
	Data Address: 0x0000~0xffff
	page:0x0000~0x0079
			 0x0100~0x0199
	
*/

#define SCLH         GPIOB->BSRR = GPIO_Pin_6
#define SCLL         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDAH         GPIOB->BSRR = GPIO_Pin_7
#define SDAL         GPIOB->BRR  = GPIO_Pin_7

#define SCLread      GPIOB->IDR  & GPIO_Pin_6
#define SDAread      GPIOB->IDR  & GPIO_Pin_7

	
#define  W_ENANLE   	GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x00)
#define  W_DISANLE    GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)0x01) 


/* Exported functions ------------------------------------------------------- */

uint8_t TWI_WriteByte(uint8_t SendByte, uint16_t WriteAddress);
uint8_t TWI_ReadByte( uint16_t ReadAddress);

void I2C_EE_Init(void);
uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data,uint16_t adr,uint8_t nbyte);
uint8_t I2C_EE_BufferRead(uint8_t *pdin_data,uint16_t adr,uint8_t nbyte);
		
unsigned int ReadEpData(unsigned int ads);
void SaveEpData(unsigned int ads,unsigned post);

#ifdef __cplusplus		   	
}

#endif

#endif


