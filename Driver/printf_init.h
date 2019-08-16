#ifndef _PRINTF_INIT_H_	
#define _PRINTF_INIT_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif

#include "stm32f10x.h"
#include <stdio.h>		   //printf FILE

u8 Get_PrintfPort(void);   		//获取打印输出端口
void Set_PrintfPort(u8 Port);	//设置打印输出端口

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif



