#include "PWM_Init.h"   


/****************************************************************************
* 名	称：
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/

void PWM_Config(void)						
{
	PWM_GPIO_Init();
	Timer1_Init();
}



void PWM_GPIO_Init(void)						//串口引脚初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
	
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);		//重映射
	//PWM OUT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14;																	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//时钟速度为50M
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
		
	GPIO_ResetBits(GPIOE, GPIO_Pin_13 |GPIO_Pin_14 ); 
//	GPIO_ResetBits(GPIOG,GPIO_Pin_10);  	   // BRAKE ==1 
}




void Timer1_Init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef        TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

		TIM_DeInit(TIM1);      

		TIM_TimeBaseStructure.TIM_Period = 99; 								      	 	 
		TIM_TimeBaseStructure.TIM_Prescaler = 71;    //(1+TIM_Prescaler) /72000000   
	
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);	
		//
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;        
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   
    TIM_OCInitStructure.TIM_Pulse = 0;          
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
		
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);      //配置通道3
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);    	
		TIM_SetCompare3(TIM1,50);
		
//    TIM_OC4Init(TIM1, &TIM_OCInitStructure);      //配置通道4		
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
//		TIM_SetCompare4(TIM1,0);
		
	  TIM_ARRPreloadConfig(TIM1, ENABLE);   				
		TIM_Cmd(TIM1, ENABLE);							//打开定时器
		TIM_CtrlPWMOutputs(TIM1, DISABLE);		//设置PMW主输出
}


