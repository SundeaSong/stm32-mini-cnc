#include "Config.h"


/****************************************************************************
* 名	称：void  SysTickInit(void)
* 功	能：系统时钟 ucos基时
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void SysTickInit(void)
{
	u32 cnts = 0;
	RCC_ClocksTypeDef rcc_clocks;

	RCC_GetClocksFreq(&rcc_clocks);									//获取系统频率	72M = 72,000,000
	cnts = (u32)rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC;		//滴答频率 1/100秒	即10ms
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	SysTick_Config(cnts);   										//720000
}

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;

  RCC_DeInit();												//时钟控制寄存器全部恢复默认值
  RCC_HSEConfig(RCC_HSE_ON);								//外部高速时钟源开启（8M晶振）
  HSEStartUpStatus = RCC_WaitForHSEStartUp();				//等待外部时钟就绪
  if(HSEStartUpStatus == SUCCESS)							//如果时钟启动成功
  {
    RCC_HCLKConfig(RCC_SYSCLK_Div1);						//定义AHB设备时钟为系统时钟1分频
    RCC_PCLK2Config(RCC_HCLK_Div1);							//定义APB2设备时钟为HCLK时钟1分频
    RCC_PCLK1Config(RCC_HCLK_Div2);							//定义APB1设备时钟为HCLK时钟2分频
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);						//定义ADCCLK时钟为PCLK2时钟6分频，不能大于14M
		RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
    FLASH_SetLatency(FLASH_Latency_2);						//设定内部FLASH的的延时周期为2周期
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);	//使能FLASH预存取缓冲区
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	//配置PLL时钟为外部高速时钟的9倍频，8MHz * 9 = 72 MHz
    RCC_PLLCmd(ENABLE);										//使能PLL时钟
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)		//等待PLL时钟设置完成准备就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);				//使用PLL时钟作为系统时钟源
    while(RCC_GetSYSCLKSource() != 0x08)					//返回系统所用时钟源确认为外部高速晶振，8M晶振。
    {   }
  }

  /* 设备时钟控制 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1   |			//使能由APB2时钟控制的外设	
   					 RCC_APB2Periph_AFIO   |	
 						 RCC_APB2Periph_USART1 | 
						 RCC_APB2Periph_GPIOA  |
						 RCC_APB2Periph_GPIOB  |
						 RCC_APB2Periph_GPIOC  |
						 RCC_APB2Periph_GPIOD  |
						 RCC_APB2Periph_GPIOE  |
						 RCC_APB2Periph_GPIOF	 |
						 RCC_APB2Periph_GPIOG, ENABLE);	   

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC    |	//使能由APB1时钟控制的外设
  						 RCC_APB1Periph_TIM2   |			
						   RCC_APB1Periph_USB    |
						   RCC_APB1Periph_USART2 |
						   RCC_APB1Periph_USB, ENABLE);	
						 	
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//使能DMA时钟
	
	

}

/****************************************************************************
* 名	称：void  BSP_Init(void)
* 功	能：驱动初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void BSP_Init(void)		//硬件资源初始化
{
	unsigned int PowerON_Count;
	/*****************system*******************/
	SystemInit(); 			//STM32系统初始化
	RCC_Configuration();
	Nvic_init();				//中断管理初始化
	SysTickInit();			//uCOS时基1ms; 
	PWM_Config();				//主轴控制
	/*****************board*******************/
	//UART 初始化
	USART1_Config();		//串口1初始化		//PA9 PA10,用作USB转串口
//	USART2_Config();		//串口2初始化		//PA2 PA3 RS485
//	USART3_Config();		//串口3初始化   //PD8 PD9 RS232	//复用
//  USART4_Config();		//串口4初始化   //PC10 PC11 RS485
//	USART5_Config();		//串口5初始化   //PC12 PD2  RS232		
	
	Set_PrintfPort(1);	//设置printf打印输出用端口		

	GPIO_Config();			//LED 指示灯    
	BKP_Config();				//后备寄存器
	I2C_EE_Init();
    
	PowerON_Count = BKP_ReadBackupRegister(BKP_DR1);	 //查看后备数据是否丢失
//	printf("PowerON_Count = %d \r\n",PowerON_Count);

	PowerON_Count ++;
	BKP_WriteBackupRegister(BKP_DR1,PowerON_Count);									
}



