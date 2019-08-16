#ifndef _RTC_INIT_H_	
#define _RTC_INIT_H_

#ifdef __cplusplus			//定义对CPP进行C处理
extern "C" {
#endif
	
#include "Config.h"

//公历日月年周
typedef struct 
{
	u8 hour;
	u8 min;
	u8 sec;                  
	u16 w_year;
	u8  w_month;
	u8  w_date;
	u8  week;             
}tm; 
extern tm timer;

void RTC_Config(void);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);
u8 RTC_Get_Week(u16 year,u8 month,u8 day);

#ifdef __cplusplus		  
}
#endif

#endif
