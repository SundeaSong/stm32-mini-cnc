#include "RTC_Init.h"
#include "BKP_Init.h"

tm timer;

/*******************************/
////////////////////////////////////////////////////////////////////////////////
// RTC时钟初始化！
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : 来重新配置RTC和BKP，仅在检测到后备寄存器数据丢失时使用
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
//    //启用PWR和BKP的时钟（from APB1）
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

//    //后备域解锁
//    PWR_BackupAccessCmd(ENABLE);

//    //备份寄存器模块复位
//    BKP_DeInit();

    //外部32.768K其哟偶那个
    RCC_LSEConfig(RCC_LSE_ON);
    //等待稳定
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

    //RTC时钟源配置成LSE（外部32.768K）
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    //RTC开启
    RCC_RTCCLKCmd(ENABLE);

    //开启后需要等待APB1时钟与RTC时钟同步，才能读写寄存器
    RTC_WaitForSynchro();

    //读写寄存器前，要确定上一个操作已经结束
    RTC_WaitForLastTask();

    //设置RTC分频器，使RTC时钟为1Hz
    //RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)
    RTC_SetPrescaler(32767);


    //等待寄存器写入完成
    RTC_WaitForLastTask();

    //使能秒中断
    RTC_ITConfig(RTC_IT_SEC, ENABLE);    

    //等待写入完成
    RTC_WaitForLastTask();

    return;
}

/*******************************************************************************
* Function Name  : RTC_Config
* Description    : 上电时调用本函数，自动检查是否需要RTC初始化， 
*                     若需要重新初始化RTC，则调用RTC_Configuration()完成相应操作
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Config(void)
{
    //我们在BKP的后备寄存器1中，存了一个特殊字符0xA5A5
    //第一次上电或后备电源掉电后，该寄存器数据丢失，
    //表明RTC数据丢失，需要重新配置
    if (BKP_ReadBackupRegister(BKP_DR2) != 0xA5A5)
    {
        //重新配置RTC
        RTC_Configuration();
        //配置完成后，向后备寄存器中写特殊字符0xA5A5
        BKP_WriteBackupRegister(BKP_DR2, 0xA5A5);
    }
    else
    {
        //若后备寄存器没有掉电，则无需重新配置RTC
        //这里我们可以利用RCC_GetFlagStatus()函数查看本次复位类型
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            //这是上电复位
        }
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            //这是外部RST管脚复位
        }
        //清除RCC中复位标志
        RCC_ClearFlag();

        //虽然RTC模块不需要重新配置，且掉电后依靠后备电池依然运行
        //但是每次上电后，还是要使能RTCCLK
        //RCC_RTCCLKCmd(ENABLE);
        //等待RTC时钟与APB1时钟同步
        //RTC_WaitForSynchro();
				
        //使能秒中断
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        //等待操作完成
        RTC_WaitForLastTask();
    }   
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
		
#ifdef RTCClockOutput_Enable
    /* Disable the Tamper Pin */
    BKP_TamperPinCmd(DISABLE); 
    /* Enable RTC Clock Output on Tamper Pin */
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif
    return;
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{                     
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			 if(year%400==0)return 1;//如果以00结尾,还要能被400整除          
			 else return 0;   
		}else return 1;   
	}else return 0; 
}                           

//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表                                                                        
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表   

//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	 u16 t;
	 u32 seccount=0;
	 if(syear<2000||syear>2099)return 1;//syear范围1970-2099，此处设置范围为2000-2099        
	 for(t=1970;t<syear;t++) //把所有年份的秒钟相加
	 {
			if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
			else seccount+=31536000;                    //平年的秒钟数
	 }
	 smon-=1;
	 for(t=0;t<smon;t++)         //把前面月份的秒钟数相加
	 {
			seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
			if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数         
	 }
	 seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	 seccount+=(u32)hour*3600;//小时秒钟数
	 seccount+=(u32)min*60;      //分钟秒钟数
	 seccount+=sec;//最后的秒钟加上去

	 //设置时钟
/*  RCC->APB1ENR|=1<<28;//使能电源时钟
RCC->APB1ENR|=1<<27;//使能备份时钟
	 PWR->CR|=1<<8;    //取消备份区写保护
	 //上面三步是必须的!*/
	 PWR_BackupAccessCmd(ENABLE);
	 RTC_WaitForLastTask();
	 RTC_SetCounter(seccount);
	 RTC_WaitForLastTask();
	 return 0;      
}

//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;
	timecount=RTC_GetCounter();
	/*timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;    */           
	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{       
		daycnt=temp;
		temp1=1970;  //从1970年开始
		while(temp>=365)
		{                          
			 if(Is_Leap_Year(temp1))//是闰年
			 {
					if(temp>=366)temp-=366;//闰年的秒钟数
					else {temp1++;break;}  
			 }
			 else temp-=365;       //平年 
			 temp1++;  
		}   
		timer.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			 if(Is_Leap_Year(timer.w_year)&&temp1==1)//当年是不是闰年/2月份
			 {
					if(temp>=29)temp-=29;//闰年的秒钟数
					else break; 
			 }
			 else 
			 {
					if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
					else break;
			 }
			 temp1++;  
		}
		timer.w_month=temp1+1;//得到月份
		timer.w_date=temp+1;  //得到日期 
	}
	temp=timecount%86400;     //得到秒钟数      
	timer.hour=temp/3600;     //小时
	timer.min=(temp%3600)/60; //分钟      
	timer.sec=(temp%3600)%60; //秒钟
	timer.week=RTC_Get_Week(timer.w_year,timer.w_month,timer.w_date);//获取星期   
	return 0;
}     

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号                                                                                                                                                     
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{     
	u16 temp2;
	u8 yearH,yearL;
	yearH=year/100;     
	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}
