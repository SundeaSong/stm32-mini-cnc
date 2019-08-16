#include "printf_init.h"

//Printf输出口
static USART_TypeDef* USARTNow = (USART_TypeDef*)0;	//当前的输出串口指针

/****************************************************************************
* 名	称：u8 Get_PrintfPort(void)
* 功	能：获取打印输出端口
* 入口参数：无
* 出口参数：u8 当前打印输出端口号	0获取失败
* 说	明：无
****************************************************************************/
u8 Get_PrintfPort(void)
{
	if(USARTNow == ((USART_TypeDef *) USART1_BASE))
		return 1;
	else if(USARTNow == ((USART_TypeDef *) USART2_BASE))
		return 2;
	else if(USARTNow == ((USART_TypeDef *) USART3_BASE))
		return 3;
	else if(USARTNow == ((USART_TypeDef *) UART4_BASE))
		return 4;
	else if(USARTNow == ((USART_TypeDef *) UART5_BASE))
		return 5;
	else
		return 0;
}

/****************************************************************************
* 名	称：void Set_PrintfPort(u8 Port)
* 功	能：设置打印输出端口
* 入口参数：u8	打印显示端口号 串口1 = 1	串口2 = 2 .....
* 出口参数：无
* 说	明：无
****************************************************************************/
void Set_PrintfPort(u8 Port)
{
	switch(Port)
	{
		case 1:
		{
			USARTNow = ((USART_TypeDef *)USART1_BASE);
			break;
		}
		case 2:
		{
			USARTNow = ((USART_TypeDef *)USART2_BASE);
			break;
		}
		case 3:
		{
			USARTNow = ((USART_TypeDef *)USART3_BASE);
			break;
		}
		case 4:
		{
			USARTNow = ((USART_TypeDef *)UART4_BASE);
			break;
		}
		case 5:
		{
			USARTNow = ((USART_TypeDef *)UART5_BASE);
			break;
		}
		default:
		{
			USARTNow = ((USART_TypeDef *)USART1_BASE);
			break;
		}
	}
}

#pragma import(__use_no_semihosting)
                           
struct __FILE 
{ 
	int handle; 
};
 
FILE __stdout;
         
_sys_exit(int x) 
{ 
	x = x; 
}

/****************************************************************************
* 名	称：int fputc(int ch, FILE *f)
* 功	能：重定义fputc函数，printf函数从终端打印输出
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
int fputc(int ch, FILE *f)
{   
		while (USART_GetFlagStatus(USARTNow,USART_FLAG_TXE)==RESET)	//等待发送完毕,对一个字符不能用TC来作发送判断
		{}   
		USARTNow->DR = (u8) ch;      
		while((USARTNow->SR&USART_FLAG_TC)==0);   
			
		return ch;
}

/****************************************************************************
* 名	称：int fgetc(FILE *f)
* 功	能：重定义fgetc函数，printf函数从终端输入数据
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USARTNow, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}

