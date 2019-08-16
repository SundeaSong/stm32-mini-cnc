#include "USART4_init.h"


char Rx4_String[60];	  			  //??????? ??lrc???????
unsigned char Rx4_Flag;  
UsartRxTypeDef4 USARTStructure4;
/****************************************************************************
* 名	称：void USART4_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART4_GPIO_Init(void)			//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//配置串口 Tx (PC.10) 为复用推挽输出
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;					//串口发送引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//频率50MHz
  	GPIO_Init(GPIOC, &GPIO_InitStructure);						//初始化引脚
    
	// 配置串口 Rx (PC.11) 为浮空输入
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					//串口接收引脚
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
  	GPIO_Init(GPIOC, &GPIO_InitStructure);						//初始化引脚
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;						//PD4 485 转换
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//端口模式为推拉输出方式	
		GPIO_Init(GPIOG, &GPIO_InitStructure);			
}

/****************************************************************************
* 名	称：void USART3_Init(void)
* 功	能：串口初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART4_Init(void)
{		
	USART_InitTypeDef UART_InitStructure;		//串口结构

	//串口分配时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	//串口初始化
	UART_InitStructure.USART_BaudRate            = USART4BaudRate;	//波特率
	UART_InitStructure.USART_WordLength          = USART_WordLength_8b;		//数据位8bit
	UART_InitStructure.USART_StopBits            = USART_StopBits_1;		//停止位个数
	UART_InitStructure.USART_Parity              = USART_Parity_No ;		//不进行奇偶效验
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//RTS和CTS使能(None不使用)
	UART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;		//发送和接收使能
	USART_Init(UART4, &UART_InitStructure);			//初始化串口
}

/****************************************************************************
* 名	称：void USART4_NVIC_Init(void)
* 功	能：串口中断向量表初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART4_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 		//中断控制器变量

	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;			//设置中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//主优先级设置
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			//设置优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//打开串口中断
	NVIC_Init(&NVIC_InitStructure);								//初始化中断向量表
}


/****************************************************************************
* 名	称：void USART2_Config(void)
* 功	能：串口设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收					 
****************************************************************************/
void USART4_Config(void)
{
	USART4_Init();					//串口初始化
	USART4_GPIO_Init();			//串口引脚初始化
	USART4_NVIC_Init();			//中断初始化
	USART4_RX_Buffer_Clear();	//接收中断与接收缓冲区绑定

		
	USART_ClearITPendingBit(UART4, USART_IT_RXNE);			//清接收标志
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);				//开启接收中断
	
	USART_Cmd(UART4, ENABLE);  								//使能失能串口外设	
}

/****************************************************************************
* 名	称：void USART2_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void USART4_RX_Buffer_Clear(void)	
{
	USARTStructure4.RX_Len = 0;
}
/****************************************************************************
* 名	称：void USART2_SendByte(u8 Data)
* 功	能：单字符发送
* 入口参数：Data 	发送单字符数据
* 出口参数：无
* 说	明：无				   
****************************************************************************/
void UART4_SendByte(u8 Data)		   //单字符数据输出
{
	USART_SendData(UART4, Data);
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
* Function Name  :
* Description    : ???????(????lrc??,0D0A)   ??':' ??'\0'
*******************************************************************************/
void Send_Command(char *str)
{
// unsigned char Lrc=0;
// unsigned char Temp=0,Temp_h=0,Temp_l=0;
// unsigned char i=0;
// char StrCmp[60];

//	while( (*str) != '\0')
//	{
//		StrCmp[i]= (*str);                                              //?????':'???
//		str++;
//		if(StrCmp[0] == 0x3a)                                           // ??????':'
//                {
//			if(i%2==0){	Temp_h=ChrToInt(*str);	Temp=0;}
//			else{	    Temp_l=ChrToInt(*str);  Temp=Temp_h*16+Temp_l;}
//			Lrc += Temp;
//			i++;
//		}
//	}
//	Lrc = (~Lrc)+1;                                       //LRC??????
//	StrCmp[i]=Ascill_16[Lrc/16];
//	StrCmp[++i]=Ascill_16[Lrc%16];
//	StrCmp[++i]='\0';

	GPIO_SetBits(GPIOG, GPIO_Pin_1 ); 
                                                        //485????
  Delay_MS(5);
	while((*str) != '\0')
	{
      UART4_SendByte((*str)); 
			str++;
	}
	UART4_SendByte(0x0d);
  UART4_SendByte(0x0a);
	Delay_MS(5);
	GPIO_ResetBits(GPIOG, GPIO_Pin_1 );                     //485????

}


/****************************************************************************
* 名	称：void USART2_IRQHandler(void)	
* 功	能：中断机制
* 入口参数：无
* 出口参数：无
* 说	明：接收到的数据存入接收缓冲区
	USART_GetITStatus		检查指定的USART中断发生与否
	USART_GetFlagStatus	检查指定的USART标志位设置与否
****************************************************************************/
void UART4_IRQHandler(void)
{
	unsigned char i;

	unsigned char temp;
	static unsigned char  num=0;
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) == SET)				// 串口接收数据触发中断
	{
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);					//清空接收中断标志			//接收到的字符数据

		temp  = USART_ReceiveData(UART4);
    if (temp=='$')
    {
       num=0;
       USARTStructure4.RX_BUFF[num]=temp;
       num++;
    }
    else if(temp==0xA)                                                          //???LF???
    {
				if(num>1)
				{
					USARTStructure4.RX_Flag=1;
					USARTStructure4.RX_Len=num;                                             //?ODOA
				}
    }
    else if(temp==0xD)                                                          //???CR???
    {
				if(num>1)
				{
					USARTStructure4.RX_Flag=1;
					USARTStructure4.RX_Len=num;                                             //?ODOA
				}
		}
    else
    {
        USARTStructure4.RX_BUFF[num]=temp;
        num++;
				if(num>=60)
				{
						for(i=0;i<60;i++)
								USARTStructure4.RX_BUFF[i]=0x00;                                //?????
						num=0;
				}
    }
	}
	else if(USART_GetFlagStatus(UART4, USART_IT_ORE) == SET)	//检测是否有接收溢出
	{
		USART_ReceiveData(UART4);																//清接收溢出标志，只能用读数据的方式来清溢出标志
	}

	else if(USART_GetITStatus(UART4, USART_IT_TXE) == SET)		//串口发送数据触发中断
	{
		
	}
}


/*******************************************************************************
* Function Name  :
* Description    : 485?? ????????lrc?????
*******************************************************************************/
void Get_Command(void)
{
unsigned char Lrc=0;
unsigned char Temp=0,Temp_h=0,Temp_l=0;
unsigned char i;

  if(USARTStructure4.RX_Flag==1)
	{
		if(USARTStructure4.RX_BUFF[0]==':')
		{
			for(i=1;i<USARTStructure4.RX_Len+1;i++)		   	//???????? 0D0A	 ?????
			{
				if(i%2 != 0){	Temp_h=ChrToInt(USARTStructure4.RX_BUFF[i]);	Temp=0;}
				else	    { 	Temp_l=ChrToInt(USARTStructure4.RX_BUFF[i]); Temp=Temp_h*16+Temp_l;}
				Lrc += Temp;
			}
		}
		Lrc=((~Lrc)+1);

		if(((~Lrc)+1) == 0)				  										//LRC????????0
		{
		  for(i=0;i<USARTStructure4.RX_Len-2;i++)		   	//???????? ??? 0D0A
				Rx4_String[i] = USARTStructure4.RX_BUFF[i];
			Rx4_String[i]='\0';
			Rx4_Flag=1;
		}

		for(i=0;i<40;i++)                                               //Clear
		{
			USARTStructure4.RX_BUFF[i]=0x20;			//?????
		}
		USARTStructure4.RX_Len=0;
		USARTStructure4.RX_Flag=0;
	}
}



