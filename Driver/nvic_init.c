#include <nvic_init.h>

/****************************************************************************
* 名	称：void Nvic_init(void)
* 功	能：中断管理初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void Nvic_init(void)
{
	#ifdef  VECT_TAB_RAM	//中断向量表存放地址到SRAM 0x20000000  
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else					//中断向量表存放地址到FLASH	0x08000000
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	//嵌套向量中断控制器设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);			//优先级分组号 0-4
															//0 - 优先级0个 响应优先级15个	  0000 1111  
															//1 - 优先级1个 响应优先级7个	  0001 0111
															//2 - 优先级3个 响应优先级3个	  0011 0011
															//3 - 优先级7个 响应优先级1个	  0111 0001
															//4 - 优先级15个 响应优先级0个	  1111 0000
}
