#include "USART1_init.h"
#include "Config.h"

#include "motion_control.h"

uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_head = 0;
volatile uint8_t rx_buffer_tail = 0;

uint8_t tx_buffer[TX_BUFFER_SIZE];
uint8_t tx_buffer_head = 0;
volatile uint8_t tx_buffer_tail = 0;
 
static unsigned char USART1_TxBuffer[1024];		//串口1DMA发送缓存
/****************************************************************************
* 名	称：void USART1_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART1_GPIO_Init(void)						//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	//配置串口 Tx (PA.09) 为复用推挽输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					//串口发送引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化引脚
    
	// 配置串口 Rx (PA.10) 为浮空输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					//串口接收引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化引脚
}

/****************************************************************************
* 名	称：void USART1_Init(void)
* 功	能：串口初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART1_Init(void)
{		
	USART_InitTypeDef UART_InitStructure;		//串口结构

	//串口分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//串口初始化
	UART_InitStructure.USART_BaudRate            = USART1BaudRate;					//波特率
	UART_InitStructure.USART_WordLength          = USART_WordLength_8b;				//数据位8bit
	UART_InitStructure.USART_StopBits            = USART_StopBits_1;				//停止位个数
	UART_InitStructure.USART_Parity              = USART_Parity_No ;				//不进行奇偶效验
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//RTS和CTS使能(None不使用)
	UART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//发送和接收使能
	USART_Init(USART1, &UART_InitStructure);										//初始化串口
}

/****************************************************************************
* 名	称：void USART1_NVIC_Init(void)
* 功	能：串口中断向量表初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART1_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 						//中断控制器变量

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开串口中断
	NVIC_Init(&NVIC_InitStructure);								//初始化中断向量表
}

/****************************************************************************
* 名	称：void USART1_DMATxd_Init(void)
* 功	能：串口DMA初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART1_DMATxd_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 						//中断控制器变量
	DMA_InitTypeDef DMA_InitStructure;							//DMA结构

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);			//使能DMA1时钟

	//DMA中断向量配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;	//设置DMA中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开中断
	NVIC_Init(&NVIC_InitStructure); 

	//DMA配置
	DMA_DeInit(DMA1_Channel4);  		   									//复位DMA1_Channel4通道为默认值
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE + 4;				//DMA通道外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART1_TxBuffer;			//DMA通道存储器基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						//DMA目的地	(DMA_DIR_PeripheralSRC源)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//当前外设寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//当前存储寄存器增加
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度为字节(8位)
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据宽度为字节(8位)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//正常缓冲模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//DMA通道优先级非常高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA通道未配置存储器到存储器传输
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);							//根据上诉设置初始化DMA
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);    						//开启DMA通道中断
}

///****************************************************************************
//* 名	称：void USART1_RX_Buffer_init(void)
//* 功	能：串口接收初始化
//* 入口参数：无
//* 出口参数：无
//* 说	明：接收缓冲区初始化
//****************************************************************************/
//void USART1_RX_Buffer_init(void)
//{
//	unsigned char i;

//		USARTStructure1.RX_Flag = 0;
//		USARTStructure1.RX_Len = 0;
//		USARTStructure1.RX_TMEP_Len = 0;
//		for(i=0; i<60; i++)
//		{
//			USARTStructure1.RX_BUFF[i] = 0x00;
//			USARTStructure1.RX_TEMP_BUFF[i] = 0x00;
//		}
//}

/****************************************************************************
* 名	称：void USART1_Config(void)
* 功	能：串口设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收					 
****************************************************************************/
void USART1_Config(void)
{
	USART1_Init();				//串口初始化
	USART1_GPIO_Init();			//串口引脚初始化
	USART1_NVIC_Init();			//中断初始化
	USART1_DMATxd_Init();		//DMA发送初始化
//	USART1_RX_Buffer_init();	//接收中断与接收缓冲区绑定

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);				//清接收标志
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				//开启接收中断
	
	USART_Cmd(USART1, ENABLE);  								//使能失能串口外设	

//	USART1_DMASendString("USART 1 Send ok",20);		   			//多字符DMA输出
}


/****************************************************************************
* 名	称：void USART1_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART1_SendByte(u8 Data)		   //单字符数据输出
{
	USART_SendData(USART1, Data);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* 名	称：void USART1_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART1_SendString(u8* Data,u32 Len)		   //多字符输出
{
	u32 i=0;
	for(i=0;i<Len;i++)
  {    
		USART_SendData(USART1, Data[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

/****************************************************************************
* 名	称：void USART1_DMASendString(u8* Data,u32 Len)
* 功	能：DMA方式多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：必须在USART1_DMATxd_Init初始化之后才能使用
			DMA无需CPU发送 和 用CPU发送 会有发送冲突	 
****************************************************************************/
void USART1_DMASendString(char* Data,u32 Len)		   //多字符输出
{
//	if (Connectstate == 1)
//	{
//		send(SOCK_MONITOR,(uint8*)Data,Len);			/*W5500向Server发送数据*/
//	}

	memcpy(USART1_TxBuffer, Data, Len);			   //拷贝数据到发送缓冲区
    DMA1_Channel4->CNDTR = Len;					   //发送字节数量
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); //开启
	DMA_Cmd(DMA1_Channel4, ENABLE);				   //始能DMA通道
}

/****************************************************************************
* 名	称：void USART1_IRQHandler(void)	
* 功	能：中断机制
* 入口参数：无
* 出口参数：无
* 说	明：接收到的数据存入接收缓冲区
	 USART_GetITStatus		检查指定的USART中断发生与否
	 USART_GetFlagStatus	检查指定的USART标志位设置与否
****************************************************************************/
void USART1_IRQHandler(void)
{
	unsigned char USART1_GetChar;
	uint8_t next_head;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)						// 串口接收数据触发中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);							//清空接收中断标志
		USART1_GetChar = USART_ReceiveData(USART1);									//接收到的字符数据

	  switch (USART1_GetChar) 
	  {
//			case 0x0a:
//				
//			break;
			case CMD_STATUS_REPORT: 
				sys.execute |= EXEC_STATUS_REPORT; 
			break; // Set as true
			case CMD_CYCLE_START:   
				sys.execute |= EXEC_CYCLE_START; 
			break; // Set as true
			case CMD_FEED_HOLD:     
				sys.execute |= EXEC_FEED_HOLD; 
			break; // Set as true
			case CMD_RESET:         
				//mc_reset(); 										
				OutputControl(0x0000);		
				Delay_MS(100);
				__set_FAULTMASK(1);   
				NVIC_SystemReset();
			
			break; // Call motion control reset routine.
			default: // Write character to buffer    	
		
			next_head = rx_buffer_head + 1;
			if (next_head == RX_BUFFER_SIZE) { next_head = 0; }
		
			// Write data to buffer unless it is full.
			if (next_head != rx_buffer_tail) 
			{
				rx_buffer[rx_buffer_head] = USART1_GetChar;
				rx_buffer_head = next_head;    
						
	//			#ifdef ENABLE_XONXOFF
	//				if ((get_rx_buffer_count() >= RX_BUFFER_FULL) && flow_ctrl == XON_SENT) {
	//					flow_ctrl = SEND_XOFF;
	//					UCSR0B |=  (1 << UDRIE0); // Force TX
	//				} 
	//			#endif			
			}		
			break;
		}
	}
	else if(USART_GetFlagStatus(USART1, USART_IT_ORE) == SET)		//检测是否有接收溢出
    {
	    USART_ReceiveData(USART1);									//清接收溢出标志，只能用读数据的方式来清溢出标志
    }

	else if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)				//串口发送数据触发中断
	{
		;
	}
}

/****************************************************************************
* 名	称：void DMA1_Channel4_IRQHandler(void)	
* 功	能：DMA中断机制
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4)) //如果发送完成
	{
		USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);    //关闭DMA发送
		DMA_Cmd(DMA1_Channel4, DISABLE);	       			//关闭DMA通道  
	}

	DMA_ClearFlag(DMA1_FLAG_GL4| DMA1_FLAG_TC4 | DMA1_FLAG_HT4 | DMA1_FLAG_TE4);  //清除DMA相关标志
}



uint8_t serial_read()
{
  uint8_t tail = rx_buffer_tail; // Temporary rx_buffer_tail (to optimize for volatile)
  if (rx_buffer_head == tail) {
    return SERIAL_NO_DATA;
  } else {
    uint8_t data = rx_buffer[tail];
    tail++;
    if (tail == RX_BUFFER_SIZE) { tail = 0; }
    rx_buffer_tail = tail;
    
//    #ifdef ENABLE_XONXOFF
//      if ((get_rx_buffer_count() < RX_BUFFER_LOW) && flow_ctrl == XOFF_SENT) { 
//        flow_ctrl = SEND_XON;
//        UCSR0B |=  (1 << UDRIE0); // Force TX
//      }
//    #endif
    
    return data;
  }
}

void serial_reset_read_buffer() 
{
  rx_buffer_tail = rx_buffer_head;

//  #ifdef ENABLE_XONXOFF
//    flow_ctrl = XON_SENT;
//  #endif
}


