#ifndef _NVIC_INIT_H_
#define _NVIC_INIT_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif
 
#include "stm32f10x.h"

void Nvic_init(void);	   //中断管理初始化

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif
