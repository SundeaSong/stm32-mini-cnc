#include "static_init.h"

u8 test=0;					//测试下断点用
u8 HardFault_Handler_X = 0;	//排错用

/****************************************************************************
* 名	称：void Delay(u32 nCount)
* 功	能：一个工作的频率延时
* 入口参数：u32 nCount	延时量
* 出口参数：无
* 说	明：无
****************************************************************************/
void Delay(u32 nCount)
{
  while(nCount--);
}

/****************************************************************************
* 名	称：void Delay_US(u32 nCount_temp)
* 功	能：微秒级
* 入口参数：u32 nCount_temp	延时量
* 出口参数：无
* 说	明：无
****************************************************************************/
void Delay_US(u32 nCount_temp)
{
	u32 nCount=nCount_temp*8;
	while(nCount--);
}

/****************************************************************************
* 名	称：void Delay_MS(u32 nCount_temp)
* 功	能：毫秒级
* 入口参数：u32 nCount_temp	延时量
* 出口参数：无
* 说	明：无
****************************************************************************/
void Delay_MS(u32 nCount_temp)
{
	u32 nCount=nCount_temp*8000;
	while(nCount--);
}

/****************************************************************************
* 名	称：void Delay_S(u32 nCount_temp)
* 功	能：秒级
* 入口参数：u32 nCount_temp	延时量
* 出口参数：无
* 说	明：无
****************************************************************************/
void Delay_S(u32 nCount_temp)
{
	u32 nCount=nCount_temp*8000000;
	while(nCount--);
}

/****************************************************************************
* 名	称：__asm void NOPDelay(void)
* 功	能：NOP指令周期延迟	   (差不多一个延迟1微秒)
* 入口参数：无
* 出口参数：无
* 说	明：调用一次延时一个系统时钟周期	  
****************************************************************************/
__asm void NOPDelay(void)	  //NOP指令周期级延迟
{
	NOP;
}

/****************************************************************************
* 名	称：u32 Get_SyS_CLK(u8 temp)
* 功	能：当前系统各个频率状态获取	   
* 入口参数：temp 选择频率参数
* 出口参数：时钟频率
* 说	明：分频频率(0) HCLK频率(1) PCLK1频率(2) PCLK2频率(3) SYSTEM频率(其它)
			x=Get_SyS_CLK(1); //分频频率
			DebugPf("ADCCLK=%ldMHz\r\n",x);
			x=Get_SyS_CLK(2); //HCLK频率
			DebugPf("HCLK=%ldMHz\r\n",x);
			x=Get_SyS_CLK(3); //PCLK1频率
			DebugPf("PCLK1=%ldMHz\r\n",x);
			x=Get_SyS_CLK(4); //PCLK2频率
			DebugPf("PCLK2=%ldMHz\r\n",x);
			x=Get_SyS_CLK(5); //SYSTEM频率 //其它
			DebugPf("SYSCLK=%ldMHz\r\n",x);			
****************************************************************************/
u32 Get_SyS_CLK(u8 temp)
{
	RCC_ClocksTypeDef RCC_ClockFreq;
	RCC_GetClocksFreq(&RCC_ClockFreq);	//为时钟变量赋予片上各时钟
	switch (temp)
	{
		case 1:	return RCC_ClockFreq.ADCCLK_Frequency; 	//分频频率
		case 2:	return RCC_ClockFreq.HCLK_Frequency; 	//HCLK频率
		case 3:	return RCC_ClockFreq.PCLK1_Frequency; 	//PCLK1频率
		case 4:	return RCC_ClockFreq.PCLK2_Frequency; 	//PCLK2频率
		default: return RCC_ClockFreq.SYSCLK_Frequency;	//SYSTEM频率
	}	
}

/****************************************************************************
* 名	称：u16 StoB_2_BtoS(u16 n)				 //12转21
* 功	能：2字节的大小端转换	   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 unsigned int 2字节的数
****************************************************************************/
u16 StoB_2_BtoS(u16 n)				 //12转21
{
	char nTmp;

	nTmp = ((char*)&n)[0];
	((char*)&n)[0] = ((char*)&n)[1];
	((char*)&n)[1] = nTmp;

	return n;
}

/****************************************************************************
* 名	称：u32 StoB_4_BtoS(u32 n)				  //1234转4321
* 功	能：4字节的大小端转换(整型)	   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 long int 4字节的数
****************************************************************************/
u32 StoB_4_BtoS(u32 n)				  //1234转4321
{
	char nTmp;

	nTmp = ((char*)&n)[0];
	((char*)&n)[0] = ((char*)&n)[3];
	((char*)&n)[3] = nTmp;

	nTmp = ((char*)&n)[1];
	((char*)&n)[1] = ((char*)&n)[2];
	((char*)&n)[2] = nTmp;	

	return n;
}

/****************************************************************************
* 名	称：float StoB_F4_BtoS(float f)				   //1234转4321
* 功	能：4字节的大小端转换(浮点型)		   
* 入口参数：n 转换前的数
* 出口参数：转换后的数
* 说	明：如 float 4字节的数
****************************************************************************/
float StoB_F4_BtoS(float f)				   //1234转4321
{
	char nTmp;

	nTmp = ((char*)&f)[0];
	((char*)&f)[0] = ((char*)&f)[3];
	((char*)&f)[3] = nTmp;

	nTmp = ((char*)&f)[1];
	((char*)&f)[1] = ((char*)&f)[2];
	((char*)&f)[2] = nTmp;	

	return f;
}

/****************************************************************************
* 名	称：u8 AsciiToHex(char ch)
* 功	能：单个ASCII码转16进制		   
* 入口参数：ch	'A'-'F'或'0'-'9'
* 出口参数：转换后的0x0A-0x0F.0x00-0x09
* 说	明：ASCII码A-F.0-9转16进制0x0A-0x0F.0x00-0x09
****************************************************************************/
u8 AsciiToHex(char ch)
{
	if(ch >= '0' && ch <= '9')	//数字0~9
	{
		return (ch - 0x30);
	}
	else
	{
		if(ch >= 'A' && ch <= 'F')	//字母A~F
		{
			return (ch - 0x37);
		}
		else
		{
			return 0xFF;
		}
	}
}

/****************************************************************************
* 名	称：char HexToAscii(u8 nHex)
* 功	能：单个16进制转ASCII码		   
* 入口参数：nHex	0x0A-0x0F.0x00-0x09
* 出口参数：转换后的ASCII码
* 说	明：16进制0x0A-0x0F.0x00-0x09转ASCII码A-F.0-9
****************************************************************************/
char HexToAscii(u8 nHex)
{
	if(nHex <= 9)	//数字0~9
	{
		return (nHex + 0x30);
	}
	else
	{
		if(nHex >= 0x0A && nHex <= 0x0F)	//字母A~F
		{
			return (nHex + 0x37);
		}
		else
		{
			return 0xFF;
		}
	}
}

/****************************************************************************
* 名	称：void BufferAsciiToHex(u8 *pAaciiBuf, u8 *pHexBuf, u16 nAsciiLen)
* 功	能：多个ASCII码转16进制		   
* 入口参数：*pAaciiBuf	ASCII码数据数组	   源
			*pHexBuf	16进制存放数组	   结果
			nAsciiLen	转换个数
* 出口参数：无
* 说	明：u8 x[4]="ABCD";
			u8 z[2]={0};		转换后结果{0xAB,0xCD}		 4位转2位
****************************************************************************/
void BufferAsciiToHex(u8 *pAaciiBuf, u8 *pHexBuf, u16 nAsciiLen)
{
	u16 i;
	u8 temp;
	if(pAaciiBuf == 0)
		return;
	if(pHexBuf == 0)
		return;

	for(i=0; i<nAsciiLen; i+=2)
	{
		temp = AsciiToHex(pAaciiBuf[i]);
		if(temp == 0xFF)
			return;

		pHexBuf[i>>1] = temp;//高4位
		pHexBuf[i>>1] <<= 4;

		temp = AsciiToHex(pAaciiBuf[i+1]);
		if(temp == 0xFF)
			return;

		pHexBuf[i>>1] += temp;//低4位
	}
}


/****************************************************************************
* 名	称：void BufferHexToAscii(u8 *pHexBuf,u8 *pAaciiBuf , u16 nHexLen)
* 功	能：多个16进制转ASCII码	   
* 入口参数：*pAaciiBuf	ASCII码存放数组	   结果
			*pHexBuf	16进制数据数组	   源
			nHexLen	转换个数
* 出口参数：无
* 说	明：u8 x[2]={0xAB,0XCD};
			u8 z[4]={0};		转换后结果{65,66,67,68}		 2位转4位
****************************************************************************/
void BufferHexToAscii(u8 *pHexBuf,u8 *pAaciiBuf , u16 nHexLen)
{
	u16 i;
	if(pAaciiBuf == 0)
		return;
	if(pHexBuf == 0)
		return;

	for(i=0; i<nHexLen; i++)
	{
		pAaciiBuf[i<<1] = HexToAscii(pHexBuf[i]>>4);
		if(pAaciiBuf[i] == 0xFF)
			return;

		
		pAaciiBuf[(i<<1)+1] = HexToAscii(pHexBuf[i] & 0x0F);
		if(pAaciiBuf[(i<<1)+1] == 0xFF)
			return;
	}
}

/****************************************************************************
* 名	称：void GetCpuIdHex32(u32 *HCpuId)
* 功	能：获取CPU芯片ID值32位   
* 入口参数：无
* 出口参数：无
* 说	明：3个u32的值存放CPUID
			u32 x[3]={0};
			0x22A6FF32
			0x32375931
			0x43097342
****************************************************************************/
void GetCpuIdHex32(u32 *HCpuId)
{
	HCpuId[0] = (*(u32*)(0x1FFFF7E8));
	HCpuId[1] = (*(u32*)(0x1FFFF7EC));
	HCpuId[2] = (*(u32*)(0x1FFFF7F0));
}

/****************************************************************************
* 名	称：void GetCpuIdHex16(u16 *HCpuId)
* 功	能：获取CPU芯片ID值16位   
* 入口参数：无
* 出口参数：无
* 说	明：6个u16的值存放CPUID
			u16 x[6]={0};
			0xFF32
			0x22A6
			0x5931
			0x3237
			0x7342
			0x4309
****************************************************************************/
void GetCpuIdHex16(u16 *HCpuId)
{
	*&HCpuId[0] = (*(u32*)(0x1FFFF7E8));
	*&HCpuId[1] = (*(u32*)(0x1FFFF7EA));
	*&HCpuId[2] = (*(u32*)(0x1FFFF7EC));
	*&HCpuId[3] = (*(u32*)(0x1FFFF7EE));
	*&HCpuId[4] = (*(u32*)(0x1FFFF7F0));
	*&HCpuId[5] = (*(u32*)(0x1FFFF7F2));
}

/****************************************************************************
* 名	称：void GetCpuIdHex8(u8* HCpuId)
* 功	能：获取CPU芯片ID值8位   
* 入口参数：无
* 出口参数：无
* 说	明：12个u8的值存放CPUID
			u8 x[12]={0};
			0x22 0xA6 0xFF 0x32 0x32 0x37 0x59 0x31 0x43 0x09 0x73 0x42
****************************************************************************/
void GetCpuIdHex8(u8* HCpuId)
{
	u32 CpuId[3]={0};

	CpuId[0] = (*(u32*)(0x1FFFF7E8));
	CpuId[1] = (*(u32*)(0x1FFFF7EC));
	CpuId[2] = (*(u32*)(0x1FFFF7F0));

  HCpuId[0] = (u8)(CpuId[0]>>24);
	HCpuId[1] = (u8)(CpuId[0]>>16);
	HCpuId[2] = (u8)(CpuId[0]>>8);
	HCpuId[3] = (u8)(CpuId[0]);
	HCpuId[4] = (u8)(CpuId[1]>>24); 
	HCpuId[5] = (u8)(CpuId[1]>>16);
	HCpuId[6] = (u8)(CpuId[1]>>8);
	HCpuId[7] = (u8)(CpuId[1]);
	HCpuId[8] = (u8)(CpuId[2]>>24); 
	HCpuId[9] = (u8)(CpuId[2]>>16);
	HCpuId[10] = (u8)(CpuId[2]>>8);
	HCpuId[11] = (u8)(CpuId[2]);
	
	
		
}


/****************************************************************************
* 名	称：void L_From_B(float* Val,u32 size)   //冒泡排序由小到大
* 功	能：对引入的数组进行由小到大排序
* 入口参数：float* Val		需要排序的数组
			u32 size 		数组个数
* 出口参数：无
* 算法说明：浮点数由小到大排列
****************************************************************************/
void L_From_B(float* Val,u32 size)   //冒泡排序由小到大
{ 
	float temp;	//零时存放
	u32 i=0;  	//自变量
	u32 j=0;   	//自变量

	for(;i<size-1;++i)  
	{  
		for(j=0;j<size-i-1;++j)  
		{   
			if(Val[j]>Val[j+1])  
			{
				//传感器数值
				temp=Val[j];
				Val[j]=Val[j+1];
				Val[j+1]=temp;
			}	
		}  
	} 
}

/****************************************************************************
* 算 法 名：牛顿插值
* 名	称：float Ca_NewTon(void)
* 功	能：牛顿插值计算
* 入口参数：无
* 出口参数：0：		有错误或缺少有效标定个数 
			其它:	转换完成
* 算法说明：
	WeighParam_Ten	物理值
	ADCAverage		模拟值

	u8 NewTon_Num(u16 flag);	//查找标定个数
	void NewTon_Num_init(CALIBRATION* CS,u8 num_Temp);	//计算牛顿差商

* 测试：
	CALIBRATION CalibrationSave={15,{8.75F,47.35F,107.85F,146.1},{1.035F,1.404F,1.979F,2.342}};
	u8 num;
	float weight;		  //127.0
	num=NewTon_Num(CalibrationSave.ValidFlag);	//查找当前标定使用个数
	NewTon_Num_init(&CalibrationSave,num);		//牛顿差商计算
	weight=Ca_NewTon(2.161F,&CalibrationSave,num);	//牛顿插值计算
****************************************************************************/
CALIBRATION CalibrationSave={15,			  //标定个数
		{8.75F,47.35F,107.85F,146.1F},		  //物理值
		{1.035F,1.404F,1.979F,2.342F}};		  //模拟值

//查找当前标定使用个数
//参数	u16 flag	标定个数记录 	二进制
//返回	u8			标定个数 		十进制
u8 NewTon_Num(u16 flag)
{
	u8 number=0;
	for(;( flag & 0x01==1);)
	{
		number++;
		flag=flag>>1;
	}
	return number;
}

//牛顿差商计算
//参数	CALIBRATION* CS	标定
//		u8 num_Temp		标定个数 十进制
void NewTon_Num_init(CALIBRATION* CS,u8 num_Temp)
{
	float sum;			   //计算和
	float mul;			   //计算乘
	float temp[10]={0.0F};	   //临时计算存放点
	u8 k;	//外部自变量
	u8 i;	//标定下标量
	u8 j;	//内部自变量

	for (i = 0; i < num_Temp; i++)
	{
		for (sum = 0.0F, k = 0; k <= i; k++)
		{
			for (mul = 1.0F, j = 0; j <= i; j++)
			{
				if (j != k)
				{
					mul = mul * (CS->VotageVal[k] - CS->VotageVal[j]);
				}
			}
			sum = sum + CS->WeightVal[k] / mul;
		}
		temp[i] = sum;
	}
	memcpy(CS->WeightVal,temp,4*10);
}

//牛顿插值计算
//参数	float ADC_temp	当前模拟值
//		CALIBRATION* CS	标定
//		u8 x			标定使用个数	十进制
float Ca_NewTon(float ADC_temp,CALIBRATION* CS,u8 x)
{
    u8 i,j;
	float OutYP,mul;

	for (OutYP = CS->WeightVal[0], i = 1; i < x; i++)
	{
		for (mul = CS->WeightVal[i], j = 0; j < i; j++)
		{
			mul = mul * (ADC_temp - CS->VotageVal[j]);
		}	
		OutYP = OutYP + mul;
	}
    return OutYP;
}

/****************************************************************************
* 名	称：long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
* 功	能：逐次滤波
* 入口参数：float* AD_New	存放滤波完成后的数据数组
			float* AD_Old	引入的未进行滤波的数据数组
			long ADOld_Len	未滤波的数据个数
			float Wave_Temp	相邻两个值的偏差范围
* 出口参数：long	滤波后数据个数
* 算法说明：x[10]
x[1]与x[0]比较，超过偏差值则不要，未超过则取数
x[2]与x[1]比较，……
测试：
#define old_len 100		  //未滤波的数据个数
float Wave=1000.0F;		  //相邻两个值的偏差范围
float old[100]={0.0F};	  //引入的未进行滤波的数据数组
float new[100]={0.0F};	  //存放滤波完成后的数据数组
long i=0;	  //累加数
long x=0;	  //滤波后数据个数
	for(i=0;i<100;i++)
		old[i]=i*2;
	old[10]=1000.0F;
	old[20]=2000.0F;
	old[30]=3000.0F;
	old[40]=4000.0F;	  //准备测试数据

	x=Sample_Wave(new,old,old_len,Wave);	//滤波
****************************************************************************/
long Sample_Wave(float* AD_New,float* AD_Old,long ADOld_Len,float Wave_Temp)
{
	long ko=1; //未进行滤波的数组下标
	long kn=1; //滤波的新数组下标

	AD_New[0]=AD_Old[0];		  //第一个直接赋值

	for(ko=1;ko<ADOld_Len;ko++)	  //未滤波数组从头到尾遍历
	{
		if(fabs(AD_Old[ko]-AD_New[kn-1])<=Wave_Temp)	  //相邻两个数的绝对差值与额定波动值进行比较
		{
			AD_New[kn]=AD_Old[ko];	//未超过就进行取值
			kn++;	//滤波数组下标增加
		}
	}
	return kn;
}

/****************************************************************************
* 名	称：u32 BeiNumber(u32 num_temp,u8 len_temp)
* 功	能：10倍数
* 入口参数：u32 num_temp	需要翻倍的数
			u8 len_temp	翻多少倍 即1后面多少个0
* 出口参数：u32 翻倍后的倍数值
* 测试代码：
	u32 x=2;
	x=BeiNumber(x,3);
	//x=2000;
****************************************************************************/
u32 BeiNumber(u32 num_temp,u8 len_temp)
{
     u32 Bei=1;		  //默认倍数值
	 	
     while(len_temp--)//按倍数进行翻倍
          Bei*=10;	  //翻倍增加
     return Bei*num_temp;	  //返回倍数
}

/****************************************************************************
* 名	称：u32 ASCIItoNumber(u8* data_temp,u8 len_temp)
* 功	能：将ASCII码数字转换成为u32型数字
* 入口参数：u8* data_temp	ASCII码数字
			u8 len_temp		ASCII码整数部分个数
* 出口参数：u32 unsigned int型数字
* 测试代码：
	u32 num=0;
	num=ASCIItoNumber("1234",4);
	//num=1234;

	num = ASCIItoNumber("123.45678",3);
	num = 123;
	num = ASCIItoNumber("35.124",2);
	num = 35;
	num = ASCIItoNumber("123.458",3);
	num = 123;
	num = ASCIItoNumber("3.14159",1);
	num = 3;
****************************************************************************/
u32 ASCIItoNumber(u8* data_temp,u8 len_temp)
{
    u8 i = 0;					//长度
    u32 val=0; 
    
    while(1)
    {
        val = val * 10 + data_temp[i] - 0x30;     //取个数 
        
        i++;				   //处理的个数增加
        if(i >= len_temp)	   //处理的个数大于希望处理的个数
           return val;
    } 
}

/****************************************************************************
* 名	称：double ASCIItoStarNumber(u8* data_temp,u8 len_temp,u8 RealLen)
* 功	能：将ASCII码数字转换小数
* 入口参数：u8* data_temp	ASCII码数字
			u8 len_temp		小数个数
			u8 RealLen		实际数字总长度个数
* 出口参数：double	小数
* 测试代码：
	double num = 0.0F;

	num = ASCIItoStarNumber("123.45678",5,9);
	//num = 0.45678;
	num = ASCIItoStarNumber("35.124",3,6);
	//num = 0.124;
	num = ASCIItoStarNumber("123.458",3,7);
	//num = 0.458;
	num = ASCIItoStarNumber("3.14159",5,7);
	//num = 0.14159
****************************************************************************/
double ASCIItoStarNumber(u8* data_temp,u8 len_temp,u8 RealLen)
{
    u8 i = RealLen - 1;		//长度
	u8 fi = 0;				//处理了的个数

    double val = 0.0F;

    while(1)
    {
		val = val * 0.1F + (data_temp[i] - 0x30) * 0.1F; 	  //整理出来小数
		
		i--;					  //下标位置变化
		fi++;					  //处理的个数增加

		if(fi >= len_temp)		  //处理的个数大于希望处理的个数
		   return (double)val;
    } 
}

/****************************************************************************
* 名	称：u8 JZ_chack(void)
* 功	能：8M主晶振起振检测 
* 入口参数：无
* 出口参数：u8	1起振	0没起振
* 说    明：OSC_IN(5,12,23)	
			OSC_OUT(6,13,24)
* 测试代码			
	flag=JZ_chack();
	if(flag==1)		//起振
		flag=9;
	else			//没起振
		flag=9;
****************************************************************************/
u8 JZ_chack(void)
{
	u8 flag=0;		 //晶振起振检测延时

	RCC_DeInit();			 		//将外设 RCC寄存器重设为缺省值
	RCC_HSEConfig(RCC_HSE_ON);		//设置外部8M高速晶振(HSE)
									//RCC_HSE_OFF  		HSE 	晶振 OFF 
									//RCC_HSE_ON  		HSE 	晶振 ON 
									//RCC_HSE_Bypass  	HSE 	晶振被外部时钟旁路
	flag = RCC_WaitForHSEStartUp();	//等待 HSE 起振，该函数将等待直到 HSE 就绪，或者在超时的情况下退出 
	if(flag == SUCCESS)
		return 1;
	else
		return 0;
}

