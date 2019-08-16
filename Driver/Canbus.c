
#include "Config.h"
#include "Canbus.h"

CanRxMsg RxMessage;							//要发的信息结构体
u8 can_var=0;
volatile u8 g_CanRecvFlag=0;	  //can是否收到数据标志
__IO uint32_t ret = 0;          /* for return of the interrupt handling */
volatile TestStatus TestRx;		  //can发送成功标志

//---------------------------------------------------------------------------
//函数名： 		   void CAN_NVIC_Configuration(void)
//函数功能描述：   CAN中断初始化
//函数参数： 	  
//函数返回值： 	  
//作者： 		 wenzer
//修改人：		   修改日期：
//修改原因： 
//---------------------------------------------------------------------------

void CAN_NVIC_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
	  /* Enable CAN1 RX0 interrupt IRQ channel */

	  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure);

	  /* Enable CAN1 RX0 interrupt IRQ channel */
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


//---------------------------------------------------------------------------
//函数名： 		  void CAN_GPIO_Init(void)
//函数功能描述：  can相关IO初始化 
//函数参数： 	  
//函数返回值： 	  
//作者： 		 wenzer
//修改人：		   修改日期：
//修改原因： 
//---------------------------------------------------------------------------
void CAN_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			//开AFIO时钟
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);	 		/* CAN1 Periph clock enable */	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_CAN, ENABLE);	   /* GPIO clock enable */
	

	  /* Configure CAN pin: RX */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
	  /* Configure CAN pin: TX */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
	
	  //STM32FVBT6 中的 can 物理引脚脚位可以设置成三种：默认模式，重定义地址1模式，重定义地址2模式。 
	//CANRX,CANTX 分别重定义到 PB8,PB9 引脚上面。 因此我们软件中第一步要进行重定义的操作：
	  GPIO_PinRemapConfig(GPIO_Remap_CAN , ENABLE);	   	//重映射

}

//---------------------------------------------------------------------------
//函数名： 		  	 void CAN_BUS1_configure(void)
//函数功能描述：   	 can速度配置
//函数参数： 	  
//函数返回值： 	  
//作者： 		 wenzer
//修改人：		   修改日期：
//修改原因： 
//---------------------------------------------------------------------------
void CAN_BUS1_configure(void)
{
//	u8 slave_id = 0x11;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */ 
  CAN_InitStructure.CAN_TTCM=DISABLE;	 //禁止时间触发通信模式
  CAN_InitStructure.CAN_ABOM=DISABLE;	 //硬件自动清0 MCR寄存器
  CAN_InitStructure.CAN_AWUM=DISABLE;	//睡眠模式清除 MCR sleep标志
  CAN_InitStructure.CAN_NART=ENABLE;  //发送失败不重发
  CAN_InitStructure.CAN_RFLM=DISABLE;	//FIFO溢出，下次报文将覆盖
  CAN_InitStructure.CAN_TXFP=DISABLE;	//报文优先级由标识符决定
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;				//正常模式
	
// CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;	  //----loop back

  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
  CAN_InitStructure.CAN_Prescaler=9;								//250K
  CAN_Init(CAN1, &CAN_InitStructure);

//(8,7,1)500k
//(8,7,9)250k
//(9,8,200)10k
//72M/2/200/(1+9+8) = 10K

  /* CAN filter init */
	//以下配置过滤器 全通过
  CAN_FilterInitStructure.CAN_FilterNumber=1; 
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)slave_id<<21)&0xffff0000)>>16;
//	
//  CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)slave_id<<21)|CAN_ID_STD|CAN_RTR_DATA)&0xffff;
//	
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xFFFF;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;				//设置过滤标志符
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;		//用来设置过滤屏蔽标识符
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;	  //fifo 0
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;					//使能过滤
  CAN_FilterInit(&CAN_FilterInitStructure); 

   /* CAN FIFO0 message pending interrupt enable */
//  CAN_ITConfig(CAN1, CAN_IT_TME, DISABLE);                	 //关发送中断
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE); 
	
}

