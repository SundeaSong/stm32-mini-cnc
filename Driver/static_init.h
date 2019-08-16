#ifndef _STATIC_INIT_H_	
#define _STATIC_INIT_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif

#include "stm32f10x.h"
#include "String.h"				//memcpy
#include "math.h"				//数学计算
#include <stdio.h>   			//printf

#define rotl(x,n) ((x>>(sizeof(x)*8-n)) | (x<<n) ) 		 //左移循环
#define rotr(x,n) ((x<<(sizeof(x)*8-n)) | (x>>n) )		 //左移循环

#define DebugPf			printf			//调试printf输出

typedef struct
{
	u32 ValidFlag;				//标定个数
	float  WeightVal[10];		//物理值
	float  VotageVal[10];		//模拟值
} CALIBRATION;					//牛顿插值标定

typedef struct
{
	u32			*QStart;		        //开始位置
	u32			*QEnd;		            //结束位置
	u32			*QIn;				    //输入位置
	u32			*QOut;				    //输出位置
	u32			Count;				    //个数
} FBuff;

extern u8 test;						//测试下断点用
extern CALIBRATION CalibrationSave;	//牛顿插值标定

void Delay(u32 nCount);				//系统频率级延时
void Delay_US(u32 nCount);			//微秒级延时
void Delay_MS(u32 nCount);			//毫秒级延时
void Delay_S(u32 nCount);			//秒级延时
void NOPDelay(void);					//NOP指令周期延迟
void L_From_B(float* Val,u32 size);	//冒泡由小到大

u32 Get_SyS_CLK	(u8 temp);			//当前系统各个频率状态获取	   分频频率(0) HCLK频率(1) PCLK1频率(2) PCLK2频率(3) SYSTEM频率(其它)
u16 StoB_2_BtoS(u16 n);				//12转21
u32 StoB_4_BtoS(u32 n);				//1234转4321 整型
float StoB_F4_BtoS(float f);			//1234转4321 浮点型

u8 AsciiToHex(char ch);				//单个ASCII码A-F.0-9转16进制0x0A-0x0F.0x00-0x09
char HexToAscii(u8 nHex);			//单个16进制0x0A-0x0F.0x00-0x09转ASCII码A-F.0-9
void BufferAsciiToHex(u8 *pAaciiBuf, u8 *pHexBuf, u16 nAsciiLen);	 	//多个ASCII码转16进制	 4位转2位
void BufferHexToAscii(u8 *pHexBuf,u8 *pAaciiBuf , u16 nHexLen);			//多个16进制转ASCII码	 2位转4位

void GetCpuIdHex8(u8* HCpuId);		//获取CPU芯片ID值8位 u8 x[12];
void GetCpuIdHex16(u16 *HCpuId);		//获取CPU芯片ID值16位 u16 x[6];
void GetCpuIdHex32(u32 *HCpuId);		//获取CPU芯片ID值32位 u32 x[3];

u8 NewTon_Num(u16 flag);									//查找当前标定使用个数
void NewTon_Num_init(CALIBRATION* CS,u8 num_Temp);		//牛顿差商计算
float Ca_NewTon(float ADC_temp,CALIBRATION* CS,u8 x);	//牛顿插值计算
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp);	//逐次滤波
u32 ASCIItoNumber(u8* data_temp,u8 len_temp);			//ASCII转数字
double ASCIItoStarNumber(u8* data_temp,u8 len_temp,u8 RealLen);	//ASCII转小数
u32 BeiNumber(u32 num_temp,u8 len_temp);					//根据输入的位数进行翻倍
u8 JZ_chack(void);  										//8M主晶振起振检测 OSC_IN(5,12,23)	OSC_OUT(6,13,24)
void RCC_USEHSI(void);									//使用STM32内部RC时钟 不精确 慢
void System_Reset(void);									//系统重启
u32 AddKey(u32 Data);			//加密钥
u32 SubKey(u32 Data);			//减密钥
u32 Fsave(FBuff* FQ,u32 val,u32 BuffLen);				//先入先出存
u8 Fget(u32* val,FBuff* FQ);				//队列取
u8 FindBo(u32 val,u32* GetBoVal);			//找波峰
char* FindString(char* Data,u32 DataLen,char* StringVal,u32 StringLen);	//字符串查找
u32 FindNoZero(u8* Data,u32 DataLen);		//找非零个数

unsigned int StringToInt(char *Command,unsigned char mod);
//unsigned char StringCompare3(char *String1,char *String2,unsigned char len);
unsigned char StringCompare3(char *Command,char *Command1,unsigned char len);
unsigned char StringCompare2(char *Command,char *Command1);

unsigned char ChrToInt(char chr);
unsigned char Get_Strlen(char *Command);

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif
