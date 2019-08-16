#include "USART5_init.h"




UsartRxTypeDef5 USARTStructure5;
unsigned char USART5_TX_BUFF[USART5_TX_BUFF_SIZEMAX];   

/****************************************************************************
* 名	称：void USART5_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART5_GPIO_Init(void)			//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
	//串口5引脚分配时钟
	//  RCC_APB2PeriphClockCmd(RCC_APB1Periph_UART5|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);

   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);  //??UART5??GPIOC???  
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);  //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC,ENABLE);
	
	//配置串口 Tx (PC.12) 为复用推挽输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;					//串口发送引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
  	GPIO_Init(GPIOC, &GPIO_InitStructure);						//初始化引脚
    
	// 配置串口 Rx (PD.2) 为浮空输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;					//串口接收引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	
  	
	  GPIO_Init(GPIOD, &GPIO_InitStructure);						//初始化引脚，
												 
}

/****************************************************************************
* 名	称：void USART5_Init(void)
* 功	能：串口初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART5_Init(void)
{		
	USART_InitTypeDef UART_InitStructure;		//串口结构

	//串口分配时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	//串口初始化
	UART_InitStructure.USART_BaudRate            = USART5BaudRate;	//波特率
	UART_InitStructure.USART_WordLength          = USART_WordLength_8b;		//数据位8bit
	UART_InitStructure.USART_StopBits            = USART_StopBits_1;		//停止位个数
	UART_InitStructure.USART_Parity              = USART_Parity_No ;		//不进行奇偶效验
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//RTS和CTS使能(None不使用)
	UART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	//发送和接收使能
	USART_Init(UART5, &UART_InitStructure);	//初始化串口
}

/****************************************************************************
* 名	称：void USART5_NVIC_Init(void)
* 功	能：串口中断向量表初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART5_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 		//中断控制器变量

	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;			//设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开串口中断
	NVIC_Init(&NVIC_InitStructure);								//初始化中断向量表
}

///****************************************************************************
//* 名	称：void USART5_DMATxd_Init(void)
//* 功	能：串口DMA初始化
//* 入口参数：无
//* 出口参数：无
//* 说	明：无
//****************************************************************************/
//void USART5_DMATxd_Init(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 		//中断控制器变量
//	DMA_InitTypeDef DMA_InitStructure;			//DMA结构

//	RCC_AHBPeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//使能DMA1时钟

//	//DMA中断向量配置
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;	//设置DMA中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//设置优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开中断
//	NVIC_Init(&NVIC_InitStructure); 

//	//DMA配置
//	DMA_DeInit(DMA1_Channel7);  		   									//复位DMA1_Channel4通道为默认值
//	DMA_InitStructure.DMA_PeripheralBaseAddr = UART5_BASE + 4;				//DMA通道外设基地址
//	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)USART5_TX_BUFF;				//DMA通道存储器基地址
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						//DMA目的地	(DMA_DIR_PeripheralSRC源)
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//当前外设寄存器不变
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//当前存储寄存器增加
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//外设数据宽度为字节(8位)
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//存储器数据宽度为字节(8位)
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//正常缓冲模式
//	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//DMA通道优先级非常高
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA通道未配置存储器到存储器传输
//	DMA_Init(DMA1_Channel7, &DMA_InitStructure);							//根据上诉设置初始化DMA
//	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);    						//开启DMA通道中断
//}

/****************************************************************************
* 名	称：void USART5_Config(void)
* 功	能：串口设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收					 
****************************************************************************/
void USART5_Config(void)
{
	USART5_Init();				//串口初始化
	USART5_GPIO_Init();			//串口引脚初始化
	USART5_NVIC_Init();			//中断初始化
//	USART5_DMATxd_Init();		//DMA发送初始化
	USART5_RX_Buffer_Clear();	//接收中断与接收缓冲区绑定
//	TIM3_Config();
//	TIM3_NVIC_Init();
		
	USART_ClearITPendingBit(UART5, USART_IT_RXNE);				//清接收标志
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);				//开启接收中断
	
	USART_Cmd(UART5, ENABLE);  								//使能失能串口外设	
}


///****************************************************************************
//* 名	称:
//* 功	能：接收超时检测
//* 入口参数：无
//* 出口参数：无
//* 说	明：	
////TIM_TimeBaseStructure.TIM_Prescaler = 7199;//时钟预分频数  
////TIM_TimeBaseStructure.TIM_Period = 9999; // 自动重装载寄存器周期的值(定时时间) 
////((1+TIM_Prescaler )/72M)*(1+TIM_Period )
////((1+7199)/72M)*(1+9999)=72/72M *10000=1 秒
//// Period =9  ,Prescaler = 7199 定时1ms
//// Period =99  ,Prescaler = 7199 定时10ms
//****************************************************************************/
//void TIM3_Config(void)
//{

//	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3
//	
//    TIM_TimeBaseStructure.TIM_Period =49;          		  //设置定时器重载值,计算时应加1
//    TIM_TimeBaseStructure.TIM_Prescaler = 7199;       		  //定时器时钟预分频,计算时应加1  定时5ms
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    		  //时钟分割，用于滤波器
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //选择向上计数模式 
//    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;		  //重载次数计数器，计数到0便产生中断事件
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);			  //以上参数初始化TIM3

//    TIM_ClearFlag(TIM3, TIM_FLAG_Update);					  		//清除更新事件标志位(防止启动便中断)
//    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);				  //打开重载更新中断 

//    TIM_Cmd(TIM3, DISABLE);									  					//使能TIM3	
//}

///****************************************************************************
//* 名	称：
//* 功	能：
//* 入口参数：
//* 出口参数：无
//* 说	明：无				   
//****************************************************************************/
//void TIM3_NVIC_Init(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure; 		//中断控制器变量

//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;			     //选择定时器2中断线
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		 //抢占优先级为1
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;			 //响应优先级为0
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能中断通道
//  NVIC_Init(&NVIC_InitStructure);								 //用以上参数初始化10-15号线中断通道
//}

/****************************************************************************
* 名	称：void USART5_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART5_RX_Buffer_Clear(void)	
{

	USARTStructure5.RX_Len = 0;
		
}
/****************************************************************************
* 名	称：void USART5_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART5_SendByte(u8 Data)		   //单字符数据输出
{
	USART_SendData(UART5, Data);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
}

/****************************************************************************
* 名	称：void USART5_SendString(u8* Data,u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无					 
****************************************************************************/
void USART5_SendString(char * Data,u32 Len)		   //多字符输出
{
	u32 i=0;

	for(i=0;i<Len;i++)
  {    
		USART_SendData(UART5, Data[i]);
        while(USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET);
	}

}

///****************************************************************************
//* 名	称：void USART5_DMASendString(u8* Data,u32 Len)
//* 功	能：DMA方式多字符输出
//* 入口参数：Data 	输出的单字符数据
//			Len		字符个数
//* 出口参数：无
//* 说	明：必须在USART5_DMATxd_Init初始化之后才能使用
//			DMA无需CPU发送 和 用CPU发送 会有发送冲突	 
//****************************************************************************/
//void USART5_DMASendData(char * Data,u32 Len)		   //多字符输出
//{
//	
//	memcpy(USART5_TX_BUFF, Data, Len);			   //拷贝数据到发送缓冲区
//    DMA1_Channel7->CNDTR = Len;					   //发送字节数量
//	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE); //开启
//	DMA_Cmd(DMA1_Channel7, ENABLE);				   //始能DMA通道
//}

/****************************************************************************
* 名	称：void USART5_IRQHandler(void)	
* 功	能：中断机制
* 入口参数：无
* 出口参数：无
* 说	明：接收到的数据存入接收缓冲区
	USART_GetITStatus		检查指定的USART中断发生与否
	USART_GetFlagStatus	检查指定的USART标志位设置与否
****************************************************************************/
void UART5_IRQHandler(void)
{
	unsigned char i;
	unsigned char USART5_GetChar;
	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)				// 串口接收数据触发中断
	{
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);					//清空接收中断标志			//接收到的字符数据

		USART5_GetChar  = USART_ReceiveData(UART5);
//		
//    TIM_Cmd(TIM3, ENABLE);									  					 		//使能TIM4	
//		TIM_SetCounter(TIM3, 0);																//清当前的计数
	
		if(USART5_GetChar==0xA)                             		//接收到LF结束符
		{

		}
		else if(USART5_GetChar==0xD)                        		//接收到CR结束符
		{
			USARTStructure5.RX_BUFF[USARTStructure5.RX_Len] = '\0';
			USARTStructure5.RX_Flag = 1;
//			TIM_Cmd(TIM3, DISABLE);									  						//关闭 TIM4	
		}
		else
		{
				USARTStructure5.RX_BUFF[USARTStructure5.RX_Len]=USART5_GetChar;
				USARTStructure5.RX_Len++;
				if(USARTStructure5.RX_Len>=100)
				{
						for(i=0;i<100;i++)
						{
							 USARTStructure5.RX_BUFF[i]=0x00;  						//清空字符串
						}
						USARTStructure5.RX_Len=0;
				}
		}
	 }

	else if(USART_GetFlagStatus(UART5, USART_IT_ORE) == SET)	//检测是否有接收溢出
	{
		USART_ReceiveData(UART5);															//清接收溢出标志，只能用读数据的方式来清溢出标志
	}

	else if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)		//串口发送数据触发中断
	{
		
	}
}
///*******************************************************************************
//* Function Name  : TIM3_IRQHandler	 定时5ms
//* Description    : This function handles TIM3 global interrupt request.
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void TIM3_IRQHandler(void)
//{
//  /* 清除中断标志位 */
////	unsigned int rlen;
//		
//  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//	
////	for(rlen = 0 ;rlen<USARTStructure2.RX_TMEP_Len; rlen++ )
////	{
////			USARTStructure2.RX_BUFF[rlen]=USARTStructure2.RX_TEMP_BUFF[rlen];
////	}
////	USARTStructure2.RX_Len = USARTStructure2.RX_TMEP_Len;
////	USARTStructure2.RX_TMEP_Len = 0;
////	USARTStructure3.RX_BUFF[USARTStructure3.RX_Len] = '\0';
////	USARTStructure3.RX_Flag=1; 
//	USARTStructure5.RX_Len = 0;
//	
//	TIM_Cmd(TIM3, DISABLE);									  			//关闭 TIM3	

//}

/****************************************************************************
* 名	称：void DMA1_Channel2_IRQHandler(void)	
* 功	能：DMA中断机制
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
//void DMA1_Channel2_IRQHandler(void)
//{
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC2)) //如果发送完成
//	{
//		USART_DMACmd(UART5, USART_DMAReq_Tx, DISABLE);    //关闭DMA发送
//		DMA_Cmd(DMA1_Channel2, DISABLE);	       			//关闭DMA通道  	

//	}

//	DMA_ClearFlag(DMA1_FLAG_GL2| DMA1_FLAG_TC2 | DMA1_FLAG_HT2 | DMA1_FLAG_TE2);  //清除DMA相关标志
//}
///****************************************************************************
//* ?	?:void DMA1_Channel7_IRQHandler(void)	
//* ?	?:DMA????
//* ????:?
//* ????:?
//* ?	?:?
//****************************************************************************/
//void DMA1_Channel7_IRQHandler(void)
//{
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC7)) //??????
//	{
//		USART_DMACmd(USART2, USART_DMAReq_Tx, DISABLE);    //??DMA??
//		DMA_Cmd(DMA1_Channel7, DISABLE);	       			//??DMA??  	
//		Delay_MS(5);
//		GPIO_ResetBits(GPIOD, GPIO_Pin_3 );  
//	}

//	DMA_ClearFlag(DMA1_FLAG_GL7| DMA1_FLAG_TC7 | DMA1_FLAG_HT7 | DMA1_FLAG_TE7);  //??DMA????
//}




