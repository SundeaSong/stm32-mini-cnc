#ifndef __SPI1_INIT_H
#define __SPI1_INIT_H

#include "stm32f10x.h"

#define WIZ_SCS			    GPIO_Pin_4	// out
#define WIZ_SCLK			GPIO_Pin_5	// out
#define WIZ_MISO			GPIO_Pin_6	// in
#define WIZ_MOSI			GPIO_Pin_7	// out

#define WIZ_RESET		    GPIO_Pin_10 // out

//#define WIZ_SCS			  GPIO_Pin_12	// out//PB12
//#define WIZ_SCLK			GPIO_Pin_13	// out//PB13
//#define WIZ_MISO			GPIO_Pin_14	// in//PB14
//#define WIZ_MOSI			GPIO_Pin_15	// out//PB15

//#define WIZ_RESET		    GPIO_Pin_9 // out  //PD9

void WIZ_SPI_Init(void);
void WIZ_CS(uint8_t val);
uint8_t SPI1_SendByte(uint8_t byte);

#endif