//---------------------------------------------------------------------------
//函数名： 		  void CAN_BUS_init(void)
//函数功能描述：   初始化can总线资源
//函数参数： 	  
//函数返回值： 	  
//作者： 		 wenzer
//修改人：		   修改日期：
//修改原因： 
//---------------------------------------------------------------------------
void CAN_BUS_init(void)
{
	
	CAN_GPIO_Init();	           /* CAN总线IO和时钟配置 并使能CAN1*/
	CAN_NVIC_Configuration();	  /* CAN 总线中断配置 */
	CAN_BUS1_configure();	      //
	
	CAN_WakeUp(CAN1);

}


//---------------------------------------------------------------------------
//函数名： 		
//函数功能描述：   
//函数参数： 	  
//函数返回值： 	  
//作者： 
//修改人：		   修改日期：
//修改原因： 
//---------------------------------------------------------------------------
unsigned char CanbusSendData(u8 reader_addr,u8 *pdata,u8 len)
{
 
	  CanTxMsg TxMessage;
	  uint8_t TransmitMailbox = 0;
		unsigned int i;
		u8 message_tx_status = 0;
	
	  TxMessage.StdId=reader_addr;		//ID
//	  TxMessage.ExtId=0x1234;
	
		TxMessage.IDE=CAN_ID_STD;			 	//标准格式
//		TxMessage.IDE=CAN_ID_EXT;			//扩展格式
	
	  TxMessage.RTR=CAN_RTR_DATA;	   	//数据帧
	  TxMessage.DLC=len;							//数据长度
	
	  for(i=0;i<len;i++)
	  {
	  	 TxMessage.Data[i]=pdata[i];
	  }
  
		TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);   //发送
		i=0;
	  while(( message_tx_status  != CANTXOK) && (i != 0x1FF))
	  {
			message_tx_status  = CAN_TransmitStatus(CAN1, TransmitMailbox);
			
			if(message_tx_status == CANTXOK)
			{
					DebugPf("message send ok\r\n");
				break;
			}
			else if(message_tx_status == CANTXPENDING)   
			{
				if(i ==0)
				{
					DebugPf("message send pending\r\n");
				}
			}
			else if(message_tx_status == CANTXFAILE)   
			{
					DebugPf("message send failed\r\n");
				break;
			}
			
			__NOP();
			__NOP();
			__NOP();
	    i++;
	  }
	
	  if(i==0x1FF) 
	  {
				CAN_CancelTransmit(CAN1,message_tx_status); //取消传送
				return 0;  //fail
			
	  }
		else
	  {
	  		return 1;
	  }
		
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{

	
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{   
	u8 tmp=0;
	u8 i =0;
	
			RxMessage.StdId=0x00;
			RxMessage.ExtId=0x00;
			RxMessage.IDE=0;
			RxMessage.DLC=0;
			RxMessage.FMI=0;
			RxMessage.Data[0]=0x00;
			RxMessage.Data[1]=0x00;
			RxMessage.Data[2]=0x00;
			RxMessage.Data[3]=0x00;
			RxMessage.Data[4]=0x00;
			RxMessage.Data[5]=0x00;
			RxMessage.Data[6]=0x00;
			RxMessage.Data[7]=0x00;
	
   if(SET == CAN_GetITStatus(CAN1,CAN_IT_FF0))
   {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
   }
   else if(SET == CAN_GetITStatus(CAN1,CAN_IT_FOV0))
   {
        CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);
   }
   else
   {
		 tmp=CAN_MessagePending(CAN1,CAN_FIFO0);							//判断接受是否有数据
		 if(tmp)
		 {	 
			 CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);		  		//接收	

			 for(i =0;i<RxMessage.DLC;i++)
			 {
				 DebugPf("%c",RxMessage.Data[i]);			 
			 }		 
//		 ret = CanbusSendData(0x88,RxMessage.Data,RxMessage.DLC);
			 CAN_FIFORelease(CAN1,CAN_FIFO0);//清fifo
		 }		
   } 
}



