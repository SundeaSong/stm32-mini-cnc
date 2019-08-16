#include "ADC.h"   

uint16_t ADCPrimevalValue[6];
/****************************************************************************
* 名	称：void AD1_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void AD1_GPIO_Init(void)						//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

/*配置PC0 1 2 3 4 5 为模拟输入(ADC Channel 10 11 12 13 14 15 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5; 	 //ADC测试试用
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	       			//端口模式为模拟输入方式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   			//翻转速度为50M
  GPIO_Init(GPIOC, &GPIO_InitStructure);			   			//用以上几个参数初始化PC口
}
/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  				 //72M/6=12,ADC最大时间不能超过14M
	
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;				 //独立的转换模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  				 //开启扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   				 //开启连续转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ADC外部开关，关闭状态
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   			 //对齐方式,ADC为12位中，右对齐方式
  ADC_InitStructure.ADC_NbrOfChannel = 6;	 						 //开启通道数，6个
  ADC_Init(ADC1, &ADC_InitStructure);
 //ADC内置温度传感器使能（要使用片内温度传感器，切忌要开启它）    
	ADC_TempSensorVrefintCmd(ENABLE);   

  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);	//ADC通道组， 第10个通道 采样顺序1，转换时间 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);	//ADC通道组， 第11个通道 采样顺序2，转换时间 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_55Cycles5);	//ADC通道组， 第12个通道 采样顺序3，转换时间 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 5, ADC_SampleTime_55Cycles5);	//ADC通道组， 第13个通道 采样顺序4，转换时间                           											
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 6, ADC_SampleTime_55Cycles5);	//ADC通道组， 第14个通道 采样顺序5，转换时间                           											
 // ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_55Cycles5);	//ADC通道组， 第15个通道 采样顺序6，转换时间                           											

 //常规转换序列2：通道16（内部温度传感器），采样时间>2.2us,(239cycles)    
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 4, ADC_SampleTime_55Cycles5);  
	
  ADC_DMACmd(ADC1, ENABLE);											 //使能ADC1 DMA 
  ADC_Cmd(ADC1, ENABLE);                                 			 //使能ADC1
  ADC_ResetCalibration(ADC1);							 			 //重置ADC1校准寄存器
  while(ADC_GetResetCalibrationStatus(ADC1));			 			 //得到重置校准寄存器状态
  ADC_StartCalibration(ADC1);							 			 //开始校准ADC1
  while(ADC_GetCalibrationStatus(ADC1));				 			 //得到校准寄存器状态
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);				 			 //使能ADC1由软件控制开始转换ADC,通过DMA方式不断的更新RAM区。
}

/*******************************************************************************
* Function Name  : DMA_Configuration(void)
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;        								//DMA初始化结构体声明
 	
  DMA_DeInit(DMA1_Channel1);		  										//开启DMA1的第一通道
  DMA_InitStructure.DMA_PeripheralBaseAddr = DR_ADDRESS;		  			//DMA对应的外设ADC基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCPrimevalValue;   	//内存存储基地址用户定义
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//DMA的转换模式为SRC模式，由外设搬移到内存
  DMA_InitStructure.DMA_BufferSize = 6;		   								//DMA缓存大小，5个
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//接收一次数据后，设备地址禁止后移
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//关闭接收一次数据后，目标内存地址后移
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //定义外设数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  		//DMA搬移数据尺寸，HalfWord就是为16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   						//转换模式，循环缓存模式。
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//DMA优先级高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  					    //M2M模式禁用
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
 		 /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

/****************************************************************************
* 名	称：void AD1_Config(void)
* 功	能：AD设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收					 
****************************************************************************/
void AD1_Config(void)
{
	ADC_Configuration();				//AD初始化
	AD1_GPIO_Init();			//AD引脚初始化
	DMA_Configuration();
}

//采样数据与实际电压/温度转换：
//u16 GetTemp(u16 advalue)   
//{   
//    u32 Vtemp_sensor;   
//    s32 Current_Temp;   
//      
////    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：    
////          V25 - VSENSE    
////  T(℃) = ------------  + 25    
////           Avg_Slope    
////   V25：  温度传感器在25℃时 的输出电压，典型值1.43 V。    
////  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为：    
////            ADC_ConvertedValue * Vdd    
////  VSENSE = --------------------------    
////            Vdd_convert_value(0xFFF)    
////  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值4.3 mV/℃。    
//   
//    Vtemp_sensor = advalue * 330 / 4096;   
//    Current_Temp = (s32)(143 - Vtemp_sensor)*10000/43 + 2500;   
//    return (s16)Current_Temp; 
//}    
 
/*******************************************************************************
* Function Name  : GetTemp
* Description    : 根据ADC结果计算温度
* Input          : u16 advalue
* Output         : 
* Return         : u16 temp
*******************************************************************************/
u16 GetTemp(u16 advalue)
{
    vu16 Current_Temp;
    
//    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：
//          V25 - VSENSE
//  T(℃) = ------------  + 25
//           Avg_Slope
//   V25：  温度传感器在25℃时 的输出电压，典型值1.43 V。
//  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为： 
//            ADC_ConvertedValue * Vdd
//  VSENSE = --------------------------
//            Vdd_convert_value(0xFFF)
//  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值4.3 mV/℃。

    Current_Temp = (1.42 - advalue*3.288/4096)*1000/4.35 + 25; 
    return (s16)Current_Temp;
}

u16 GetVolt(u16 advalue)   
{
   
    return (u16)(advalue * 3288 / 4096);   
} 
