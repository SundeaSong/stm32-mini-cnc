#ifndef _USART5_INIT_H_	
#define _USART5_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif

#include "stm32f10x.h"
#include "static_init.h"	//串口结构体

#define USART5BaudRate		19200		   			  //定义串口1的波特率	
//接收缓冲区容量
#define USART5_RX_BUFF_SIZEMAX 100
#define USART5_TX_BUFF_SIZEMAX 100



typedef struct                                                          //
{
 unsigned char RX_Flag;                                                 // 
 unsigned char RX_Len;                                                  //  
  char RX_BUFF[USART5_RX_BUFF_SIZEMAX];                     		//第二缓存
}
UsartRxTypeDef5;

extern UsartRxTypeDef5 USARTStructure5;
extern unsigned char USART5_TX_BUFF[USART5_TX_BUFF_SIZEMAX];   

void USART5_Config(void);		  						//串口配置
void USART5_RX_Buffer_Clear(void); 				//清空接收缓冲区
//void TIM3_Config(void);
//void TIM3_NVIC_Init(void);
void USART5_SendByte(u8 Data);							//单字符数据发送
void USART5_SendString(char * Data,u32 Len);		//多字符发送
//void USART5_DMASendData( char * Data,u32 Len);	//DMA多字节发送

	
	

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif



