#include "HMIGT02.h"


enum status TOUCH_KEY;


//------------------------------------------------------------------------
//函数功能：获得键值	
//?01WRR0000**	
//------------------------------------------------------------------------
unsigned long Get_DATA(void)
{
	unsigned char i=0;
	unsigned long Keynum;
	
	USART3_RX_Buffer_Clear();	
	USARTStructure3.RX_Flag=0; 
	
	USART3_DMASendData("?01WDR03000001**\r",21);
	Delay_MS(10);

	while((USARTStructure3.RX_Flag != 1) && (i<50))
	{			
		i++;
		Delay_MS(1);	
		if(i>49)  return 0;					  		  	//通信超时，退出
	}

	if(USARTStructure3.RX_Len == 12)							//判断接收长度
	{	
		if((USARTStructure3.RX_BUFF[2]=='1')&&(USARTStructure3.RX_BUFF[5]=='R'))	  //站号和状态测试
		{
			
			Keynum = HexStringToInt(&USARTStructure3.RX_BUFF[6],4);	
		}		
		else
		{
			return 0;			
		}
	}
	else
	{
		return 0;
	}
	
	return Keynum;
}



//------------------------------------------------------------------------
//函数功能：获得键值	
//?01WRR0000**	
//------------------------------------------------------------------------
unsigned int SendGTdata(int STA,int ERR)
{

	char Tx_String[40]={'?','0','1','W','D','W','0','2','0','0','0','0','0','2'};

		Tx_String[14] = Ascill_16[(STA>>12)&0x0f];
		Tx_String[15] = Ascill_16[(STA>>8)&0x0f];
		Tx_String[16] = Ascill_16[(STA>>4)&0x0f];
		Tx_String[17] = Ascill_16[(STA)&0x0f];

		Tx_String[18] = Ascill_16[(ERR>>12)&0x0f];
		Tx_String[19] = Ascill_16[(ERR>>8)&0x0f];
		Tx_String[20] = Ascill_16[(ERR>>4)&0x0f];
		Tx_String[21] = Ascill_16[(ERR)&0x0f];

//		Tx_String[22] = Ascill_16[(CIO>>12)&0x0f];
//		Tx_String[23] = Ascill_16[(CIO>>8)&0x0f];
//		Tx_String[24] = Ascill_16[(CIO>>4)&0x0f];
//		Tx_String[25] = Ascill_16[(CIO)&0x0f];	
		
//		Tx_String[26] = '*';
//		Tx_String[27] = '*';

//		Tx_String[28] = 0xd;
//		Tx_String[29] = '\0';

		Tx_String[22] = '*';
		Tx_String[23] = '*';

		Tx_String[24] = 0xd;
		Tx_String[25] = '\0';
		

		USART3_RX_Buffer_Clear();	
		USARTStructure3.RX_Flag=0; 
	
		USART3_SendString(&Tx_String[0],25);
	
		Delay_MS(10);
		
		return 0;
}

//------------------------------------------------------------------------
//函数功能：获得键值	
//?01WRR0000**	
//------------------------------------------------------------------------
unsigned long Get_Key(void)
{
	unsigned char i=0;
	unsigned long Keynum;
	
	USART3_RX_Buffer_Clear();	
	USARTStructure3.RX_Flag=0; 
	
	USART3_DMASendData("?01WRR00100001**\r",21);
	Delay_MS(10);

	while((USARTStructure3.RX_Flag != 1) && (i<50))
	{			
		i++;
		Delay_MS(1);	
		if(i>49)  return 0;					  		  	//通信超时，退出
	}

	if(USARTStructure3.RX_Len == 12)							//判断接收长度
	{
		if((USARTStructure3.RX_BUFF[2]=='1')&&(USARTStructure3.RX_BUFF[5]=='R'))	  //站号和状态测试
		{
			
			Keynum = HexStringToInt(&USARTStructure3.RX_BUFF[6],4);
//			DebugPf("%ld",Keynum);
//			if (USARTStructure3.RX_BUFF[8] == '6')
//			{
//				
//			}
//			switch(USARTStructure3.RX_BUFF[8])
//			{
//				case '0':			//模式
//							if(USARTStructure3.RX_BUFF[9]=='1')	  	
//							{
//								TOUCH_KEY=AUTO;   //自动模式10
//							}
//							else if(USARTStructure3.RX_BUFF[9]=='2')
//							{
//								TOUCH_KEY=MANU;   //手动模式20
//							}
//							else if(USARTStructure3.RX_BUFF[9]=='4')
//							{
//								TOUCH_KEY=SETT;		//设置模式40
//							}
//							else if(USARTStructure3.RX_BUFF[9]=='8')
//							{
//								TOUCH_KEY=REST;	
//							}
//							else
//							{								
//								TOUCH_KEY=LDLE;
//							}
//	
//							break;
//				case '1':
//								TOUCH_KEY=M000;	
//							break;
//				case '2':	
//								TOUCH_KEY=M180;
//							break;
//				case '4':	
//								TOUCH_KEY=JUP;	
//							break;
//				case '8':	
//								TOUCH_KEY=JDW;	
//							break;
//				default:	break;
//			}	
//			
		}		
		else
		{
			return 0;			
		}
	}
	else
	{
		return 0;
	}
	
	return Keynum;
}



//*******************************************************************************
//* Function Name  :
//* Description    :
//*******************************************************************************/
unsigned long HexStringToInt(unsigned char *Command,unsigned char len)
{
    char i=0;
    unsigned long num=0;
    unsigned char byt=0;
    unsigned char tempstr;
				
			for(i = len; i>0 ;i--)
			{
					tempstr=(*Command);
					byt=ChrToInt(tempstr);
					num=num+byt*pow(16,i-1);
					Command++;
			}
			
    return num;
}



