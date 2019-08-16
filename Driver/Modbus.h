

#ifndef _MODBUS_H_
#define _MODBUS_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif
 
#include "stm32f10x.h"

	
#define SlaveID 0x01				// 从机ID号
	
//数据全部16位
	
#define 	MBPARTADRESS 			1000	
#define 	MBPUBLICADRESS 		2000
	
extern unsigned int Modbus_Port[16];						// 地址从1000 
extern unsigned int Modbus_Public_Var[128];			// 地址从2000

unsigned int crc16(unsigned char* puchMsg,unsigned char usDataLen);
void ParseRecieve(void);  //包解析	
	
	
	
void Nvic_init(void);	   //中断管理初始化

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif

