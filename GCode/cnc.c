#include "cnc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "Config.h"
#include "Grbl_config.h"
#include "planner.h"
#include "nuts_bolts.h"
#include "stepper.h"
#include "spindle_control.h"
#include "coolant_control.h"
#include "motion_control.h"
#include "gcode.h"
#include "protocol.h"
#include "limits.h"
#include "planner.h"
#include "report.h"
#include "settings.h"

 
struct TJob *Job,*JobCurrent;
char CancelAll;
char ReverseY=0;
unsigned int Rpm = 5000;
int Opakuj = 1;
int GlobalDelay=CNC_SPEED,StepsPerMM=800,mem_use = 0;
int lcd_x,lcd_y;
long int currX,currY,currZ,d_max;
int LastXDelay,LastYDelay,LastZDelay,XCount,YCount;
static __IO uint32_t TimingDelay;
unsigned int max_s_lim,accel_lim;
int nextDelay;
_Bool DisplayOnly = 0;

char DDRD,DDRB,DDRA,DDRC,PORTA,PORTB,PORTC,PORTD,PINB;
system_t sys;  
float Radius=10,dX=10,dY=10,step_Z = 0.2,Dest_Z,XOffset,YOffset,XStep,YStep;



void init_grbl(void)
{
      // Reset system.
			serial_reset_read_buffer();
      plan_init(); // Clear block buffer and planner variables
      gc_init(); // Set g-code parser to default state
      protocol_init(); // Clear incoming line data and execute startup lines
      spindle_init();
      coolant_init();
      limits_init();	//Koncove dorazy - cez interrupt
      st_reset(); // Clear stepper subsystem variables.

      // Sync cleared gcode and planner positions to current system position, which is only
      // cleared upon startup, not a reset/abort. 
      sys_sync_current_position();

      // Reset system variables.
      sys.abort = false;
      sys.execute = 0;
      if (bit_istrue(settings.flags,BITFLAG_AUTO_START)) { sys.auto_start = true; }
      
      // Check for power-up and set system alarm if homing is enabled to force homing cycle
      // by setting Grbl's alarm state. Alarm locks out all g-code commands, including the
      // startup scripts, but allows access to settings and internal commands. Only a homing
      // cycle '$H' or kill alarm locks '$X' will disable the alarm.
      // NOTE: The startup script will run after successful completion of the homing cycle, but
      // not after disabling the alarm locks. Prevents motion startup blocks from crashing into
      // things uncontrollably. Very bad.
      #ifdef HOMING_INIT_LOCK
        if (sys.state == STATE_INIT && bit_istrue(settings.flags,BITFLAG_HOMING_ENABLE)) { sys.state = STATE_ALARM; }
      #endif
      
      // Check for and report alarm state after a reset, error, or an initial power up.
      if (sys.state == STATE_ALARM) {
        report_feedback_message(MESSAGE_ALARM_LOCK); 
      } else {
        // All systems go. Set system to ready and execute startup script.
        sys.state = STATE_IDLE;
        protocol_execute_startup(); 
      } 
}
//char GetAbort(void)
//{
//	SetFont(1);
//	SetFontSize(1,1,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	delete_all_objects();
//	button(100,100 ,200,150,"KONIEC",1);
//	button(500,100,200,150,"POKRACUJ",2);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			LCD_Clear(LIME);
//			Action1 = 0;
//			return 1;
//		}
//		if(Action1 == 2)
//		{
//			Action1 = 0;
//			LCD_Clear(LIME);
//			return 0;
//		}
//	}	
//}

//char BoolArray_Get( char *array, int pos){
//		char val;
//		val = array[pos / 8];
//		val = val & (1 << (7 - (pos % 8)));
//		if(val) val = 1;	
//		return val;
//}
//void BoolArray_Set( char *array, int pos, char val){
//		if(val)
//			array[pos/8] = array[pos / 8] | (1 << (7 - (pos % 8)));
//		else
//			array[pos/8] = array[pos / 8] & ~(1 << (7 - (pos % 8)));
//		if(val) val = 1;	
//}

///**
// * @brief  Initialize and start the SysTick counter and its interrupt.
// *
// * @param  uint32_t ticks is the number of ticks between two interrupts
// * @return  none
// *
// * Initialise the system tick timer and its interrupt and start the
// * system tick timer / counter in free running mode to generate 
// * periodical interrupts.
// */


//void add_job(unsigned char id, unsigned char group,unsigned char type, float X1, float Y1, float X2, float Y2, float A1, float A2, unsigned int speed){
//    struct TJob *p;
//    if((p=malloc(sizeof(struct TJob)))==NULL)
//    {
//				LCD_Clear(RED);
//				SetFgColor(BLACK);
//        FontXScale = 2;
//        FontYScale = 2;
//				PutText(0,30, GetMaxX(), 80, "malloc() error. No Space", ALINE_CENTER);
//				while(1);
//    }
//		mem_use += sizeof(struct TJob);
//    p->id = id; 
//    p->type = type; 
//		p->group = group;
//    p->X1 = X1; 
//    p->Y1 = Y1; 
//    p->X2 = X2; 
//    p->Y2 = Y2; 
//    p->A1 = A1; 
//    p->A2 = A2; 
//    p->speed = speed; 
//		p->next = NULL;
//		if(Job == NULL)	//	Zatial prazdny zoznam
//		{
//			JobCurrent = p;
//			Job = JobCurrent;
//		}else
//		{
//			JobCurrent->next = p;	//	Predosly objekt bude ukazovat na aktualny
//			JobCurrent = p;				//	Predosly objekt uz je aktualny
////	  	p->next = Job;
////			Job = p;
//		}
//}
//int Pos(char ch, char * str){
//		int i;
//		for(i = 0; i < strlen(str);i++)
//		{
//				if(str[i] == ch)
//						return i;
//		}
//		return -1;
//}
void delete_all_jobs(void){
 	struct TJob *temp;
	temp = Job;	//	temp ukazuje na prvyv zozname=>posledne pridany objekt
	while(temp)
	{
	  Job=temp->next;
		free(temp);
		temp = Job;
	}
	mem_use = 0;
	Job = NULL;
}
void Motor_Delay(unsigned int nCount)
{
  nCount = nCount * 20;
  for(; nCount != 0; nCount--);
}
void MotorsSetGPIO(void){
//	GPIOD->BSRR = 1 << 2;
//	GPIOB->BSRR = 1 << 10;
//	GPIOA->BSRR = 1 << 0;
//	GPIOD->CRL &= 0xFFFFF0FF;	//	PD2 OUT - Step
//	GPIOD->CRL |= 0x00000200;	//	PD2 OUT
//	GPIOB->CRH &= 0xFFFFF0FF;	//	PB10 OUT - Dir
//	GPIOB->CRH |= 0x00000200;	//	PB10 OUT
//	GPIOA->CRL &= 0xFFFFFFF0;	//	PA0 OUT - Enable
//	GPIOA->CRL |= 0x00000002;	//	PA0 OUT
//	
//	GPIOB->BSRR = 1 << 9;
//	GPIOA->BSRR = 1 << 1;
//	GPIOB->BSRR = 1 << 8;
//	GPIOB->CRH &= 0xFFFFFF0F;	//	PB9 OUT - Step
//	GPIOB->CRH |= 0x00000020;	//	PB9 OUT
//	GPIOA->CRL &= 0xFFFFFF0F;	//	PA1 OUT - Dir
//	GPIOA->CRL |= 0x00000020;	//	PA1 OUT
//	GPIOB->CRH &= 0xFFFFFFF0;	//	PB8 OUT - Enable
//	GPIOB->CRH |= 0x00000002;	//	PB8 OUT
//	
//	GPIOA->BSRR = 1 << 2;
//	GPIOB->BSRR = 1 << 7;
//	GPIOA->BSRR = 1 << 3;
//	GPIOA->CRL &= 0xFFFFF0FF;	//	PA2 OUT - Step
//	GPIOA->CRL |= 0x00000200;	//	PA2 OUT
//	GPIOB->CRL &= 0x0FFFFFFF;	//	PB7 OUT - Dir
//	GPIOB->CRL |= 0x20000000;	//	PB7 OUT
//	GPIOA->CRL &= 0xFFFF0FFF;	//	PA3 OUT - Enable
//	GPIOA->CRL |= 0x00002000;	//	PA3 OUT
//	
//#define Y1        GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14)
//#define Y1_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_14)
//#define Y1_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_14)
//#define Y2        GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15)
//#define Y2_ON     GPIO_ResetBits(GPIOD, GPIO_Pin_15)
//#define Y2_OFF    GPIO_SetBits(GPIOD, GPIO_Pin_15)
//#define Y3        GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_2)
//#define Y3_ON     GPIO_ResetBits(GPIOG, GPIO_Pin_2)
//#define Y3_OFF    GPIO_SetBits(GPIOG, GPIO_Pin_2)
//#define Y4        GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3)
//#define Y4_ON     GPIO_ResetBits(GPIOG, GPIO_Pin_3)
//#define Y4_OFF    GPIO_SetBits(GPIOG, GPIO_Pin_3)
//#define Y5        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
//#define Y5_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_7)
//#define Y5_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_7)
//#define Y6        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
//#define Y6_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_8)
//#define Y6_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_8)
//#define Y7        GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
//#define Y7_ON     GPIO_ResetBits(GPIOC, GPIO_Pin_9)
//#define Y7_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_9)
//#define Y8        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)
//#define Y8_ON     GPIO_ResetBits(GPIOA, GPIO_Pin_8)
//#define Y8_OFF    GPIO_SetBits(GPIOA, GPIO_Pin_8)

	LastXDelay = 0;
	LastYDelay = 0;
	LastZDelay = 0;
	
	
}
void printpos_grbl(int x ,int y)
{
//	DebugPf("X=[%+08.3f] Y=[%+08.3f] Z=[%+08.3f]\r\n",(float)sys.position[X_AXIS]/ settings.steps_per_mm[X_AXIS],(float)sys.position[Y_AXIS]/ settings.steps_per_mm[Y_AXIS],(float)sys.position[Z_AXIS]/ settings.steps_per_mm[Z_AXIS]);

//	DebugPf("X=[%ld] Y=[%ld] Z=[%ld]\r\n",x_pos,y_pos,z_pos);

}

//void printpos(int y)
//{
///*	char buffer[200];
//	SetFontSize(1,1,2,FONT_LARGE);
//	sprintf(buffer,"    X=%.5ld,Y=%.5ld,Z=%.5ld",currX,currY,currZ);
//	PutText(0,y,790,0,buffer,ALINE_RIGHT);
//	sprintf(buffer,"    X=%07.3f,Y=%07.3f,Z=%07.3f",(float)currX / StepsPerMM,(float )currY / StepsPerMM,(float)currZ / StepsPerMM);
//	PutText(0,y-20,790,0,buffer,ALINE_RIGHT);*/
//	
//}
void MotorsEnable(char motors){
		if(motors & (1 << 0))
				MOTOR1_ENABLE;
		else
				MOTOR1_DISABLE;
		if(motors & (1 << 1))
				MOTOR2_ENABLE;
		else
				MOTOR2_DISABLE;
		if(motors & (1 << 2))
				MOTOR3_ENABLE;
		else
				MOTOR3_DISABLE;
		_delay_us(5);
}


//#define SYSTICK_MAXCOUNT       ((1<<24) -1)                                    

//double CalcDelay(int Steps)	//	Udava pocet krokov od zaciatku alebo do konca
//{
//	double Zrychlenie = 0.0001;
//	double VzialenostKroku = 0.001;
//	double TimeStep,TimeNextStep,StepTime;
//	TimeStep =  sqrt((2 * Steps * VzialenostKroku) / Zrychlenie);
//	TimeNextStep =  sqrt((2 * (Steps+1) * VzialenostKroku) / Zrychlenie);
//		StepTime = (6000 * (TimeNextStep - TimeStep))-1338;
////		StepTime = (8000 * (TimeNextStep - TimeStep))-1784;
//		//StepTime = (6000 * (TimeNextStep - TimeStep))-1338;
//	if (StepTime < 0) StepTime = 0;
//	return StepTime;
//}

//int MotorCalcNextAndDelay(unsigned int delay,uint32_t min_delay, long int pos, long int max, char startstop )
//{
//	int result;
//	long int watchdog=10000;
//	if(!min_delay) return 0;
//	pos++;
//	SysTick->LOAD  = (72) * delay - 1;      		/* set reload register */
//  SysTick->VAL   = 0;                                         /* Load the SysTick Counter Value */
//  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
//                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
//  
//  //	Toto pocitam az pocas cakania na koniec aktualneho delay
//	if((startstop & 0x03) == 0) result = min_delay;
//	else
//	{
//		if(((startstop & 0x01) && (pos < 300)) || ((startstop & 0x02) && (pos > (max - 300))) )
//		{
//			if((startstop & 0x01) && (pos < 300))
//			{	
//				result = CalcDelay(pos);
//			
//			}if((startstop & 0x02) && (pos > (max - 300)))
//			{	
//				result = CalcDelay(max-pos);
//			}
//		}
//		else
////		if(((startstop & 0x01) && (pos > 300)) && ((startstop & 0x02) && (pos < (max - 300))) )
//			result = min_delay;
//	}
//  while(!((SysTick->CTRL)&SysTick_CTRL_COUNTFLAG_Msk))
//	{
//			watchdog--;
//			if(!watchdog)
//				break;
//		
//	}
//	SysTick->CTRL=0x00;
//	return result + min_delay;
//}
//void cncArc(float x, float y, float radius, float s_e, float e_e, unsigned char cc)
//{	
//	float xi,yi,step,i;
//	long int x_steps,y_steps;
////	Najdem zaciatocny bod a presuniem sa tam
//	xi = x + (radius * cos(3.1415 * ((s_e - 90) / 180.0)));	
//	yi = y + (radius * sin(3.1415 * ((s_e - 90) / 180.0)));
//	mm2steps(xi,yi,&x_steps,&y_steps);
//	cncMoveToXY(x_steps,y_steps,GlobalDelay,0x03);
//	if(cc)
//	{
//		if(e_e < s_e) e_e = e_e + 360;
//		step = (e_e - s_e) / (unsigned int)(e_e - s_e);
//		i = s_e;
//		while(i <= e_e)
//		{
//			xi = x + (radius * cos(3.1415 * ((i - 90.0) / 180.0)));
//			yi = y + (radius * sin(3.1415 * ((i - 90.0) / 180.0)));
//			mm2steps(xi,yi,&x_steps,&y_steps);
//			if( i == s_e )
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x01);
//			else if(i == e_e)
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x02);
//			else
//					cncMoveToXY(x_steps,y_steps,GlobalDelay,0x00);
//			i += step;
//		}
//	}else
//	{
//		if(s_e < e_e) e_e = e_e - 360;
//		step = (s_e - e_e) / (unsigned int)(s_e - e_e);
//		i = s_e;
//		while(i >= e_e)
//		{
//			xi = x + (radius * cos(3.1415 * ((i - 90.0) / 180.0)));
//			yi = y + (radius * sin(3.1415 * ((i - 90.0) / 180.0)));
//			mm2steps(xi,yi,&x_steps,&y_steps);
//			if( i == s_e)
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x01);
//			else if(i == e_e )
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x02);
//			else
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x00);
//			i -= step;
//		}
//	}
//}

//void cncCircle(float x, float y, float radius, unsigned char cc)
//{	
//	int i,step;
//	float xi,yi;
//	long int x_steps,y_steps;
////	Najdem zaciatocny bod a presuniem sa tam
//	xi = x + (radius * cos(3.1415 * ((0 - 90) / 180.0)));	
//	yi = y + (radius * sin(3.1415 * ((0 - 90) / 180.0)));
//	mm2steps(xi,yi,&x_steps,&y_steps);
//	cncMoveToXY(x_steps,y_steps,GlobalDelay,0x03);
//	step = (30 / radius);
//	if(!step) step = 1;
//	if(cc)
//	{
//		for(i = step ; i<= 360; i += step)
//		{
//			xi = x + (radius * cos(3.1415 * ((i - 90) / 180.0)));
//			yi = y + (radius * sin(3.1415 * ((i - 90) / 180.0)));
//			mm2steps(xi,yi,&x_steps,&y_steps);
//			if( i == step )
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x01);
//			else if(i == 360 - step)
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x02);
//			else
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x00);
//		}
//	}else
//	{
//		for(i = 360 - step ; i>= 0 ; i -= step)
//		{
//			 xi = x + (int)(radius * cos(3.1415 * ((i - 90) / 180.0)));
//			 yi = y + (int)(radius * sin(3.1415 * ((i - 90) / 180.0)));
//			mm2steps(xi,yi,&x_steps,&y_steps);
//			if( i == 360 - step )
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x01);
//			else if(i == step)
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x02);
//			else
//				cncMoveToXY(x_steps,y_steps,GlobalDelay,0x00);
//		}
//	}
//}

//void mm2steps(float X,float Y, long int *x, long int *y)	//	Skonvertujem udaj v milimetroch na kroky motora
//{
//	*x = X * StepsPerMM;	//	Toto treba opravit na 800
//	*y = Y * StepsPerMM;
//}
//int MoveToXY(float X, float Y, int minDelay,char StartStop){
//	long int x,y;
//	mm2steps(X,Y,&x,&y);
//	return cncMoveToXY(x,y,minDelay,StartStop);
//}
//int MoveToZ(float Z, int minDelay,char StartStop){
//	long int z = Z * StepsPerMM;
//	return cncMoveToZ(z,minDelay,StartStop);
//}

//int cncMoveToXY(long int NewX, long int NewY, int minDelay, char StartStop){
//	long int i,dx,dy,my,mx,x_step,y_step;
//    
//	MotorsEnable(0x07);	//	zapnem vsetky 3 motory
//	TouchXPos = 0;
//	if (CancelAll) return 0;
//	if(StartStop & 0x01)
//		nextDelay = minDelay + CalcDelay(1);
//	else
//		nextDelay = minDelay;
//	if(StartStop & 0x80)	//	Fast move
//			SetColor(RED);
//	else									//	CNC obrabanie
//			SetColor(BLUE);
//	if(NewX >= currX)
//	{
//		MOTOR1_LEFT;
//		delay_us(5);
//		dx = NewX - currX;
//		x_step = 1;
//	}
//	else
//	{
//		MOTOR1_RIGHT;
//		delay_us(5);
//		dx = currX - NewX;
//		x_step = -1;
//	}
//	if(NewY >= currY)
//	{
//		MOTOR2_LEFT;
//		delay_us(5);
//		dy = NewY - currY;
//		y_step = 1;
//	}else
//	{
//		MOTOR2_RIGHT;
//		delay_us(5);
//		dy = currY - NewY;
//		y_step = -1;
//	}
//	d_max = dx;
//	if(dY >dX) d_max = dy;
//	if(dx >= dy)	//	Vacsia draha a tym padom aj rychlost v osi X
//	{
//		if(dy)
//			dy = (dx * 1000L) / dy;
//		else
//			dy = dx * 2000L;
//		my = 0;
//		for(i = 0; i< dx;i++)
//		{
//			if(TouchXPos)	//	Ak som stlacil cosi na displeji
//			{
//					beep(200);
//					TouchXPos = 0;	
//					while(!TouchXPos);
//					beep(200);
//					if(TouchXPos > 500)	//	CancelAll All
//					{
//						GlobalDelay = 150;
//						MoveToZ(-10,GlobalDelay,0x03);
//						CancelAll = 1;
//						TouchXPos = 0;	
//						break;
//					}
//					TouchXPos = 0;	
//			}
//			my += 1000;
//			if(!DisplayOnly)
//			MOTOR1->BRR = MOTOR1_PIN;
//			if(my >= dy)
//				if(!DisplayOnly)
//					MOTOR2->BRR = MOTOR2_PIN;
//			if(minDelay)
//				if(!DisplayOnly)
//					delay_us(5);
//			if(!DisplayOnly)
//				MOTOR1->BSRR = MOTOR1_PIN;
//			if(!DisplayOnly)
//				nextDelay = MotorCalcNextAndDelay(nextDelay,minDelay,i,dx,StartStop);
//			if(my >= dy)
//			{
//					if(!DisplayOnly)
//						MOTOR2->BSRR = MOTOR2_PIN;
//					my -= dy;
//					currY += y_step;
//			}
//			currX += x_step;
//			if((lcd_x != (currX / (StepsPerMM / 3.0))) || (lcd_y != (currY / (StepsPerMM / 3.0))))
//			{
//				lcd_x = (currX / (StepsPerMM / 3.0));
//				lcd_y = (currY / (StepsPerMM / 3.0));
//				DrawPixel(30 + lcd_x, 450 - lcd_y);
//			}
//		}
//	}else
//	{
//		if(dx)
//			dx = (dy * 1000L) / dx;
//		else
//			dx = dy * 2000L;
//		mx = 0;
//		for(i = 0; i< dy;i++)
//		{
//			if(TouchXPos)	//	Ak som stlacil cosi na displeji
//			{
//					beep(200);
//					TouchXPos = 0;	
//					while(!TouchXPos);
//					beep(200);
//					if(TouchXPos > 500)	//	CancelAll All
//					{
//						MoveToZ(-10,GlobalDelay,0x03);
//						CancelAll = 1;
//						TouchXPos = 0;	
//						break;
//					}
//					TouchXPos = 0;	
//			}
//			mx += 1000;
//			if(!DisplayOnly)
//				MOTOR2->BRR = MOTOR2_PIN;
//			if(mx >= dx)
//				if(!DisplayOnly)
//					MOTOR1->BRR = MOTOR1_PIN;
//			if(minDelay)
//				if(!DisplayOnly)
//					delay_us(5);
//			if(!DisplayOnly)
//				MOTOR2->BSRR = MOTOR2_PIN;
//			if(!DisplayOnly)
//				nextDelay = MotorCalcNextAndDelay(nextDelay,minDelay,i,dy,StartStop);
//			if(mx >= dx)
//			{
//					if(!DisplayOnly)
//						MOTOR1->BSRR = MOTOR1_PIN;
//					mx -= dx;
//					currX += x_step;
//			}
//			currY += y_step;
//			if((lcd_x != (currX / (StepsPerMM / 3.0))) || (lcd_y != (currY / (StepsPerMM / 3.0))))
//			{
//				lcd_x = (currX / (StepsPerMM / 3.0));
//				lcd_y = (currY / (StepsPerMM / 3.0));
//				DrawPixel(30 + lcd_x, 450 - lcd_y);
//			}
//		}
//	}
//	return 1;
//}
//int cncMoveToZ(long int NewZ, int minDelay, char StartStop){
//	long int i,dz,z_step;
//	MotorsEnable(0x07);	//	zapnem vsetky 3 motory
//	TouchXPos = 0;
//	if (CancelAll) return 0;
//	if(StartStop & 0x01)
//		nextDelay = minDelay + CalcDelay(1);
//	else
//		nextDelay = minDelay;
//	if(NewZ >= currZ)
//	{
//		MOTOR3_DOWN;
//		delay_us(5);
//		dz = NewZ - currZ;
//		z_step = 1;
//	}
//	else
//	{
//		MOTOR3_UP;
//		delay_us(5);
//		dz = currZ - NewZ;
//		z_step = -1;
//	}
//	for(i = 0; i< dz;i++)
//	{
//		if(TouchXPos)	//	Ak som stlacil cosi na displeji
//		{
//				beep(200);
//				TouchXPos = 0;	
//				while(!TouchXPos);
//				beep(200);
//				if(TouchXPos > 500)	//	CancelAll All
//				{
//					GlobalDelay = 150;
//					MoveToZ(-10,GlobalDelay,0x03);
//					CancelAll = 1;
//					TouchXPos = 0;	
//					break;
//				}
//				TouchXPos = 0;	
//		}
//		if(!DisplayOnly)
//			MOTOR3->BRR = MOTOR3_PIN;
//		if(minDelay)
//			if(!DisplayOnly)
//				delay_us(5);
//		if(!DisplayOnly)
//			MOTOR3->BSRR = MOTOR3_PIN;
//		if(!DisplayOnly)
//			nextDelay = MotorCalcNextAndDelay(nextDelay,minDelay,i,dz,StartStop);
//		currZ += z_step;
//	}
//	return 1;
//}

//void drawcalcbuttons(void)
//{
//	button(1,1,100,100,"0",10+0);
//	button(220,1,100,100,"<-",3);
//	button(1,110,100,100,"1",10+1);
//	button(110,110,100,100,"2",10+2);
//	button(220,110,100,100,"3",10+3);
//	button(1,220,100,100,"4",10+4);
//	button(110,220,100,100,"5",10+5);
//	button(220,220,100,100,"6",10+6);
//	button(1,330,100,100,"7",10+7);
//	button(110,330,100,100,"8",10+8);
//	button(220,330,100,100,"9",10+9);
//	button(400,1,300,100,"OK",1);
//	button(400,150,300,100,"Cancel",2);
//	button(110,1,100,100,",",4);
//  
//}
//unsigned char MessageBox(char *Text, char btnOK, char btnYes, char btnNo,char btnCancel, char btnCount)
//{
//	int btnWidth = 200;
//	int btnLeft = (800 / btnCount - btnWidth) / 2;
//	int Space = btnLeft * 2;
//  delete_all_objects();
//  LCD_Clear(rgb(0,0,0));
//	SetFontSize(2,2,2,FONT_SMALL);
//	SetBkColor(BLACK);
//	SetFgColor(YELLOW);
//	PutText(1,400,798,0, Text, ALINE_CENTER);
//	if(btnOK)
//	{
//		button(btnLeft,100,btnWidth,200,"OK",1);
//		btnLeft += btnWidth + Space;
//	}
//	if(btnYes)
//	{
//		button(btnLeft,100,btnWidth,200,"Yes",2);
//		btnLeft += btnWidth + Space;
//	}
//	if(btnNo)
//	{
//		button(btnLeft,100,btnWidth,200,"No",3);
//		btnLeft += btnWidth + Space;
//	}
//	if(btnCancel)
//	{
//		button(btnLeft,100,btnWidth,200,"Cancel",4);
//		btnLeft += btnWidth + Space;
//	}
//	Action1 = 0;
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1)
//		{	
//			int x = Action1;
//			Action1 = 0;
//			return x;
//		}					
//	}
//	
//}
//unsigned int GetInt(char *Text,unsigned int defval, unsigned int min, unsigned int max)
//{
//	char buffer[100],bufferpos,first=1;
//  delete_all_objects();
//  LCD_Clear(rgb(0,0,0));
//	drawcalcbuttons();	//	Vykreslim klavesnicu 0-9,....
//	SetFontSize(2,2,2,FONT_SMALL);
//	SetBkColor(BLACK);
//	SetFgColor(YELLOW);
//	PutText(500,400,0,0, Text, ALINE_LEFT);
//	sprintf(buffer,"Opakovani=%d  Delay=%d MemoryCNC=%dB,WIDGETS=%dB",Opakuj,GlobalDelay,mem_use,mem_use1);
//	PutText(10,440,0, 0, buffer, ALINE_LEFT);
//	SetBkColor(YELLOW);
//	SetFontSize(3,3,3,FONT_SMALL);
//	SetFgColor(BLACK);
//	LCD_Rect(500,300,500+150,350,YELLOW);
//	bufferpos = sprintf(buffer,"%d",defval);
//	PutText(500,300,640,0, buffer, ALINE_RIGHT);
//	Action1 = 0;
//	while(1)
//	{
//		draw_press_unpress();
////		FontNo=FONT_DIGI;
//		SetBkColor(YELLOW);
//		if(Action1 == 3)	//	Pressed backspace
//		{	
//		  	if(bufferpos > 0)
//			{
//		  		bufferpos--;
//				buffer[bufferpos] = 0;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,640,0, buffer, ALINE_RIGHT);
//			Action1 = 0;
//		}					
//		if(Action1 >= 10)	//	Pressed number 0-9
//		{	
//			if(first)
//			{
//				first = 0;
//				bufferpos = 0;
//				memset(buffer,0,sizeof(buffer));				
//			}
//	  	if(bufferpos < 6)
//			{
//				buffer[bufferpos] = 48 + Action - 10;
//		  		bufferpos++;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,640,0, buffer, ALINE_RIGHT);
//			Action1 = 0;
//		}					
//		if(Action1 == 2)	//	Cancel
//		{
//		  	return defval;
//		}					
//		if(Action1 == 1)	//	OK
//		{
//		  	unsigned long val=atol(buffer);
//		  	if((val < (long)min) || (val > (long)max))
//			{
//				beep(100);
//			 	Action1 = 0;
//			} else
//			  	return atoi(buffer);
//		}					
//	}
//}
//float Getfloat(char *Text,float defval, float min, float max)
//{
//	char buffer[100],bufferpos,first=1;
//  delete_all_objects();
//  LCD_Clear(rgb(0,0,0));
//	drawcalcbuttons();	//	Vykreslim klavesnicu 0-9,....
//	button(330,330,100,100,"-",5);
//	SetFontSize(2,2,2,FONT_SMALL);
//	SetBkColor(BLACK);
//	SetFgColor(YELLOW);
//	PutText(500,400,0,0, Text, ALINE_LEFT);
//	sprintf(buffer,"Opakovani=%d  Delay=%d MemoryCNC=%dB,WIDGETS=%dB",Opakuj,GlobalDelay,mem_use,mem_use1);
//	PutText(10,440,0, 0, buffer, ALINE_LEFT);
//	SetBkColor(YELLOW);
//	SetFontSize(3,3,3,FONT_SMALL);
//	SetFgColor(BLACK);
//	LCD_Rect(500,300,500+150,350,YELLOW);
//	bufferpos = sprintf(buffer,"%6.3f",defval);
//	PutText(500,300,140,0, buffer, ALINE_LEFT);
//	Action1 = 0;
//	while(1)
//	{
//		draw_press_unpress();
////		FontNo=FONT_DIGI;
//		SetBkColor(YELLOW);
//		if(Action1 == 3)	//	Pressed backspace
//		{	
//		  if(bufferpos > 0)
//			{
//		  	bufferpos--;
//				buffer[bufferpos] = 0;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,140,0, buffer, ALINE_LEFT);
//			Action1 = 0;
//		}					
//		if(Action1 >= 10)	//	Pressed number 0-9
//		{	
//			if(first)
//			{
//				first = 0;
//				bufferpos = 0;
//				memset(buffer,0,sizeof(buffer));				
//			}
//		  if(bufferpos < 9)
//			{
//				buffer[bufferpos] = 48 + Action - 10;
//		  		bufferpos++;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,140,0, buffer, ALINE_LEFT);
//			Action1 = 0;
//		}					
//		if(Action1 == 4)	//	  ciarka
//		{	
//			if(first)
//			{
//				first = 0;
//				bufferpos = 0;
//				memset(buffer,0,sizeof(buffer));				
//			}
//		  if(bufferpos < 9)
//			{
//				buffer[bufferpos] = '.';
//		  		bufferpos++;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,140,0, buffer, ALINE_LEFT);
//			Action1 = 0;
//		}					
//		if(Action1 == 5)	//	  Minus
//		{	
//			if(first)
//			{
//				first = 0;
//				bufferpos = 0;
//				memset(buffer,0,sizeof(buffer));				
//			}
//		  if(bufferpos < 9)
//			{
//				buffer[bufferpos] = '-';
//		  		bufferpos++;
//			}
//			LCD_Rect(500,300,500+150,350,YELLOW);
//			PutText(500,300,140,0, buffer, ALINE_LEFT);
//			Action1 = 0;
//		}					
//		if(Action1 == 2)	//	Cancel
//		{
//		  	return defval;
//		}					
//		if(Action1 == 1)	//	OK
//		{
//		  float val=atof(buffer);
//		  if((val < min) || (val > max))
//			{
//				beep(100);
//			 	Action1 = 0;
//			} else return val;
//		}					
//	}
//}
//void setup1(void)
//{
//	SetFont(1);
//	SetFontSize(1,1,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	delete_all_objects();
//	button(1,10 ,200,150,"Opakovani",1);
//	button(1,170,200,150,"Delay",2);
//	button(1,330,200,150,"Steps/mm",3);
//	button(590,10,200,150,"Cancel",4);
//	button(590,330,200,150,"Restart BOARD",5);
//	button(590,170,200,150,"Draw primitives",6);
//	button(385,330,200,150,"Z setup",7);
//	button(385,10,200,150,"Load File",8);
//	button(385,170,200,150,"goto 0,0,0",9);
//	button(205,170,175,150,"min delay",10);
//	button(205,10,175,150,"Calibrate touch",11);	
//	button(205,330,175,150,"Feed rate",12);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			Action1 = 0;
//			Opakuj = GetInt("Opakovani",Opakuj, 1, 10000);
//			Action1 = 1;
//			break;
//		}
//		if(Action1 == 2)
//		{
//			Action1 = 0;
//			GlobalDelay = GetInt("Delay",GlobalDelay, 0, 1000000);
//			Action1 = 1;
//			break;
//		}
//		if(Action1 == 3)
//		{
//			Action1 = 0;
//			StepsPerMM = GetInt("Steps/mm",StepsPerMM, 1, 100000);
//			Action1 = 1;
//			break;
//		}
//		if(Action1 == 4)
//		{
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 5)
//		{
//			NVIC_SystemReset();
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 6)
//		{
//			Action1 = 0;
//			cnc_menu1();
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 7)
//		{
//			Action1 = 0;
//			setup_z();
//			break;
//		}
//		if(Action1 == 8)
//		{
//			select_file("");
////			LoadFile(filename);
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 9)
//		{
//			MoveToZ(-20.0,FAST_DELAY,0x03);
//			MoveToXY(0.0,0.0,FAST_DELAY,0x83);	//	Aj start aj stop
//			MoveToZ(0.0,FAST_DELAY,0x03);
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 10)
//		{
//			Action1 = 0;
//			settings.pulse_microseconds = GetInt("pulse",settings.pulse_microseconds, 1, 10000);
//			Action1 = 1;
//			break;
//		}
//		if(Action1 == 11)
//		{
//			Action1 = 0;
//			calibrate_touch();
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 12)
//		{
//			uint16_t feed = settings.default_feed_rate;
//			feed = GetInt("Feed rate",feed,0,10000);
//			settings.default_feed_rate = feed;
//			settings.default_seek_rate = feed;
//			EE_WriteVariable(32,feed);
//			Action1 = 0;
//			break;
//		}
//	}
//	
//}


//void cnc_menu1(void)
//{
//	SetFont(1);
//	SetFontSize(1,1,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	delete_all_objects();
//	button(10,10 ,200,150,"Circle",1);
//	button(10,170,200,150,"Rectangle",2);
//	button(590,10,200,150,"Cancel",3);
//	button(390,330,400,150,"X-krat otvor Xmm vzdialenost Xmm",4);
//	button(10,330,200,150,"ReverseY",5);
//	button(220,10,200,150,"AlignCNC",6);	//	Zarovnanie nastroja podla kontaktu na specifickom pine
//	button(590,170,200,150,"Feed rate",7);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			Action1 = 0;
//			cnc_menu1_circle();
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 2)
//		{
//			Action1 = 0;
//			cnc_menu1_rect();
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 3)
//		{
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 4)
//		{
//				double polomer = 2.0, vzdialenost = 40.0,hlbka = 3.0;
//				int i,pocet = 2;
//				long SavedZ = currZ,SavedX=currX,SavedY=currY;
//				polomer = Getfloat("Polomer",polomer,-99999,99999);
//				hlbka = Getfloat("Hlbka",hlbka,-99999,99999);
//				vzdialenost = Getfloat("Vzdialenost",vzdialenost,-99999,99999);
//				pocet = GetInt("Pocet otvorov",pocet,1,1000);
//				LCD_Clear(LIME);
//				SavedX = currX;
//				SavedY = currY;
//				SavedZ = currZ;
//				for(i = 0; i< pocet ;i++)
//				{
//						cncMoveToZ(SavedZ,GlobalDelay,0x03);
//						while(currZ < (SavedZ + (hlbka * StepsPerMM)))
//						{
//								cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//								cncCircle((float)SavedX / StepsPerMM,	(float)SavedY / StepsPerMM,polomer,1);
//						}
//						cncMoveToZ(SavedZ,GlobalDelay,0x03);
//						if(i < pocet - 1)
//						{
//								MoveToZ(-20.0,FAST_DELAY,0x03);
//								cncMoveToXY(SavedX + (vzdialenost * StepsPerMM),SavedY,FAST_DELAY,0x83);
//								SavedX = currX;
//								MoveToZ(0.0,FAST_DELAY,0x03);
//						}
//				}
//				Action1 = 0;
//				break;
//		}
//		if(Action1 == 5)
//		{
//			ReverseY ^= 0x01;
//			break;
//		}
//		if(Action1 == 6)
//		{
//			unsigned int f = Rpm;
//			float pos;
//			RS485_stop();
//			RS485_start_left();
//			RS485_set_freq(5000);
//			SetFont(1);
//			SetFontSize(2,2,2,FONT_LARGE);
//			SetFgColor(BLACK);
//			SetBkColor(LIME);
//			SetColor(LIME);
//			LCD_Clear(GetColor());
//			SetColor(BLUE);
//			delete_all_objects();
//			PutText(10,400,0, 0, "Nastav rucnym posunom polohu nastoja", ALINE_LEFT);
//			button(100,100 ,200,150,"Align",1);
//			button(500,100 ,200,150,"Cancel",2);
//			MotorsEnable(0x00);
//			while(1)
//			{
//				draw_press_unpress();
//				if(Action1 == 1)
//				{
//					int i;
//					Action1 = 0;
//					MotorsEnable(0x07);	//	steppers enable
//					delay_ms(1);
//					if(!(GPIOD->IDR & (1 << 14)))
//					{
//							beep(3000);						//	je kontakt aj bez toho ze by mal byt
//					}else
//					{
//							MOTOR1_LEFT;
//							beep(50);
//							while((GPIOD->IDR & (1 << 14)))
//							{
//									MOTOR1->BRR = MOTOR1_PIN;
//									delay_ms(1);
//									MOTOR1->BSRR = MOTOR1_PIN;
//									delay_ms(1);
//							}
//							MOTOR1_RIGHT;
//							beep(300);
//							for(i = 0 ; i < (2*StepsPerMM);i++)	//	Vratim sa 2mm nazad
//							{
//									MOTOR1->BRR = MOTOR1_PIN;
//									delay_us(300);
//									MOTOR1->BSRR = MOTOR1_PIN;
//									delay_us(300);
//							}
//							beep(50);
//							MOTOR2_LEFT;
//							while((GPIOD->IDR & (1 << 14)))
//							{
//									MOTOR2->BRR = MOTOR2_PIN;
//									delay_ms(1);
//									MOTOR2->BSRR = MOTOR2_PIN;
//									delay_ms(1);
//							}
//							beep(300);
//							MOTOR2_RIGHT;
//							for(i = 0 ; i < (2*StepsPerMM);i++)	//	Vratim sa 2mm nazad
//							{
//									MOTOR2->BRR = MOTOR2_PIN;
//									delay_us(300);
//									MOTOR2->BSRR = MOTOR2_PIN;
//									delay_us(300);
//							}
//							beep(50);
//							MOTOR3_DOWN;
//							while((GPIOD->IDR & (1 << 14)))
//							{
//									MOTOR3->BRR = MOTOR3_PIN;
//									delay_ms(1);
//									MOTOR3->BSRR = MOTOR3_PIN;
//									delay_ms(1);
//							}
//							MOTOR3_UP;
//							beep(300);
//							for(i = 0 ; i < (2*StepsPerMM);i++)	//	Vratim sa 2mm nazad
//							{
//									MOTOR3->BRR = MOTOR3_PIN;
//									delay_us(300);
//									MOTOR3->BSRR = MOTOR3_PIN;
//									delay_us(300);
//							}
//							//	tu som v pozicii 2mm 2mm 2mm od vychodzej
//					}
//					pos = (float)sys.position[X_AXIS] / settings.steps_per_mm[X_AXIS]; 
//					pos = Getfloat("Zadajte sucansu X poziciu",pos,-1000.0,1000.0);
//					sys.position[X_AXIS] = (pos - 2.0 )* settings.steps_per_mm[X_AXIS]; 
//					
//					pos = (float)sys.position[Y_AXIS] / settings.steps_per_mm[Y_AXIS]; 
//					pos = Getfloat("Zadajte sucansu Y poziciu",pos,-1000.0,1000.0);
//					sys.position[Y_AXIS] = (pos - 2.0 )* settings.steps_per_mm[Y_AXIS]; 
//					
//					pos = (float)sys.position[Z_AXIS] / settings.steps_per_mm[Z_AXIS]; 
//					pos = Getfloat("Zadajte sucansu Z poziciu",pos,-1000.0,1000.0);
//					sys.position[Z_AXIS] = (pos + 2.0 )* settings.steps_per_mm[Z_AXIS]; 
//					sys_sync_current_position();
//					break;
//				}
//				if(Action1 == 2)
//				{
//					Action1 = 0;
//					break;
//				}
//			}
//			RS485_stop();
//			Rpm = f;
//			RS485_start_left();
//			RS485_set_freq(Rpm);	//	Obnovim povodne otacky
//			break;
//		}
//		if(Action1 == 7)
//		{
//			settings.acceleration = Getfloat("Feed acc",settings.acceleration,0.0,9000000.0);;
//			settings.default_feed_rate = GetInt("Feed rate",settings.default_feed_rate,0,10000);
//			gc.feed_rate = settings.default_feed_rate;
//			Action1 = 0;
//			break;
//		}
//		
//	}
//}

//void cnc_menu1_circle(void)
//{
//	int i;
//	float X,Y;
//	char buffer[50];

//znovu1:
//	X = (float)currX / StepsPerMM;
//	Y = (float)currY / StepsPerMM;
//znovu:
//	SetFont(1);
//	SetFontSize(2,2,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	sprintf(buffer,"X = %10.2f",X);
//	PutText(430,440,0,0,buffer,ALINE_LEFT);
//	sprintf(buffer,"Y = %10.2f",Y);
//	PutText(430,405,0,0,buffer,ALINE_LEFT);
//	sprintf(buffer,"Radius = %10.3f",Radius);
//	PutText(430,370,0,0,buffer,ALINE_LEFT);
//	printpos(355);
//	sprintf(buffer,"Opakovani=%dx",Opakuj);
//	PutText(550,460,790,0,buffer,ALINE_RIGHT);
//	delete_all_objects();
//	button(220,330,150,100,"from TOP",1);
//	button(220,170 ,150,100,"from CENTER",2);
//	button(220,10,150,100,"from BOTTOM",3);
//	button(10,170 ,150,100,"from LEFT",4);
//	button(430,170 ,150,100,"from RIGHT",5);
//	button(10,330 ,150,100,"Set Y",6);
//	button(430,10 ,150,100,"Set X",7);
//	button(10,10 ,150,100,"Radius",8);
//	button(640,170 ,150,100,"goto X,Y",9);
//	button(640,10,150,100,"Cancel",99);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				cncArc(X,Y+Radius,Radius,0,360,1);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 2)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				cncArc(X,Y,Radius,0,360,1);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 3)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				cncArc(X,Y-Radius,Radius,180,180+360,1);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 4)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				cncArc(X+Radius,Y,Radius,270,270+360,1);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 5)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				cncArc(X - Radius,Y,Radius,90,90 + 360,1);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 6)
//		{
//			Y = Getfloat("Y suradnica",Y,-99999,99999);
//			Action1 = 0;
//			goto znovu;
//		}
//		if(Action1 == 7)
//		{
//			X = Getfloat("X suradnica",X,-99999,99999);
//			Action1 = 0; 
//			goto znovu;
//		}
//		if(Action1 == 8)
//		{
//			Radius = Getfloat("Radius",Radius,-99999,99999);
//			Action1 = 0;
//			goto znovu;
//		}
//		if(Action1 == 9)
//		{
//			MoveToXY(X,Y,GlobalDelay,0x03);
//			Action1 = 0;
//			goto znovu;
//		}
//		if(Action1 == 99)
//		{
//			Action1 = 0;
//			break;
//		}
//	}
//	
//}
//void setup_z(void)
//{
//	float Z;
//	char buffer[50];

//znovu1:
//	Z = (float)currZ / StepsPerMM;
//znovu:
//	SetFont(1);
//	SetFontSize(2,2,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	sprintf(buffer,"Z = %10.2f",Z);
//	PutText(485,440,0,0,buffer,ALINE_LEFT);
//	sprintf(buffer,"stepZ= %10.2f",step_Z);
//	PutText(430,400,0,0,buffer,ALINE_LEFT);
//	printpos(355);
//	sprintf(buffer,"Opakovani=%dx",Opakuj);
//	PutText(550,460,790,0,buffer,ALINE_RIGHT);
//	delete_all_objects();
//	button(10,330 ,150,100,"Null Z",1);
//	button(220,330,150,100,"Set Z",2);
//	button(10,170 ,150,100,"UP 10mm",3);
//	button(10,10 ,150,100,"DOWN 10mm",4);
//	button(220,170 ,150,100,"Z step [mm]",5);
//	button(220,10 ,150,100,"goto Z",6);
//	button(430,170 ,150,100,"UP 50mm",7);
//	button(430,10 ,150,100,"DOWN 50mm",8);
//	button(640,170,150,100,"Disable",9);
//	button(640,10,150,100,"Cancel",99);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			currZ = 0;
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 2)
//		{
//			Z = Getfloat("Z suradnica",Z,-99999,99999);
//			Action1 = 0; 
//			goto znovu;
//		}
//		if(Action1 == 3)
//		{
//			MoveToZ(Z-10, GlobalDelay,0x03);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 4)
//		{
//			MoveToZ(Z+10, GlobalDelay,0x03);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 5)
//		{
//			step_Z = Getfloat("step_Z",step_Z,-99999,99999);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 6)
//		{
//			MoveToZ(Z, GlobalDelay,0x03);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 7)
//		{
//			MoveToZ(Z-50, FAST_DELAY * 2,0x03);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 8)
//		{
//			MoveToZ(Z+50, FAST_DELAY * 2,0x03);
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 9)
//		{
//			MotorsEnable(0x03);
//			Action1 = 0;
//		}
//		if(Action1 == 99)
//		{
//			Action1 = 0;
//			break;
//		}
//	}
//	
//}

//void cnc_menu1_rect(void)
//{
//	int i;
//	float X,Y;
//	char buffer[50];

//znovu1:
//	X = (float)currX / StepsPerMM;
//	Y = (float)currY / StepsPerMM;
//znovu:
//	SetFont(1);
//	SetFontSize(2,2,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	sprintf(buffer,"X = %10.2f",X);
//	PutText(430,440,0,0,buffer,ALINE_LEFT);
//	sprintf(buffer,"Y = %10.2f",Y);
//	PutText(430,405,0,0,buffer,ALINE_LEFT);
//	sprintf(buffer,"dX = %7.3f dY= %7.3f ",dX,dY);
//	PutText(430,370,0,0,buffer,ALINE_LEFT);
//	printpos(355);
//	sprintf(buffer,"Opakovani=%dx",Opakuj);
//	PutText(550,460,790,0,buffer,ALINE_RIGHT);
//	delete_all_objects();
//	button(220,330,150,100,"Set dY",1);
//	button(220,170 ,150,100,"Right-Top",2);
//	button(220,10,150,100,"Right-Bottom",3);
//	button(10,170 ,150,100,"Left-Top",4);
//	button(430,170 ,150,100,"Set dX",5);
//	button(430,10 ,150,100,"Set X",6);
//	button(10,330 ,150,100,"Set Y",7);
//	button(10,10 ,150,100,"Left-Bottom",8);
//	button(640,10,150,100,"Cancel",99);
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 4)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X,Y,GlobalDelay,0x03);
//				MoveToXY(X + dX, Y, GlobalDelay,0x03);
//				MoveToXY(X + dX, Y + dY, GlobalDelay,0x03);
//				if(!dX || !dY)
//					cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X, Y + dY, GlobalDelay,0x03);
//				MoveToXY(X, Y, GlobalDelay,0x03);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 3)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X,Y,GlobalDelay,0x03);
//				MoveToXY(X - dX, Y, GlobalDelay,0x03);
//				MoveToXY(X - dX, Y - dY, GlobalDelay,0x03);
//				if(!dX || !dY)
//					cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X, Y - dY, GlobalDelay,0x03);
//				MoveToXY(X, Y, GlobalDelay,0x03);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 8)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X , Y,GlobalDelay,0x03);
//				MoveToXY(X , Y - dY, GlobalDelay,0x03);
//				MoveToXY(X + dX, Y - dY, GlobalDelay,0x03);
//				if(!dX || !dY)
//					cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X + dX, Y , GlobalDelay,0x03);
//				MoveToXY(X, Y, GlobalDelay,0x03);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 2)
//		{
//			for(i = 0; i < Opakuj;i++)
//			{
//				cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X , Y, GlobalDelay,0x03);
//				MoveToXY(X , Y + dY, GlobalDelay,0x03);
//				MoveToXY(X - dX, Y + dY, GlobalDelay,0x03);
//				if(!dX || !dY)
//					cncMoveToZ(currZ + StepsPerMM *  step_Z,GlobalDelay * 5,0x03);
//				MoveToXY(X - dX, Y, GlobalDelay,0x03);
//				MoveToXY(X , Y,GlobalDelay,0x03);
//				sprintf(buffer,"%d from %d Z=%.3f",i+1,Opakuj,(float)currZ / StepsPerMM);
//				PutText(550,440,790,0,buffer,ALINE_RIGHT);
//			}
//			Action1 = 0;
//			goto znovu1;
//		}
//		if(Action1 == 7)
//		{
//			Y = Getfloat("Y suradnica",Y,-99999,99999);
//			Action1 = 0;
//			goto znovu;
//		}
//		if(Action1 == 6)
//		{
//			X = Getfloat("X suradnica",X,-99999,99999);
//			Action1 = 0; 
//			goto znovu;
//		}
//		if(Action1 == 1)
//		{
//			dY = Getfloat("dY",dY,-99999,99999);
//			Action1 = 0; 
//			goto znovu;
//		}
//		if(Action1 == 5)
//		{
//			dX = Getfloat("dX",dX,-99999,99999);
//			Action1 = 0; 
//			goto znovu;
//		}
//		if(Action1 == 99)
//		{
//			Action1 = 0;
//			break;
//		}
//	}
//}

//void setup_spindle(void)
//{
//	SetFont(1);
//	SetFontSize(1,1,2,FONT_LARGE);
//	SetFgColor(BLACK);
//	SetBkColor(LIME);
//	SetColor(LIME);
//	LCD_Clear(GetColor());
//	SetColor(BLUE);
//	delete_all_objects();
//	button(1,10 ,200,150,"Start",1);
//	button(1,170,200,150,"Stop",2);
//	button(1,330,200,150,"RESET",3);
//	button(590,10,200,150,"Cancel",4);
//	button(590,330,200,150,"Frequency",5);
//	button(590,170,200,150,"Start REVERS",6);
//	
//	while(1)
//	{
//		draw_press_unpress();
//		if(Action1 == 1)
//		{
//			Action1 = 0;
//			RS485_start_right();
//		}
//		if(Action1 == 2)
//		{
//			Action1 = 0;
//			RS485_stop();
//		}
//		if(Action1 == 3)
//		{
//			Action1 = 0;
//			RS485_reset();
//		}
//		if(Action1 == 4)
//		{
//			Action1 = 0;
//			break;
//		}
//		if(Action1 == 5)
//		{
//			Action1 = 0;
//			Rpm = GetInt("Zadaj otacky/min",Rpm,1,50000);
//			Action1 = 11;
//			RS485_set_freq(Rpm);
//			break;
//		}
//		if(Action1 == 6)
//		{
//			Action1 = 0;
//			RS485_start_left();
//		}
//	}
//	
//}




