#include "USART3_init.h"


UsartRxTypeDef3 USARTStructure3;
unsigned char USART3_TX_BUFF[USART3_TX_BUFF_SIZEMAX];   

/****************************************************************************
* 名	称：void USART3_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_GPIO_Init(void)			//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
//	//串口引脚分配时钟  默认
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	//配置串口 Tx (PB.10) 为复用推挽输出
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					//串口发送引脚
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化引脚
//    
//	// 配置串口 Rx (PB.11) 为浮空输入
//  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					//串口接收引脚
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化引脚
//	
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_AFIO,ENABLE);
	
		GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);//串口重映射
	//配置串口 Tx (PD.8) 为复用推挽输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					//串口发送引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
  	GPIO_Init(GPIOD, &GPIO_InitStructure);						//初始化引脚
    
	// 配置串口 Rx (PD.9) 为浮空输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					//串口接收引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
  	GPIO_Init(GPIOD, &GPIO_InitStructure);						//初始化引脚
	
}

/****************************************************************************
* 名	称：void USART3_Init(void)
* 功	能：串口初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_Init(void)
{		
	USART_InitTypeDef UART_InitStructure;		//串口结构

	//串口分配时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//串口初始化
	UART_InitStructure.USART_BaudRate            = USART3BaudRate;	//波特率
	UART_InitStructure.USART_WordLength          = USART_WordLength_8b;		//数据位8bit
	UART_InitStructure.USART_StopBits            = USART_StopBits_1;		//停止位个数
	UART_InitStructure.USART_Parity              = USART_Parity_No ;		//不进行奇偶效验
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//RTS和CTS使能(None不使用)
	UART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//发送和接收使能
	USART_Init(USART3, &UART_InitStructure);	//初始化串口
}

/****************************************************************************
* 名	称：void USART3_NVIC_Init(void)
* 功	能：串口中断向量表初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 		//中断控制器变量

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;			//设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开串口中断
	NVIC_Init(&NVIC_InitStructure);								//初始化中断向量表
}


/****************************************************************************
* 名	称：void USART3_Config(void)
* 功	能：串口设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收					 
****************************************************************************/
void USART3_Config(void)
{
	USART3_Init();				//串口初始化
	USART3_GPIO_Init();			//串口引脚初始化
	USART3_NVIC_Init();			//中断初始化
	USART3_RX_Buffer_Clear();	//接收中断与接收缓冲区绑定
		
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);				//清接收标志
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);				//开启接收中断
	
	USART_Cmd(USART3, ENABLE);  								//使能失能串口外设	
}

/****************************************************************************
* 名	称：void USART3_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART3_RX_Buffer_Clear(void)	
{

	USARTStructure3.RX_Len = 0;
		
}
/****************************************************************************
* 名	称：void USART3_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART3_SendByte(u8 Data)		   //单字符数据输出
{
	USART_SendData(USART3, Data);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* 名	称：void USART3_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART3_SendString(char * Data,u32 Len)		   //多字符输出
{
	u32 i=0;

	for(i=0;i<Len;i++)
    {    
		USART_SendData(USART3, Data[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}

}

/****************************************************************************
* 名	称：void USART3_IRQHandler(void)	
* 功	能：中断机制
* 入口参数：无
* 出口参数：无
* 说	明：接收到的数据存入接收缓冲区
	USART_GetITStatus		检查指定的USART中断发生与否
	USART_GetFlagStatus	检查指定的USART标志位设置与否
****************************************************************************/
void USART3_IRQHandler(void)
{
	unsigned char i;
	unsigned char USART3_GetChar;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)				            // 串口接收数据触发中断
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);					        //清空接收中断标志			

		USART3_GetChar  = USART_ReceiveData(USART3);                            //接收到的字符数据

		if(USART3_GetChar==0xA)                             		            //接收到LF结束符
		{
            USARTStructure3.RX_BUFF[USARTStructure3.RX_Len] = '\0';
			USARTStructure3.RX_Flag = 1;
		}
		else if(USART3_GetChar==0xD)                        		            //接收到CR结束符
		{
			
		}
        else if(USART3_GetChar==':')
        {
            USARTStructure3.RX_Len = 0;
			USARTStructure3.RX_BUFF[USARTStructure3.RX_Len] = USART3_GetChar;
			USARTStructure3.RX_Len++;
        }
		else
		{
            USARTStructure3.RX_BUFF[USARTStructure3.RX_Len]=USART3_GetChar;
            USARTStructure3.RX_Len++;
            if(USARTStructure3.RX_Len>=100)
            {
                for(i=0;i<100;i++)
                {
                     USARTStructure3.RX_BUFF[i]=0x00;  				    //清空字符串
                }
                USARTStructure3.RX_Len=0;
            }
		}
	 }
	else if(USART_GetFlagStatus(USART3, USART_IT_ORE) == SET)	                //检测是否有接收溢出
	{
		USART_ReceiveData(USART3);											    //清接收溢出标志，只能用读数据的方式来清溢出标志
	}

	else if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)		                //串口发送数据触发中断
	{
		;
	}
}


