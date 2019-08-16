/**********************************PWM OUT BOARD v1.0 3轴控制程序*********************************************/
//作者：董斌
//日期：2016.11.23
//修改1：
/************端口定义*************************************************************************************/
//X1(PE2) 	X2(PE3) 	X3(PE4) 	X4(PE5) 	Axis home sensor
//x5(PE6) 	x6(PF0) 	x7(PF1) 	x8(PF2) 	axis pos sensor
//x9(PF3) 	x10(PF4) 	x11(PF5) 	x12(PF6) 	axis neg sensor

//Y1(PG8) 	Y2(PG7) 	Y3(PG6) 	Y4(PG5)	 自定义输出

//Pluse1(PC6) 	Pluse2(PC7) 	Pluse3(PC8) 	Pluse4(PC9)
//Dir1(PD12) 		Dir2(PD13) 		Dir3(PD14) 		Dir4(PD15)
//EN1 (PG12) 		EN1 (PG11) 		EN1 (PG10) 		EN1 (PG9) 

//PEn1(PE1) 		PEn2(PE0) 		PEn3(PG14) 		PEn4(PG13)  差分IC使能
/*********************************************************************************************************/
#include "Config.h"


#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 
/* __GNUC__ */
   
unsigned int Program_Cycle_Nums=0;	
void Spindle_Control(void);
/*******************************************************************************
* Function Name  : Main()
* Description    : 
* Parameter		 
* Return		
********************************************************************************/
int main(void)
{   
	BSP_Init();										//Board Init，板级初始化
  PWR_ON;
	
	settings_init();
//	settings_reset(true);				//Ê¹ÓÃÄ¬ÈÏ²ÎÊý
	st_init(); 
	
	memset(&sys, 0, sizeof(sys));
	sys.abort = true;   // Set abort to complete initialization
  sys.state = STATE_INIT;  // Set alarm state to indicate unknown initial position 
	init_grbl();

	if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == SUCCESS)
	{
		DebugPf("*** CNC Control system boot ok***\r\n");
	}
	else
	{
		DebugPf("*** Boot Fail [ HSE ]");
	}
	
	while(1)
	{

		protocol_process();					//分析串口命令并执行
		Spindle_Control();					//主轴控制 上升沿触发型

		/*********************/ 		//运行指示灯
		Program_Cycle_Nums++;
		if(Program_Cycle_Nums==50)
		{
			LED_LED1_ON();
		}
		else if(Program_Cycle_Nums>=100)		//1S
		{
			LED_LED1_OFF();
			Program_Cycle_Nums=0;			
		} 
		//Delay_MS(10);
	}	
}


/***********主轴按键控制*******************/
void Spindle_Control(void)
{
	static u8 k=1;
	static u8 b=0;
	
	if(k == 1)
	{
		if(!IN_X4)		//按键按下
		{
			k=0;
			b++;	
			if(b==1)
			{
				SPINDLE_ON;
				MotorsEnable(0x00);		//释放XYZ轴
			}
			else if(b==2)
			{
				SPINDLE_OFF;
				MotorsEnable(0x07);		//使能XYZ轴
				b=0;
			}
		}
	}
	else if(k ==0)
	{
		if(IN_X4)		//按键松开
		{k=1;}
	}
	//*********************

			
}

/*******************************END*************************************************/

