#include <stdint.h>
#include "nuts_bolts.h"
#include "GPIO_init.h"
#include "ff.h"

#ifndef __CNC_H
#define __CNC_H
 

struct TJob {
  unsigned char id;
  unsigned char type;
  unsigned char group;
	float	X1;
	float	Y1;
	float	X2;
	float	Y2;
	float	A1;
	float	A2;
	unsigned int speed;
	struct TJob *next;
};
extern unsigned int Rpm;
extern char ReverseY;
extern void init_grbl(void);
extern char DDRD,DDRB,DDRA,DDRC,PORTA,PORTB,PORTC,PORTD,PINB;


#define CNC_SPEED  150
extern float Radius,dX,dY,step_Z,Dest_Z,XOffset,YOffset,XStep,YStep;
extern char CancelAll;
extern long int currX,currY,currZ;
extern struct TJob *Job,*JobCurrent;
extern int lcd_x,lcd_y,Opakuj,GlobalDelay,StepsPerMM,mem_use,XCount,YCount;
extern void add_job(unsigned char id, unsigned char group, unsigned char type, float X1, float Y1, float X2, float Y2, float A1, float A2, unsigned int speed);
extern void delete_all_jobs(void);
extern int Pos(char ch, char * str);
extern char BoolArray_Get( char *array, int pos);
extern void BoolArray_Set( char *array, int pos, char val);
extern void printpos(int y);
extern void printpos_grbl(int x ,int y);
extern void beep(int i);
extern unsigned int GetInt(char *Text,unsigned int defval, unsigned int min, unsigned int max);
extern float Getfloat(char *Text,float defval, float min, float max);
extern unsigned char MessageBox(char *Text, char btnOK, char btnYes, char btnNo,char btnCancel, char btnCount);
extern void MotorsEnable(char motors);
extern void MotorsSetGPIO(void);
extern void Motor_Delay(unsigned int nCount);
extern int cncMoveToXY(long int NewX, long int NewY, int minDelay, char StartStop);
extern int cncMoveToZ(long int NewZ, int minDelay, char StartStop);
extern int MoveToXY(float X, float Y, int minDelay,char StartStop);
extern int MoveToZ(float Z, int minDelay,char StartStop);
extern void mm2steps(float X,float Y, long int *x, long int *y);	//	Skonvertujem udaj v milimetroch na kroky motora
extern void cncArc(float x, float y, float radius, float s_e, float e_e, unsigned char cc);
extern void cncCircle(float x, float y, float radius, unsigned char cc);
extern void setup1(void);
extern void cnc_menu1(void);
extern void cnc_menu1_circle(void);
extern void cnc_menu1_rect(void);
extern void setup_z(void);
extern void setup_spindle(void);
extern char GetAbort(void);

//extern FRESULT select_file( char*  path);
//extern FRESULT LoadFile(char *filename);
//extern char filename[256]; /* current file name                   */

extern _Bool DisplayOnly;
#define FAST_DELAY	30


/************????*************************************************************************************/
//X1(PE2) 	X2(PE3) 	X3(PE4) 	X4(PE5) 	Axis home sensor
//x5(PE6) 	x6(PF0) 	x7(PF1) 	x8(PF2) 	axis pos sensor
//x9(PF3) 	x10(PF4) 	x11(PF5) 	x12(PF6) 	axis neg sensor

//Y1(PG8) 	Y2(PG7) 	Y3(PG6) 	Y4(PG5)	 ?????

//Pluse1(PC6) 	Pluse2(PC7) 	Pluse3(PC8) 	Pluse4(PC9)
//Dir1(PD12) 		Dir2(PD13) 		Dir3(PD14) 		Dir4(PD15)
//EN1 (PG12) 		EN1 (PG11) 		EN1 (PG10) 		EN1 (PG9) 

//PEn1(PE1) 		PEn2(PE0) 		PEn3(PB9) 		PEn4(PB8)  ??IC??

#define BEEP_ON 		GPIO_ResetBits(GPIOG, GPIO_Pin_8)		//Y1 蜂鸣器
#define BEEP_OFF 		GPIO_SetBits(GPIOG, GPIO_Pin_8)	

#define FAN_ON 			GPIO_ResetBits(GPIOG, GPIO_Pin_7)		//Y2 风扇 Collant 冷却
#define FAN_OFF 		GPIO_SetBits(GPIOG, GPIO_Pin_7)			//风扇

#define SPINDLE_ON		TIM_CtrlPWMOutputs(TIM1, ENABLE);		//黑 主轴 激光控制
#define SPINDLE_OFF		TIM_CtrlPWMOutputs(TIM1, DISABLE);		//关闭输出

#define MOTOR1_DISABLE   GPIO_ResetBits(GPIOG, GPIO_Pin_12)			//E1白 X使能
#define MOTOR1_ENABLE    GPIO_SetBits(GPIOG, GPIO_Pin_12)			

#define MOTOR2_DISABLE   GPIO_ResetBits(GPIOG, GPIO_Pin_11)			//E2橙 Y
#define MOTOR2_ENABLE    GPIO_SetBits(GPIOG, GPIO_Pin_11)	

#define MOTOR3_DISABLE   GPIO_ResetBits(GPIOG, GPIO_Pin_10)			//E3蓝 Z
#define MOTOR3_ENABLE    GPIO_SetBits(GPIOG, GPIO_Pin_10)


//#define MOTOR1_LEFT     OUT_Y4_OFF				//紫 X轴方向
//#define MOTOR1_RIGHT   	OUT_Y4_ON

//#define MOTOR2_LEFT     OUT_Y12_OFF				//棕 y轴方向
//#define MOTOR2_RIGHT   	OUT_Y12_ON	

//#define MOTOR3_UP	    	OUT_Y5_OFF				//黄 Z轴方向
//#define MOTOR3_DOWN    	OUT_Y5_ON	


//#define MOTOR1_PLUSE_L    OUT_Y1_OFF			//灰 X轴脉冲
//#define MOTOR1_PLUSE_H   	OUT_Y1_ON	

//#define MOTOR2_PLUSE_L    OUT_Y11_OFF			//红 Y
//#define MOTOR2_PLUSE_H    OUT_Y11_ON

//#define MOTOR3_PLUSE_L    OUT_Y7_OFF			//绿 Z
//#define MOTOR3_PLUSE_H   	OUT_Y7_ON


//#define LIMIT_SWITCHES_ACTIVE_HIGH		//极限开关动作时为H

#define LIMIT_PIN       InputRead();

#define X_LIMIT_BIT     0  
#define Y_LIMIT_BIT     1  
#define Z_LIMIT_BIT     2  

#define LIMIT_MASK ((1<<X_LIMIT_BIT)|(1<<Y_LIMIT_BIT)|(1<<Z_LIMIT_BIT)) // All limit bits


#define STEPPING_PORT      OutputRead()

#define X_STEP_BIT         0   // Y1
#define Y_STEP_BIT         1   // Y2
#define Z_STEP_BIT         2   // Y3

#define X_DIRECTION_BIT    4   // Y5
#define Y_DIRECTION_BIT    5   // Y6
#define Z_DIRECTION_BIT    6   // Y7

#define STEP_MASK ((1<<X_STEP_BIT)|(1<<Y_STEP_BIT)|(1<<Z_STEP_BIT)) // All step bits
#define DIRECTION_MASK ((1<<X_DIRECTION_BIT)|(1<<Y_DIRECTION_BIT)|(1<<Z_DIRECTION_BIT)) // All direction bits
#define STEPPING_MASK (STEP_MASK | DIRECTION_MASK) // All stepping-related bits (step/direction)

 
#endif

