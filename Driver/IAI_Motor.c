
 /***********************************************************************************************
 目标板：ControlBoard V1.0开发板
 程序功能：IAI协议功能块  Modbus通信协议  19200 n.8.1
 工程建立日期：2013.12.9
 编写人：董斌
 修改1：
 修改2：
*************************************************************************************************/
#include "IAI_Motor.h"

const unsigned char  Ascill_16[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/*******************************************************************************
* Function Name  : ":01050427FF00"
* Description    : 设置命令模式有效指令
* Parameter		 : 站地址 1 or 2 or 3 十进制
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_OrderEnable(unsigned char AdressNum)						
{
	unsigned char Tx_String[]={':','0','0','0','5','0','4','2','7','F','F','0','0','\0'};
														//命令格式附初始值
	Tx_String[2]=AdressNum|0x30;						    //地址转换

	return IAI_Send_Command(AdressNum,Tx_String);		//返回

}															     
/*******************************************************************************
* Function Name  : :01050403FF00 
* Description    : 设置伺服ON指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_ServoON(unsigned char AdressNum)								
{
	unsigned char Tx_String[]={':','0','0','0','5','0','4','0','3','F','F','0','0','\0'};

	Tx_String[2]=AdressNum|0x30;

	return IAI_Send_Command(AdressNum,Tx_String);

}

/*******************************************************************************
* Function Name  : :010504030000 
* Description    : 设置伺服OFF指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_ServoOFF(unsigned char AdressNum)							
{
	unsigned char Tx_String[]={':','0','0','0','5','0','4','0','3','0','0','0','0','\0'};

	Tx_String[2]=AdressNum|0x30;

	return IAI_Send_Command(AdressNum,Tx_String);
}
/*******************************************************************************
* Function Name  : :001099040002040000abcd
* Description    : 设置当前速度指令
* Parameter		 : 站地址 1 or 2 or 3	 速度	1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_Speed(unsigned char AdressNum,unsigned char CSpeed ) 								
{
	unsigned char Tx_String[]={':','0','0','1','0','9','9','0','4','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};

	Tx_String[2]=AdressNum|0x30;
 	switch(CSpeed)
	{
//            case 1:
//                Tx_String[19]='0';Tx_String[20]='3';Tx_String[21]='E';Tx_String[22]='8';        //10mm/s
//                break;
//            case 10:
//                Tx_String[19]='2';Tx_String[20]='7';Tx_String[21]='1';Tx_String[22]='0';        //100
//               // Tx_String[19]='1';Tx_String[20]='F';Tx_String[21]='4';Tx_String[22]='0';            //80
////                Tx_String[19]='1';Tx_String[20]='7';Tx_String[21]='7';Tx_String[22]='0';
//                break;

		case 1:					   					//20mm/s
			Tx_String[19]='0';Tx_String[20]='7';Tx_String[21]='D';Tx_String[22]='0';	break;
                case 2:                                                                         //40mm/s
			Tx_String[19]='0';Tx_String[20]='F';Tx_String[21]='A';Tx_String[22]='0';	break;
		case 3:										//60mm/s
			Tx_String[19]='1';Tx_String[20]='7';Tx_String[21]='7';Tx_String[22]='0';	break;
		case 4:                                                                         //80mm/s
			Tx_String[19]='1';Tx_String[20]='F';Tx_String[21]='4';Tx_String[22]='0';	break;
		case 5:										//100mm/s
                        Tx_String[19]='2';Tx_String[20]='7';Tx_String[21]='1';Tx_String[22]='0';	break;
		case 6:										//120mm/s
			Tx_String[19]='2';Tx_String[20]='E';Tx_String[21]='E';Tx_String[22]='0';	break;
		case 7:										//140mm/s
			Tx_String[19]='3';Tx_String[20]='6';Tx_String[21]='B';Tx_String[22]='0';	break;
		case 8:										//160mm/s
			Tx_String[19]='3';Tx_String[20]='E';Tx_String[21]='8';Tx_String[22]='0';	break;
		case 9:					   					//180mm/s
			Tx_String[19]='4';Tx_String[20]='6';Tx_String[21]='5';Tx_String[22]='0';	break;
		case 10:                                                                        //200mm/s
			Tx_String[19]='4';Tx_String[20]='E';Tx_String[21]='2';Tx_String[22]='0';        break;                     
		default: 	return 4;
	}
	return IAI_Send_WData(AdressNum,Tx_String);
}
/*******************************************************************************
* Function Name  : :0105040AFF00
* Description    : 设置停止指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_Pause(unsigned char AdressNum)
{
        unsigned char Tx_String1[]={':','0','0','0','5','0','4','0','A','F','F','0','0','\0'};

	Tx_String1[2]=AdressNum|0x30;

        return IAI_Send_Command(AdressNum,Tx_String1);
       
}
/*******************************************************************************
* Function Name  : :0105040AFF00
* Description    : 设置清除停止指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_ClearPause(unsigned char AdressNum)
{

        unsigned char Tx_String1[]={':','0','0','0','5','0','4','0','A','0','0','0','0','\0'};
        
	Tx_String1[2]=AdressNum|0x30;

        return IAI_Send_Command(AdressNum,Tx_String1);

}
/*******************************************************************************
* Function Name  : :01050407FF00	:010504070000
* Description    : 设置清除报警指令	  双指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Set_ClearArm(unsigned char AdressNum)
{
	unsigned char Tx_String1[]={':','0','0','0','5','0','4','0','7','F','F','0','0','\0'};
	unsigned char Tx_String2[]={':','0','0','0','5','0','4','0','7','0','0','0','0','\0'};	  
	Tx_String1[2]=AdressNum|0x30;
	Tx_String2[2]=AdressNum|0x30;

	if (IAI_Send_Command(AdressNum,Tx_String1)==0x8000)
		return IAI_Send_Command(AdressNum,Tx_String2);
	else
		return 0x8005;
}
/*******************************************************************************
* Function Name  : :0105040B0000 :0105040BFF00
* Description    : 回原点指令	   双指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Go_Origin(unsigned char AdressNum)
{
	unsigned char Tx_String1[]={':','0','0','0','5','0','4','0','B','0','0','0','0','\0'};
	unsigned char Tx_String2[]={':','0','0','0','5','0','4','0','B','F','F','0','0','\0'};

	Tx_String1[2]=AdressNum|0x30;
	Tx_String2[2]=AdressNum|0x30;

	if (IAI_Send_Command(AdressNum,Tx_String1)==0x8000)
        {
		return IAI_Send_Command(AdressNum,Tx_String2);
        }
	else
		return 0x8005;
}
/*******************************************************************************
* Function Name  : :0110990000020400000000
* Description    : 回零点指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Go_Zero(unsigned char AdressNum)
{

	unsigned char Tx_String[]={':','0','0','1','0','9','9','0','0','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};

	Tx_String[2]=AdressNum|0x30;

	Tx_String[19]='0';Tx_String[20]='0';
	Tx_String[21]='0';Tx_String[22]='0';	

	return IAI_Send_WData(AdressNum,Tx_String);

}
/*******************************************************************************
* Function Name  : 
* Description    : 回指定位置指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : TRUE FALSE
********************************************************************************/
unsigned int IAI_Go_AssignPoint(unsigned char AdressNum,int Point)
{
 	unsigned char Tx_String[]={':','0','0','1','0','9','9','0','0','0','0','0','2','0','4','0','0','0','0','0','0','0','0','\0'};
	Tx_String[2]=AdressNum|0x30;
        if(Point < 0)
        {
            Point=0;
        }
	Tx_String[22]=Ascill_16[Point%16];						//取余数  查表取字符
	Tx_String[21]=Ascill_16[Point/16%16];					//取余数
	Tx_String[20]=Ascill_16[Point/256%16];					//取余数
	Tx_String[19]=Ascill_16[Point/4096%16];					//取余数
	return IAI_Send_WData(AdressNum,Tx_String);
}
/*******************************************************************************
* Function Name  : :010390050002
* Description    : 读取当前状态指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : 当前状态 8421位状态
********************************************************************************/
unsigned int Read_CurrentStatus(unsigned char AdressNum)
{
	unsigned char Tx_String[]={':','0','0','0','3','9','0','0','5','0','0','0','1','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4;
	unsigned int i;
	Tx_String[2]=AdressNum|0x30;
	i=IAI_Send_Flag(AdressNum, Tx_String);
	if(i==0x8000 )
	{
		HEX1=ChrToInt(Rx4_String[7]);
		HEX2=ChrToInt(Rx4_String[8]);
		HEX3=ChrToInt(Rx4_String[9]);
		HEX4=ChrToInt(Rx4_String[10]);
		Clear_Rx4_String();
		return HEX1*4096+HEX2*256+HEX3*16+HEX4;
	}
	return 0xffff;
}
/*******************************************************************************
* Function Name  : :010390020001
* Description    : 读取错误代码指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : 错误代码 十六进制
********************************************************************************/
unsigned int Read_ErrorCode(unsigned char AdressNum)
{
	unsigned char Tx_String[]={':','0','0','0','3','9','0','0','2','0','0','0','1','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4;
	unsigned int i;
        Tx_String[2]=AdressNum|0x30;
	i=IAI_Send_Flag(AdressNum, Tx_String);
	if(i==0x8000 )
	{
		HEX1=ChrToInt(Rx4_String[7]);
		HEX2=ChrToInt(Rx4_String[8]);
		HEX3=ChrToInt(Rx4_String[9]);
		HEX4=ChrToInt(Rx4_String[10]);
		Clear_Rx4_String();
		return HEX1*4096+HEX2*256+HEX3*16+HEX4;
	}
	return 0xffff;
}
/*******************************************************************************
* Function Name  : :010390000002
* Description    : 读取当前位置指令
* Parameter		 : 站地址 1 or 2 or 3
* Return		 : 当前位置 十进制	 返回值/100 即当前位置 200.00
********************************************************************************/
unsigned int Read_CurrentPoint(unsigned char AdressNum)
{
	unsigned char Tx4_String[]={':','0','0','0','3','9','0','0','1','0','0','0','1','\0'};
	unsigned char HEX1,HEX2,HEX3,HEX4;
        unsigned int i;
	Tx4_String[2]=AdressNum|0x30;
	i=IAI_Send_Flag(AdressNum, Tx4_String);
	if(i==0x8000)
	 {
		HEX1=ChrToInt(Rx4_String[7]);
		HEX2=ChrToInt(Rx4_String[8]);
		HEX3=ChrToInt(Rx4_String[9]);
		HEX4=ChrToInt(Rx4_String[10]);
		Clear_Rx4_String();
		return HEX1*4096+HEX2*256+HEX3*16+HEX4;
 	 }
	return 0xffff;
}
/*******************************************************************************
* Function Name  : 
* Description    : 返回写入数据后的确认命令
* Parameter		 : 
* Return		 :
********************************************************************************/
unsigned int IAI_Send_WData(unsigned char AdressNum,unsigned char *WData)
{
unsigned int tim;

	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	   {
		    Clear_Rx4_String();
                    Send_Command(WData);
		     tim=0;
		    while((Rx4_Flag!=1)&&(tim<TIME_OUT))
		      {
                            Delay_MS(10);
			    tim++;
		      }
		    if (tim>=TIME_OUT)
		      {
		         return 0x8001;                                    //超时退出
		      }
		    else
		     {
		 	   WData[13]='\0';								//数据不比较只比操作位
			   if(StringCompare(&WData[0])==0)	  			//自写字符串比较
			      {
			         return 0x8000;                               // 返回OK
			      }
			   else
			 	return 0x8002;                                   //数据比对错误
		     }
	   }
	else
	  {
		return 0x8003;                                            //地址超出
	  }
}
/*******************************************************************************
* Function Name  : 
* Description    : 返回发送后的确认命令
* Parameter		 : 
* Return		 :
********************************************************************************/
unsigned int IAI_Send_Command(unsigned char AdressNum,unsigned char *Command)
{
    unsigned int tim;

	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	    {
		   Clear_Rx4_String();
       Send_Command(Command);
		   tim=0;
		   while((Rx4_Flag!=1)&&(tim<TIME_OUT))
		     {
                            Delay_MS(10);
			    tim++;
		     }
		    if (tim>=TIME_OUT)
		      {
		         return 0x8001;                                    //超时退出
		      }
		   else
		     {
			   if(StringCompare(&Command[0])==0)	  		  	//自写字符串比较
			     {
			       return 0x8000;                                  //接受OK
			     }
			   else
			    {
			      return 0x8002;                                   //返回错误
			    }
		    }
	  }
	else
	   {
		return 0x8003;                                            //地址错误
	   }
}
/*******************************************************************************
* Function Name  : 
* Description    : 返回发送后的确认标志
* Parameter		 : 
* Return		 :
********************************************************************************/
unsigned int IAI_Send_Flag(unsigned char AdressNum,unsigned char *Command)
{
	
unsigned int tim;

	if( ( AdressNum>0 ) && ( AdressNum< MAX_ADRESS) )
	   {
                    Clear_Rx4_String();
                    Send_Command(Command);
		    tim=0;
		    while((Rx4_Flag!=1)&&(tim<TIME_OUT))
		       {
                            Delay_MS(10);
			    tim++;
		       }
		    if (tim>=TIME_OUT)
		       {
		         return 0x8001;                                    //超时退出
		       }
		   else	
		       {		   
		       	return 0x8000;                                   // 返回正确 	
           }
        }       
	else
	   {
		return 0x8003;                                            //地址超出 
	   }
}
/*******************************************************************************
* Function Name  : 
* Description    : LRC  处理
*******************************************************************************/
unsigned char LRC_DATA(unsigned char *str)				 
{
 unsigned char Lrc=0;
 unsigned char Temp=0,Temp_h=0,Temp_l=0;
 unsigned char i=0;
 unsigned char StrCmp[40];

	while( (*str) != '\0')
	{		
		StrCmp[i]= (*str);			 	//第一个字符':'不计算
		str++;	
		if(StrCmp[0] == 0x3a)		 	// 首字符必须是':'
	    {							 
			if(i%2==0)
			  {	
			    Temp_h=ChrToInt(*str);
                            Temp=0;
			   }
			else
			   {	 
			     Temp_l=ChrToInt(*str);  
			     Temp=Temp_h*16+Temp_l;
			   }	
			Lrc += Temp;
			i++;
		}
	}
	Lrc = (~Lrc)+1;				  		//LRC校验取反加一 
	return Lrc;
}
///*******************************************************************************
//* Function Name  : 
//* Description    : 十六进制字符转整形
//********************************************************************************/
//unsigned char ChrToInt(unsigned char chr)
//{
//    unsigned char n=0;

//    if (chr>='0' && chr<='9')
//      {
//	   n=chr-'0';   
//	  }   
//    else if (chr>='a' && chr<='f')
//      {
//	    n=chr-'a'+10; 
//	  }          
//    else if (chr>='A' && chr<='F')
//      {
//        n=chr-'A'+10;
//      } 
//    return n;
//}

/*******************************************************************************
* Function Name  : 
* Description    : 字符串比较 相等返回0
********************************************************************************/
unsigned char StringCompare(unsigned char *Command)
{
  unsigned char i=0,flag=0;
	
	i=0;
	while( (*Command) != '\0')
	{		
	if((*Command)!=Rx4_String[i])			//按字节比较
	 {
		 flag=1;
					break;
				 }  
		 else
				{
					Command++;	
		 i++;
		 flag=0;
				}     
	}
	Clear_Rx4_String();
	return flag;		   						//不为0相等
     }

/*******************************************************************************
* Function Name  : 
* Description    : 清除接收缓存区的全部字符及标志位
********************************************************************************/
void Clear_Rx4_String(void)
{
   unsigned char i=0;

	for(i=0;i<40;i++)	
	{
		Rx4_String[i]=0x20;            // ascill 是空格 
	}
	Rx4_Flag=0;
}

/////*******************************************************************************
////* Function Name  : 
////* Description    : 获取字符串的长度 指针以"\0"结束
////********************************************************************************/
//unsigned char Get_Strlen(unsigned char *Command)
//{
//    unsigned char i=0;

//	while( (*Command) != '\0')
//	{		
//		Command++;	
//		i++;
//	}
//	return i;	
//}


//*********************************end**********************************************/
















