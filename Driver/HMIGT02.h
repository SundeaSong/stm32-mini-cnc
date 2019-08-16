

#ifndef _HMIGT02_H_	
#define _HMIGT02_H_

#ifdef __cplusplus		   				//定义对CPP进行C处理 //开始部分
extern "C" {
#endif


#include "Config.h"
	

	
	
enum status		 //枚举
{
	TIMEOUT=0, //超时//校验出错 //空闲

	AUTO=1,	   //自动模式10
	MANU=2,	   //手动模式20
	REST=4,	   //设置模式40
	ORG=8,		 //系统复位80
	M000=16,		 // 0 位置
	M180=32,		 // 180位置
	JUP=64,	   //微调上
	JDW=128,	   //微调下
	SAVE_Q=256,	   //保存询问
	SAVE_Y=512,	   //保存是
	SAVE_N=1024,	   //保存否
	EXIT = 2048,
	CSPEED = 4096,
	SSET = 8192,	
};
extern enum status TOUCH_KEY;

unsigned long Get_Key(void);
unsigned long Get_DATA(void);

unsigned long Get_Key(void);
unsigned long HexStringToInt(unsigned char *Command,unsigned char len);
unsigned int SendGTdata(int STA,int ERR);	

#ifdef __cplusplus		   				//定义对CPP进行C处理 //结束部分
}




#endif

#endif


