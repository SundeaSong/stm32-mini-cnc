#ifndef _GPIO_INIT_H_	
#define _GPIO_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif

#include "Config.h"

#define ON  0
#define OFF 1
	
//X1(PE2) 	X2(PE3) 	X3(PE4) 	X4(PE5) 	Axis home sensor
//x5(PE6) 	x6(PF0) 	x7(PF1) 	x8(PF2) 	axis pos sensor
//x9(PF3) 	x10(PF4) 	x11(PF5) 	x12(PF6) 	axis neg sensor
	
#define IN_X1			GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define IN_X2			GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define IN_X3			GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)
#define IN_X4			GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)
#define IN_X5			GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6)
#define IN_X6			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0)
#define IN_X7			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1)
#define IN_X8			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2)
	
#define IN_X9			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3)
#define IN_X10			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4)
#define IN_X11			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_5)
#define IN_X12			GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)

//Pluse1(PC6) 	Pluse2(PC7) 	Pluse3(PC8) 	Pluse4(PC9)
//Dir1(PD12) 		Dir2(PD13) 		Dir3(PD14) 		Dir4(PD15)

#define OUT_Y1    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define Y1_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define Y1_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_6)

#define OUT_Y2    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define Y2_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_7)
#define Y2_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_7)

#define OUT_Y3    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define Y3_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define Y3_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define OUT_Y4    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define Y4_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define Y4_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_9)

#define OUT_Y5    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12)
#define Y5_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define Y5_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_12)

#define OUT_Y6    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13)
#define Y6_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define Y6_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_13)

#define OUT_Y7    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14)
#define Y7_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define Y7_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_14)

#define OUT_Y8    GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15)
#define Y8_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_15)
#define Y8_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_15)

#define PWR_ON   GPIO_ResetBits(GPIOD, GPIO_Pin_11);GPIO_SetBits(GPIOD, GPIO_Pin_10)
#define PWR_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_11);GPIO_ResetBits(GPIOD, GPIO_Pin_10)

typedef union
{
    struct
    {
        unsigned X_1   : 1;
        unsigned X_2   : 1;
        unsigned X_3   : 1;
        unsigned X_4   : 1;
        unsigned X_5   : 1;
        unsigned X_6   : 1;
        unsigned X_7   : 1;
        unsigned X_8   : 1;

        unsigned X_9   : 1;
        unsigned X_10   : 1;
        unsigned X_11   : 1;
        unsigned X_12   : 1;
        unsigned X_13   : 1;
        unsigned X_14   : 1;
        unsigned X_15   : 1;
        unsigned X_16   : 1;
    }ONE;
    unsigned int ALL;
}Type_Input_Port;

extern Type_Input_Port Input_Port;

typedef union
{
    struct
    {
        unsigned Y_1   : 1;
        unsigned Y_2   : 1;
        unsigned Y_3   : 1;
        unsigned Y_4   : 1;
        unsigned Y_5   : 1;
        unsigned Y_6   : 1;
        unsigned Y_7   : 1;
        unsigned Y_8   : 1;

        unsigned Y_9   : 1;
        unsigned Y_10   : 1;
        unsigned Y_11   : 1;
        unsigned Y_12   : 1;
        unsigned Y_13   : 1;
        unsigned Y_14   : 1;
        unsigned Y_15   : 1;
        unsigned Y_16   : 1;
    }ONE;
    unsigned short ALL;
}Type_Output_Port;

extern Type_Output_Port Output_Port;

void GPIO_Config(void);
void OutputControl(unsigned int stats);
unsigned int InputRead(void);
unsigned int OutputRead(void);
//unsigned char Input_L_Value(unsigned int num);
//unsigned char Input_H_Value(unsigned int num);


#ifdef __cplusplus		  
}
#endif

#endif
