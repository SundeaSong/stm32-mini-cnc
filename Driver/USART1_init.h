#ifndef _USART1_INIT_H_	
#define _USART1_INIT_H_

#ifdef __cplusplus									  //定义对CPP进行C处理
extern "C" {
#endif

#include "Config.h"

#define USART1BaudRate		115200		   			  													//定义串口1的波特率

	/*************************************///Grbl_serial
#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 64
#endif

#define SERIAL_NO_DATA 0xff
	
extern uint8_t rx_buffer[RX_BUFFER_SIZE];
extern uint8_t rx_buffer_head;
extern volatile uint8_t rx_buffer_tail;

extern uint8_t tx_buffer[TX_BUFFER_SIZE];
extern uint8_t tx_buffer_head;
extern volatile uint8_t tx_buffer_tail;

void serial_write(uint8_t data);



void USART1_Config(void);		  								//串口配置
//void USART1_RX_Buffer_Clear(void); 						//清空接收缓冲区

void USART1_SendByte(u8 Data);								//单字符数据发送
void USART1_SendString(u8* Data,u32 Len);			//多字符发送
void USART1_DMASendString(char* Data,u32 Len);	//DMA多字符发送

char Transchar(char r);	
uint8_t serial_read(void);
void serial_reset_read_buffer(void);
	
#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif
