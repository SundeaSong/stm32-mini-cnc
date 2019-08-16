

#ifndef _HX711_H_	
#define _HX711_H_

#ifdef __cplusplus		   				//定义对CPP进行C处理 //开始部分
extern "C" {
#endif


#include "Config.h"
	
#define HX711_SCK_HIGH   GPIO_SetBits(GPIOD, GPIO_Pin_1 )
#define HX711_SCK_LOW    GPIO_ResetBits(GPIOD, GPIO_Pin_1 )
	
#define HX711_DOUT   		 GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)

//#define HX711_SCK_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_8 )
//#define HX711_SCK_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_8 )
//	
//#define HX711_DOUT   		 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
	
void HX711_Config(void);
long HX711_Read(void);			//增益128

	
float Read_weight(void);
void Setzero_Compensate(void);
void Readzero_Compensate(void);
void SetCal_Ratio(unsigned int normal);
void ReadCal_Ratio(void);



extern long ZeroValue;
extern float Ratio;



#ifdef __cplusplus		   				//定义对CPP进行C处理 //结束部分
}




#endif

#endif


