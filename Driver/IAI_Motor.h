#ifndef __IAICONTROL_H
#define __IAICONTROL_H


#include "Config.h"

//extern const uchar Ascill_16[];
//extern uchar Rx4_String[40];	  				//接受到的字符串 去除lrc校验及回车换行	extern
//extern uchar Rx4_Flag;

unsigned int IAI_Set_OrderEnable(unsigned char AdressNum);						//设置命令模式有效指令
unsigned int IAI_Set_ServoON(unsigned char AdressNum);							//设置伺服ON指令
unsigned int IAI_Set_ServoOFF(unsigned char AdressNum);							//设置伺服OFF指令
unsigned int IAI_Set_Speed(unsigned char AdressNum,unsigned char CSpeed ); 		//设置当前速度指令						
unsigned int IAI_Set_Pause(unsigned char AdressNum);                                    //设置暂停指令
unsigned int IAI_Set_ClearPause(unsigned char AdressNum);
unsigned int IAI_Set_ClearArm(unsigned char AdressNum);							//设置清除报警指令
unsigned int IAI_Go_Origin(unsigned char AdressNum);				  			//回原点指令
unsigned int IAI_Go_Zero(unsigned char AdressNum);							//回零点指令
unsigned int IAI_Go_AssignPoint(unsigned char AdressNum,int Point);		//回指定位置指令

unsigned int Read_CurrentStatus(unsigned char AdressNum);						//读取当前状态指令
unsigned int Read_ErrorCode(unsigned char AdressNum);							//读取错误代码指令
unsigned int Read_CurrentPoint(unsigned char AdressNum);						     //读取当前位置指令

unsigned int IAI_Send_WData(unsigned char AdressNum,unsigned char *WData);		 //发送写数据后的返回值
unsigned int IAI_Send_Flag(unsigned char AdressNum,unsigned char *Command);		  //发送读取数据后的返回值
unsigned int IAI_Send_Command(unsigned char AdressNum,unsigned char *Command);	  //发送控制命令后的返回值

unsigned char LRC_DATA(unsigned char *str);                                      // LRC X数据处理
//unsigned char ChrToInt(unsigned char chr);					 														 //十六进制字符串转十进制
unsigned char StringCompare(unsigned char *Command);                            //字符串比较
void Clear_Rx4_String(void);                                                    //清除接收缓存区的全部字符及标志位
//unsigned char Get_Strlen(unsigned char *Command);                                //测试字符串的长度

#endif /* __END_*/



