#ifndef _BSP_H_	
#define _BSP_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif

#include "stm32f10x.h"				//STM32库函数
#include "nvic_init.h"	   			//中断管理
#include "USART1_init.h"			//串口1
#include "USART2_init.h"			//串口2
#include "printf_init.h"			//打印端口
	
//ErrorStatus HSEStartUpStatus;

#define OS_TICKS_PER_SEC 100		//滴答频率(ucos系统频率)，100分之1秒，10ms

#define SYSTEM_VERSION 0   			//系统版本

void BSP_Init(void);				//硬件资源初始化

#ifdef __cplusplus		   			//定义对CPP进行C处理 //结束部分
}
#endif

#endif