/****************************************************************************
* 名	称：void RCC_USEHSI(void)
* 功	能：启用STM32内部RC晶振
* 入口参数：无
* 出口参数：无
* 说	明：
		如果使用内部RC振荡器而不使用外部晶振，请按照下面方法处理：
		1 对于100脚或144脚的产品，OSC_IN应接地，OSC_OUT应悬空。
		2 对于少于100脚的产品，有2种接法：
			2.1 OSC_IN和OSC_OUT分别通过10K电阻接地。此方法可提高EMC性能。
			2.2 分别重映射OSC_IN和OSC_OUT至PD0和PD1，再配置PD0和PD1为推挽输出并输出'0'。此方法可以减小功耗并(相对上面2.1)节省2个外部电阻。
* 测    试：
	u32 x=0;
	while(1)
	{
		DebugPf("--- %d ---\r\n",x);
		x++;
		Delay_S(1);
	}
****************************************************************************/
void RCC_USEHSI(void)
{
	GPIO_InitTypeDef X;

	//PD0 PD1分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE );	//对端口D分配APB2高速时钟，并开启端口	  重映射OSC_IN(PD0) OSC_OUT(PD1)

	//PD0_OSC_IN
	X.GPIO_Pin = GPIO_Pin_0;			//对要使用的端口引脚设定
	X.GPIO_Mode = GPIO_Mode_Out_PP;		//对要使用的端口引脚模式设定PP为推挽输出
	X.GPIO_Speed = GPIO_Speed_50MHz;	//对要使用的端口引脚频率设定50MHz
	GPIO_Init(GPIOD, &X);				//初始化GPIOD寄存器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE );	//对端口D分配APB2高速时钟，并开启端口
	
	//PD1_OSC_OUT
	X.GPIO_Pin = GPIO_Pin_1;			//对要使用的端口引脚设定
	X.GPIO_Mode = GPIO_Mode_Out_PP;		//对要使用的端口引脚模式设定PP为推挽输出
	X.GPIO_Speed = GPIO_Speed_50MHz;	//对要使用的端口引脚频率设定50MHz
	GPIO_Init(GPIOD, &X);				//初始化GPIOD寄存器

	GPIO_ResetBits(GPIOD, GPIO_Pin_0);		//OSC_IN
	GPIO_ResetBits(GPIOD, GPIO_Pin_1);		//OSC_OUT
		
	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);			//重映射OSC_IN OSC_OUT

	RCC_DeInit();						//将外设 RCC寄存器重设为缺省值 
	RCC_HSICmd(ENABLE);					//使能或者失能内部高速晶振(HSI) 
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}	  //HSI 晶振就绪	HSI内部晶振 HSE外部晶振
	
	if(1)	   //始终执行，方便关闭，内部时钟设置
	{
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);		//使能预取指缓存
		FLASH_SetLatency(FLASH_Latency_2);		   	//设置代码延时值为2个延时周期 
		RCC_HCLKConfig(RCC_SYSCLK_Div1);		   	//设置 AHB 时钟(HCLK)
		RCC_PCLK1Config(RCC_HCLK_Div2);			   	//设置低速 AHB 时钟(PCLK1)
		RCC_PCLK2Config(RCC_HCLK_Div1);			   	//设置高速 AHB 时钟(PCLK2)
													//RCC_HCLK_Div1   APB1 时钟 = HCLK 
													//RCC_HCLK_Div2   APB1 时钟 = HCLK / 2 
													//RCC_HCLK_Div4   APB1 时钟 = HCLK / 4 
													//RCC_HCLK_Div8   APB1 时钟 = HCLK / 8 
													//RCC_HCLK_Div16  APB1 时钟 = HCLK / 16 
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2);	 //设置 PLL 时钟源及倍频系数
													//RCC_PLLSource_HSI_Div2  PLL 的输入时钟 = HSI时钟频率除以 2 
													//RCC_PLLSource_HSE_Div1  PLL 的输入时钟 = HSE时钟频率 
													//RCC_PLLSource_HSE_Div2  PLL 的输入时钟 = HSE 时钟频率除以 2 
													//RCC_PLLMul_2  PLL 输入时钟 x 2 
													//RCC_PLLMul_3  PLL 输入时钟 x 3 
													//RCC_PLLMul_4  PLL 输入时钟 x 4 
													//RCC_PLLMul_5  PLL 输入时钟 x 5 
													//RCC_PLLMul_6  PLL 输入时钟 x 6 
													//RCC_PLLMul_7  PLL 输入时钟 x 7 
													//RCC_PLLMul_8  PLL 输入时钟 x 8 
													//RCC_PLLMul_9  PLL 输入时钟 x 9 
													//RCC_PLLMul_10  PLL 输入时钟 x 10 
													//RCC_PLLMul_11  PLL 输入时钟 x 11 
													//RCC_PLLMul_12  PLL 输入时钟 x 12 
													//RCC_PLLMul_13  PLL 输入时钟 x 13 
													//RCC_PLLMul_14  PLL 输入时钟 x 14 
													//RCC_PLLMul_15  PLL 输入时钟 x 15 
													//RCC_PLLMul_16  PLL 输入时钟 x 16 
		RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,那么它不能被失能	  //使能或者失能 PLL,这个参数可以取：ENABLE或者DISABLE
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}	 //等待指定的 RCC 标志位设置成功 等待PLL初始化成功
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	//设置系统时钟(SYSCLK)设置PLL为系统时钟源
		while(RCC_GetSYSCLKSource() != 0x08){}		//等待PLL成功用作于系统时钟的时钟源
													// 0x00：HSI 作为系统时钟
													// 0x04：HSE 作为系统时钟
													// 0x08：PLL 作为系统时钟
	}
}

/****************************************************************************
* 名	称：void System_Reset(void)
* 功	能：系统热重启
* 入口参数：无
* 出口参数：无
* 说	明：
		__enable_irq()  //开中断
		__disable_irq() //关中断 
* 测    试：
//重启方式1：
	u8 x=0;
	DebugPf("START\r\n");
	while(1)
	{
		DebugPf("%d\r\n",x++);
		if(x==5)				//数到10重启
		{
			System_Reset();
			while(1){;}			//如果复位失败则死在这里
		}
		Delay_S(1);
	}
	//__asm	汇编指令

//重启方式2：	
	//NVIC_SystemReset();	//固件库自带重启命令
	u8 x=0;
	DebugPf("START\r\n");
	while(1)
	{
		DebugPf("%d\r\n",x++);
		if(x==5)				//数到10重启
		{
			__disable_irq();	//禁止一切中断产生 
			NVIC_SystemReset();
			while(1){;}			//如果复位失败则死在这里
		}
		Delay_S(1);
	}
****************************************************************************/
__asm void System_Reset(void) 
{ 
	MOV R0, #1           //  
	MSR FAULTMASK, R0    // 清除FAULTMASK 禁止一切中断产生 
	LDR R0, =0xE000ED0C  // 
	LDR R1, =0x05FA0004  //  
	STR R1, [R0]         // 系统软件复位    
}

/****************************************************************************
* 名	称：u32 AddCode(u32 Data)
* 功	能：对原始数据增加密钥 
* 入口参数：u32 Data	原始数据
* 出口参数：u32	加密钥后的数据
* 说    明：无
****************************************************************************/
u32 AddKey(u32 Data)
{
	Data=rotr(Data,13);	  //循环右移13位
	return Data;
}

/****************************************************************************
* 名	称：u32 SubKey(u32 Data)
* 功	能：对有密钥的数据减去密钥 
* 入口参数：u32 Data	加密钥后的数据
* 出口参数：u32	原始数据
* 说    明：无
****************************************************************************/
u32 SubKey(u32 Data)
{
	Data=rotl(Data,13);	  //循环左移13位
	return Data;
}

/*
	u32 srcval=0xABCDEF12;
	u32 srcval_T=0;

	srcval_T = AddKey(srcval);
	DebugPf("Src = %X\t A = %X\r\n",srcval,srcval_T);
	srcval_T = SubKey(srcval);	
	DebugPf("Src = %X\t S = %X\r\n",srcval,srcval_T);
	DebugPf("Src = %X\t S = %X\r\n",srcval,AddKey(srcval_T));
*/

/****************************************************************************
* 名	称：void FIFO(u32* x,u32 len, u32 val)
* 功	能：先进先出队列	拷贝
* 入口参数：u32* x			存放缓冲区
			u32 len			缓冲区长度
			u32 val			输入值
* 出口参数：无
* 说	明：无
****************************************************************************/
void FIFO(u32* x,u32 len, u32 val)
{
	u32 i;
	for(i = 0;i < (len - 1);i++) 
	{
		x[i] = x[i + 1];
	}
	x[len - 1] = val;   
}

/****************************************************************************
* 名	称：u32 Fsave(FBuff* FQ,u32 val,u32 BuffLen)
* 功	能：队列存值
* 入口参数：FBuff* FQ	存放队列
			u32* val	存入的值
			u32 BuffLen	存放区长度
* 出口参数：u32			当前存放区中的个数
* 说	明：先入先出存	指针
****************************************************************************/
u32 Fsave(FBuff* FQ,u32 val,u32 BuffLen)
{
	*(FQ->QIn) = val;			//输入位置存数
	FQ->QIn++;					//输入位置增加
	if(FQ->QIn > FQ->QEnd)		//输入地址等于输出地址
		FQ->QIn = FQ->QStart;	//跳到开始地址
	if(FQ->Count < BuffLen)		//数据没装满的情况下
		FQ->Count++;	 			//继续装
	return FQ->Count;			//返回个数
}

/****************************************************************************
* 名	称：u8 Fget(u32* val,FBuff* FQ)
* 功	能：队列取值
* 入口参数：u32* val	取出的值
			FBuff* FQ	数据队列
* 出口参数：u8	0		取值失败
				1		取值成功
* 说	明：先入后出取	指针
****************************************************************************/
u8 Fget(u32* val,FBuff* FQ)
{
	if(FQ->Count > 0)				//缓冲区有数
	{
		FQ->QIn --;					//输入位置后退
		if(FQ->QIn < FQ->QStart)
			FQ->QIn = FQ->QEnd;	//如果处在比第一个位置还小的位置则为开始位置
		FQ->QOut = FQ->QIn;		//得到最后一个值
		*val = *(FQ->QOut);			//取值
		FQ->Count--;					//保存的个数减少
		return 1;						//取值成功
	}
	else								//缓冲区没数
		return 0;						//取值失败
}

/* 队列初始化例子
#define maxADlen 5				//数据存放区大小
static u32 DownData[maxADlen]={0};	//数据存放区
FBuff DownAD = 
{
	DownData,				//开始位置	Start
	DownData+(maxADlen-1),		//结束位置	End
	DownData,				//输入位置	In
	DownData,				//输出位置	Out	暂时
	0,						//个数
};
*/

/****************************************************************************
* 名	称：u8 FindBo(u32 val,u32* GetBoVal)
* 功	能：找波峰
* 入口参数：u32 val			引入AD值
			u32* GetBoVal	波峰输出值
* 出口参数：u8 是否有波峰形成	1形成	0没形成
* 说	明：中间数大于两边即为波峰
****************************************************************************/
static u32 BoBuff[3]={0};
static u8 i=0;
FBuff ADB = 
{
	BoBuff,				//开始位置	Start
	BoBuff+(3-1),		//结束位置	End
	BoBuff,				//输入位置	In
	BoBuff,				//输出位置	Out	暂时
	0,						//个数
};
u8 FindBo(u32 val,u32* GetBoVal)
{
	if(i<2)	//滑窗初始化	装入前2个数据
	{
		Fsave(&ADB,val,3);			//装数据
		i++;				
		return 0;			//没得到波峰
	}

	Fsave(&ADB,val,3);			//装数据

	if(BoBuff[1] > BoBuff[0] && BoBuff[1] > BoBuff[2])
	{
		*GetBoVal = BoBuff[1];	//得到波峰值
		return 1;				//得到波峰
	}
	return 0;
}

/****************************************************************************
* 名	称：u8* FindString(u8* Data,u32 DataLen,u8* StringVal,u32 StringLen)
* 功	能：在字符串中找字符串并定位
* 入口参数：u8* Data		存放字符串的数据源
			u32 DataLen		数据源最大个数
			u8* StringVal	要查找的字符串
			u32 StringLen	要查找的字符串个数
* 出口参数：u8*	定位字符串在数据源中的位置，首位字符串的位置	没找到返回指向0地址的指针
* 说	明：
u8 a[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};	//26
u8 b[1] = 'a';								//1
u8* f = 0;
f = FindString(a,sizeof(a),b,sizeof(b));
****************************************************************************/
char* FindString(char* Data,u32 DataLen,char* StringVal,u32 StringLen)
{
	char* s = 0;		 //源地址指针
	char*	p = 0;		 //比较数据地址指针
	u32 i = 0;		 //比较数据个数

	if(DataLen == 0 || StringLen == 0 || StringVal == 0 || Data == 0)	 //查找的数据和数据源为0数据为空
		return p;

	s = Data;		 //指向数据源
	p = StringVal;	 //指向查找数据

	while(DataLen > 0)		//源数据没比较完
	{
		if(*s != *p)	  	//没有相似数据
		{
			DataLen--;		//比较过的数据
			p = StringVal;	//指向查找数据
			if(i != 0)		//前面有几次比较成功了的
			{
				s -= (StringLen - 1);		//对成功过的第二个开始从新比较		abcabcd	比较abcd
				i = 0;				//比较成功个数归零
			} 
		}
		else			  	//找到相似数据
		{
			i++;			//比较成功个数增加

			if(i >= StringLen)				//找到
				return s - (StringLen - 1);	//移到比较的首字符位置

			p++;			//比较数据后移一个
		}
		s++;				//源数据位移
	}

	p = 0;			  //没找到
	return p;
}

/****************************************************************************
* 名	称：u32 FindNoZero(u8* Data,u32 DataLen)
* 功	能：在数据中查找非零个数
* 入口参数：u8* Data		数据
			u32 DataLen		数据最大个数
* 出口参数：u32				非零个数
* 说	明：遇到零退出，如果中途为0则中途退出
****************************************************************************/
u32 FindNoZero(u8* Data,u32 DataLen)
{
	u32 i = 0;	//累加器

	if(Data == 0)
		return 0;

	while(Data[i] != 0x00)	//数据不为0则继续循环
	{	
		i++;
		if(i >= DataLen)	//超过最大数据退出循环
			break;
	}	
	return i;
}

/*******************************************************************************
* Function Name  :
* Description    : 获取字符串的长度 指针以"\0"结束 Command固定字符
********************************************************************************/
unsigned char StringCompare3(char *Command,char *Command1,unsigned char len)
{
    unsigned char i=0, flag=0;
    i=0;

    for(i=0;i<len;i++)
    {
        if((*Command)!=(*Command1))
        {
            flag=1;
            break;
        }
        else
        {
            Command++;
            Command1++;
            flag=0;
        }
    }
    return flag;
}
//unsigned char StringCompare3(char *String1,char *String2,unsigned char len)
//{
//    unsigned char i;
//    if(len)
//    {
//        for(i=0;i<len-1;i++) 
//				{
//					if(*String1++!=*String2++)
//					{
//							return 1;
//					}						
//				}
//    }
//    else
//    {
//        while((*String1!='\0')||(*String2!='\0'))
//        {
//            if(*String1++!=*String2++) return 1;
//        }
//    }
//    return 0;
//}



///*******************************************************************************
//* Function Name  :
//* Description    : 获取字符串的长度 指针以"\0"结束 Command固定字符
//********************************************************************************/
unsigned char StringCompare2(char *Command,char *Command1)
{
    unsigned char i=0, flag=0;
    i=0;

    while( (*Command) != '\0')
    {
        if((*Command1) != '\0')                 //长度比较（检测结束）
        {
            if((*Command)!=(*Command1))
            {
                flag=1;
                break;
            }
            else
            {
                Command++;
                Command1++;
                i++;
                flag=0;
            }
        }
        else
        {
            flag=1;
            break;
        }
    }
    return flag;
}

/*******************************************************************************
* Function Name  :
* Description    : 十六进制字符转整形
********************************************************************************/
unsigned char ChrToInt(char chr)
{
    unsigned char n=0;

    if (chr>='0' && chr<='9')
      {
	   n=chr-'0';
	  }
    else if (chr>='a' && chr<='f')
      {
	    n=chr-'a'+10;
	  }
    else if (chr>='A' && chr<='F')
      {
        n=chr-'A'+10;
      }
    return n;
}

//*******************************************************************************
//* Function Name  :
//* Description    :
//*******************************************************************************/
unsigned int StringToInt(char *Command,unsigned char mod)
{
    char i=0;
    unsigned int num=0;
    unsigned char len=0, byt=0;
    unsigned char tempstr[8];

    while( (*Command) != '\0')
    {
        tempstr[len]=(*Command);
        Command++;
        len++;
    }   
    len=len-1;
    if (mod == 0)
    {
        for(i = len; i>0 ;i--)
        {
            byt=ChrToInt(tempstr[len-i]);
            num=num+byt*pow(10,i);
            Command++;
        }
    }
    else
    {
        for(i = len; i>0 ;i--)
        {
            byt=ChrToInt(tempstr[len-i]);
            num=num+byt*pow(16,i);
            Command++;
        }
    }
    return num;
}

///*******************************************************************************
//* Function Name  :
//* Description    : 获取字符串的长度 指针以"\0"结束
//********************************************************************************/
unsigned char Get_Strlen(char *Command)
{
  unsigned char i=0;

	while( (*Command) != '\0')
	{
		Command++;
		i++;
	}
	return i;
}



