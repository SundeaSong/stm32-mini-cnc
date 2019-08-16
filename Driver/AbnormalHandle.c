 /***********************************************************************************************
 目标板：ControlBoard V1.0开发板
 程序功能：MODUBUS 上位处理模块 错误重发机制
 协议： Modbus通信协议  19200 n.8.1
 工程建立日期：2013.12.9
 编写人：董斌
 修改1：
 修改2：
*************************************************************************************************/

#include "AbnormalHandle.h"

unsigned int Error_Code;

unsigned char ServoBusyBit[MAX_ADRESS];				//????
unsigned char ServoPauseBit[MAX_ADRESS];			//????
unsigned char ServoOriginBit[MAX_ADRESS];			//????
unsigned char ServoErrorBit[MAX_ADRESS];			//????
unsigned char ServoPwrOnBit[MAX_ADRESS];			//??ON
int ServoCurrentPoint[MAX_ADRESS];            //?????? XYZ

/*******************************************************************************
* Function Name  :
* Description    : 空闲时 读取当前位置,报警代码,PCIO状态
********************************************************************************/
void Read_Status_Handle(unsigned char adress)
{
    unsigned int RET;
    unsigned char adres,i;
    unsigned char max_adres;

    switch (adress)
    {
        case 0:
            adres=1;                        //XYZ轴
            max_adres=MAX_ADRESS;
            break;
        case 1:
            adres=1;                        //仅X轴
            max_adres=2;
            break;
        case 2:
            adres=2;                        //仅Y轴
            max_adres=3;
            break;
        case 3:
            adres=3;                        //仅Z轴
            max_adres=4;
            break;
        default:                            //地址超出
            adres=4;
            max_adres=4; 
            break;
    }
    for(i=adres;i<max_adres;i++)
    {
				RET=Read_CurrentStatus(i);
				if(RET != 0xffff)
				{
					if(RET & 0x0008) 	ServoBusyBit[i]=1;                      //伺服忙碌  BIT(3)
					else			ServoBusyBit[i]=0;

					if(RET & 0x0020) 	ServoPauseBit[i]=1;			//伺服暂停	BIT(5)
					else			ServoPauseBit[i]=0;

					if(RET & 0x0010) 	ServoOriginBit[i]=1;                    //伺服原点	BIT(4)
					else			ServoOriginBit[i]=0;

					if(RET & 0x0400) 	ServoErrorBit[i]=1;			//伺服报警	BIT(10)
					else			ServoErrorBit[i]=0;

					if(RET & 0x1000) 	ServoPwrOnBit[i]=1;			//伺服ON 	BIT(12)
					else			ServoPwrOnBit[i]=0;
				}

        Delay_MS(10);      
        RET=Read_ErrorCode(i);                                                   //报警代码
        if(RET != 0xffff)
        {
            switch (i)
            {
            case 1:
                Error_Code = RET|0x1000;                                         //各轴的报警代码
                break;
            case 2:
                Error_Code = RET|0x2000;
                break;
            case 3:
                Error_Code = RET|0x4000;
                break;
            default:break;
            }
        }

				Delay_MS(10);
        RET=Read_CurrentPoint(i);                                               //读取伺服当前位置（绝对）
        if(RET != 0xffff)
        {
             ServoCurrentPoint[i]= RET;
        }
    }

}
/*******************************************************************************
* Function Name  :
* Description    : 伺服初始化
* Parameter		 : 1 or 2 or 3
* Return		 :
********************************************************************************/
unsigned int ServoInitialize(unsigned char AdressNum)
{
    unsigned char i=1;

    ServoBusyBit[AdressNum]=0;			//伺服忙碌
    ServoPauseBit[AdressNum]=0;			//伺服暂停
    ServoOriginBit[AdressNum]=0;                //伺服原点
    ServoErrorBit[AdressNum]=0;			//伺服报警
    ServoPwrOnBit[AdressNum]=0;			//伺服ON
 
   i=0;
   while(i<REPEAT_TIMES)			  			//命令模式开启
   {
        if(IAI_Set_OrderEnable(AdressNum) == 0x8000)		  	//失败重发
        {
                break;                                                  //成功跳出
        }
        Delay_MS(10);
        i++;
   }

   i=0;
   while(i<REPEAT_TIMES)			  			 //清除报警
   {
   		if(IAI_Set_ClearArm(AdressNum) == 0x8000)		 //失败重发
		{
			break;                                           //成功跳出
		}
		Delay_MS(10);
		i++;
   }
   IAI_Set_ClearPause(AdressNum);                                       //清除暂停命令

   i=0;
   while(i<REPEAT_TIMES)			  			 //伺服ON
   {
   		if(IAI_Set_ServoON(AdressNum) == 0x8000)		 //失败重发
		{
			break;						 //成功跳出
		}
		Delay_MS(10);
		i++;
   }

   Read_Status_Handle(AdressNum);

   return 0x0000;
}

/*******************************************************************************
* Function Name  :
* Description    : 伺服速度设置
* Parameter		 : 1 or 2 or 3
* Return		 :
********************************************************************************/
unsigned int ServoSetSpeed(unsigned char AdressNum,unsigned int CSpeed)
{
unsigned char i=0;

   while( i<REPEAT_TIMES )						//运行速度设置
   {
   		if(IAI_Set_Speed(AdressNum,CSpeed) == 0x8000)		//失败重发
		{
			break;                                          //成功跳出
		}
		Delay_MS(10);
		i++;
   }
   if( i >= REPEAT_TIMES )	   return 0xA010+AdressNum*3;
   return 0;
}

/*******************************************************************************
* Function Name  :
* Description    : 伺服回原点
* Parameter		 : 1 or 2 or 3
* Return		 :
********************************************************************************/
unsigned int ServoGoOrigin(unsigned char AdressNum)
{
unsigned char i=0;

   while( i<REPEAT_TIMES )						//回原点设置
   {
   		if(IAI_Go_Origin(AdressNum) == 0x8000)		  		//失败重发
		{
			break;									    //成功跳出
		}
		Delay_MS(10);
		i++;
   }
   if( i >= REPEAT_TIMES )	   return 1;
   return 0;
}

/*******************************************************************************
* Function Name  :
* Description    : 伺服移动指定位置
* Parameter		 :  地址 位置
* Return		 :
********************************************************************************/
unsigned int ServoGoAssignPoint(unsigned char AdressNum,unsigned int Point)
{
unsigned char i=0;
   while( i<REPEAT_TIMES )		  				//去指定位置
   {
   		if(IAI_Go_AssignPoint(AdressNum, Point) == 0x8000)	//失败重发
		{
			break;								    	//成功跳出
		}
		Delay_MS(10);
		i++;
   }
   if( i >= REPEAT_TIMES )	   return 1;
   return 0;
}


