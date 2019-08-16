#ifndef _USART2_INIT_H_	
#define _USART2_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif

#include "stm32f10x.h"
#include "static_init.h"	//串口结构体

#define USART2BaudRate		38400		   			  //定义串口1的波特率	
//接收缓冲区容量
#define USART2_RX_BUFF_SIZEMAX 1024
#define USART2_TX_BUFF_SIZEMAX 1024


typedef struct                                                          //
{
 unsigned char RX_Flag;                                                 // 
 unsigned char RX_TMEP_Len;  
 unsigned char RX_Len;                                                  //  
 unsigned char RX_TEMP_BUFF[USART2_RX_BUFF_SIZEMAX];                    //临时接收
 char RX_BUFF[USART2_RX_BUFF_SIZEMAX];                     		//第二缓存
}
UsartRxTypeDef1;

extern UsartRxTypeDef1 USARTStructure1;	   		//串口结构体
extern UsartRxTypeDef1 USARTStructure2;
extern unsigned char USART2_TX_BUFF[USART2_TX_BUFF_SIZEMAX];   

void USART2_Config(void);		  						//串口配置
void USART2_RX_Buffer_Clear(void); 				//清空接收缓冲区
//void TIM3_Config(void);
//void TIM3_NVIC_Init(void);
void USART2_SendByte(u8 Data);							//单字符数据发送
void USART2_SendString(char * Data,u32 Len);		//多字符发送
void USART2_DMASendData(u8* Data,u32 Len);	//DMA多字节发送
void ReturnPCString(char*str);

	
	

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif



