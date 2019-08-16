#ifndef _USART3_INIT_H_	
#define _USART3_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif

#include "stm32f10x.h"
#include "static_init.h"	//串口结构体

#define USART3BaudRate		115200		   			  //定义串口1的波特率	
//接收缓冲区容量
#define USART3_RX_BUFF_SIZEMAX 100
#define USART3_TX_BUFF_SIZEMAX 100



typedef struct                                                          //
{
 unsigned char RX_Flag;                                                 // 
 unsigned char RX_Len;                                                  //  
  char RX_BUFF[USART3_RX_BUFF_SIZEMAX];                     		//第二缓存
}
UsartRxTypeDef3;

extern UsartRxTypeDef3 USARTStructure3;
extern unsigned char USART3_TX_BUFF[USART3_TX_BUFF_SIZEMAX];   

void USART3_Config(void);		  						//串口配置
void USART3_RX_Buffer_Clear(void); 				//清空接收缓冲区
//void TIM4_Config(void);
//void TIM4_NVIC_Init(void);
void USART3_SendByte(u8 Data);							//单字符数据发送
void USART3_SendString(char * Data,u32 Len);		//多字符发送
//void USART3_DMASendData( char * Data,u32 Len);	//DMA多字节发送

	
	

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif



