/* 
 * File:   AbnormalHandle.h
 * Author: Administrator
 *
 * Created on 2013年12月31日, 下午1:27
 */

#ifndef ABNORMALHANDLE_H
#define	ABNORMALHANDLE_H


#include "Config.h"

#define MAX_ADRESS 2		// 轴数量 不含0，modbus 0地址群发
#define REPEAT_TIMES  2							 	//??????
#define MISTAKE_RNGE  5                                                         // ???? (*/100) mm
#define TIME_OUT 10								//???? 100ms (??)2ms

#ifdef	__cplusplus
extern "C" {
#endif

extern unsigned char ServoBusyBit[MAX_ADRESS];				//????
extern unsigned char ServoPauseBit[MAX_ADRESS];				//????
extern unsigned char ServoOriginBit[MAX_ADRESS];			//????
extern unsigned char ServoErrorBit[MAX_ADRESS];				//????
extern unsigned char ServoPwrOnBit[MAX_ADRESS];                         //??ON
extern int ServoCurrentPoint[MAX_ADRESS];                             //?????? ???
	
extern unsigned int Error_Code;
	
void Read_Status_Handle(unsigned char adress);								  					//定时命令处理函数

unsigned int ServoInitialize(unsigned char AdressNum);			  	//伺服初始话函数
unsigned int ServoGoOrigin(unsigned char AdressNum);			  	//伺服回原点函数
unsigned int ServoGoAssignPoint(unsigned char AdressNum,unsigned int Point);  //伺服去指定位置
unsigned int ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed);       //伺服全局速度设置


#ifdef	__cplusplus
}
#endif

#endif	/* ABNORMALHANDLE_H */

