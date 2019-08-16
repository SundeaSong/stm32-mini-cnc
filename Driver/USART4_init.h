#ifndef _USART4_INIT_H_	
#define _USART4_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif

#include "Config.h"
#include "stm32f10x.h"
#include "static_init.h"	//串口结构体

#define USART4BaudRate		38400		   			  //定义串口1的波特率	
//接收缓冲区容量
#define USART4_RX_BUFF_SIZEMAX 100
#define USART4_TX_BUFF_SIZEMAX 100

typedef struct                                                          //结构体
{
 unsigned char RX_Flag;                                                 //接收状态		01248
 unsigned char RX_Len;                                                  //接收到得长度	(二进制模式使用)字符模式使用\0计算
          char RX_BUFF[USART4_RX_BUFF_SIZEMAX];                                           //接收到得数据缓存
}UsartRxTypeDef4;

extern UsartRxTypeDef4 USARTStructure4;
extern unsigned char USART4_TX_BUFF[USART4_TX_BUFF_SIZEMAX];   


extern char Rx4_String[60];	  			  //??????? ??lrc???????
extern unsigned char Rx4_Flag;  

void USART4_Config(void);		  						//串口配置
void USART4_RX_Buffer_Clear(void); 				//清空接收缓冲区

void USART4_SendByte(u8 Data);							//单字符数据发送
void USART4_SendString(char * Data,u32 Len);		//多字符发送
void Send_Command(char *str);
void Get_Command(void);

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif



