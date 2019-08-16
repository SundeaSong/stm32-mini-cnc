#ifndef __SPI2_INIT_H
#define __SPI2_INIT_H

#include "stm32f10x.h"

//#define WIZ_SCS			    GPIO_Pin_4	// out
//#define WIZ_SCLK			GPIO_Pin_5	// out
//#define WIZ_MISO			GPIO_Pin_6	// in
//#define WIZ_MOSI			GPIO_Pin_7	// out

//#define WIZ_RESET		    GPIO_Pin_10 // out

#define SPI_FLASH_SCS			  GPIO_Pin_12	// out//PB12
#define SPI_FLASH_SCLK			GPIO_Pin_13	// out//PB13
#define SPI_FLASH_MISO			GPIO_Pin_14	// in//PB14
#define SPI_FLASH_MOSI			GPIO_Pin_15	// out//PB15

#define SPI_FLASH_WP		    GPIO_Pin_8 // out  //PD8

void SPI_FLASH_SPI_Init(void);
void SPI_FLASH_CE(uint8_t val);
uint8_t SPI2_SendByte(uint8_t byte);

#endif

