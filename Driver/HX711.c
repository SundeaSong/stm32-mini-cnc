
#include "HX711.h"


long ZeroValue;
float Ratio;

union
{
  float f;
  unsigned char s[4];
}UnionFloat;

union
{
  long l;
  unsigned char s[4];
}UnionLong;


void HX711_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);		//开时钟
															 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;									
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//时钟
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//端口模式为推拉输出方式
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOD, GPIO_Pin_1 ); 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;								//数据				
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//端口模式为推拉输砇U方式	
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开时钟
//															 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;									
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//out
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//端口模式为推拉输出方式
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	GPIO_ResetBits(GPIOB, GPIO_Pin_8 ); 	

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;								//时钟				
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						//端口模式为推拉输砇U方式	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//****************************************************
//读取HX711
//****************************************************
long HX711_Read(void)	//增益128
{
	long count; 
	unsigned char i; 

	Delay_US(2);

	HX711_SCK_LOW;
	count=0; 
	while(HX711_DOUT); 
	for(i=0;i<24;i++)
	{ 
	  	HX711_SCK_HIGH; 
	  	count=count<<1; 
			HX711_SCK_LOW;
	  	if(HX711_DOUT)
			count++; 
	} 
	HX711_SCK_HIGH;
  count=count^0x800000;					//第25个脉冲下降沿来时，转换数据
	Delay_US(2);
	HX711_SCK_LOW; 

//Count=Count^0x800000:
//-20mV    ~   -1mV   ~   0mV    ~   20mV
//0x800000   0xffffff   0x000000   0x7fffff
//Count=Count^0x800000:
//-20mV    ~   -1mV   ~   0mV    ~   20mV
//0x000000   0x7fffff   0x800000   0xffffff
//	
	return(count - 0x800000);
}




/*******************************************************************************
* Function Name  :
* Description    : 
*******************************************************************************/
float Read_weight(void)
{
	unsigned char i;
	double tempweight;
	float fweight =0;
	long lweight =0;
	float nweight =0;	
	float Tweight[10] = {0};
	unsigned char ads = 0;
	
	
//	if(ads >=10 )
//	{
//		ads = 0;
//	}

//	Tweight[ads++] =(HX711_Read() - ZeroValue) / Ratio;	
	
	tempweight = 0;
	for(i = 0;i< 3;i++)
	{
		Tweight[ads++] =(HX711_Read() - ZeroValue) / Ratio;		
		tempweight += Tweight[i];
	}

	fweight = tempweight /3;
	lweight = (fweight+0.05) *10;
	nweight =(float)lweight/10;	
	
//	Nweight = (HX711_Read() - ZeroValue) / Ratio;
//  tempweight = tempweight+( HX711_Read() - ZeroValue) / Ratio;
//	tempweight = 0;
//	
//	for(i = 0;i< 5;i++)
//	{
//		tempweight += HX711_Read();
//	}
//	tempweight = tempweight /5;
//	Nweight = (tempweight - ZeroValue) / Ratio;
//	
return nweight;	
}
/*******************************************************************************
* Function Name  :
* Description    : 
*******************************************************************************/
void Setzero_Compensate(void)
{
	unsigned char i;
	
	long value = 0;
	
	for(i = 0;i<5;i++)
	{
		value += HX711_Read();					//读出当前值
		Delay_MS(1);
	}
	ZeroValue = value/5;							//取10次平均值
	
	UnionLong.l = ZeroValue;
	
	I2C_EE_BufferWrite(	&UnionLong.s[0],0x00,4);		//保存
}


/*******************************************************************************
* Function Name  :
* Description    : 
*******************************************************************************/
void Readzero_Compensate(void)
{
	I2C_EE_BufferRead(&UnionLong.s[0],0x00,4);		 //读取
	ZeroValue = UnionLong.l;
	DebugPf("ZeroValue = %ld\r\n",ZeroValue );
}
/*******************************************************************************
* Function Name  :
* Description    : 
*******************************************************************************/
void SetCal_Ratio(unsigned int normal)
{
	unsigned char i;
	long temp = 0;
	long value = 0;
	
	for(i = 0;i<5;i++)
	{
		value += (HX711_Read() );		//读出当前值
		Delay_MS(1);
	}
	
	temp = value/5;												//取10次平均值
	temp =temp - ZeroValue;
	Ratio = temp / normal;											
	UnionFloat.f = Ratio;
	
	I2C_EE_BufferWrite(	&UnionFloat.s[0],0x10,4);		//保存
	
}


/*******************************************************************************
* Function Name  :
* Description    : 
*******************************************************************************/
void ReadCal_Ratio(void)
{
	I2C_EE_BufferRead(&UnionFloat.s[0],0x10,4);		 //读取
	Ratio = UnionFloat.f; 
	
	if(( Ratio < 400) || ( Ratio > 440))
	{
		Ratio = 424;
	}
	DebugPf("Ratio = %f\r\n",Ratio );
}

