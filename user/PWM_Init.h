#ifndef _PWM_INIT_H_	
#define _PWM_INIT_H_

#ifdef __cplusplus									  //定义对CPP进行C处理
extern "C" {
#endif

#include "Config.h"


void PWM_Config(void);	
void PWM_GPIO_Init(void);
void Timer1_Init(void);
	
#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif






