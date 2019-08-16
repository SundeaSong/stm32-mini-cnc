#include "I2CEEPROM.h"

enum ENUM_TWI_REPLY
{
	TWI_NACK=0
	,TWI_ACK=1
};

enum ENUM_TWI_BUS_STATE
{
	TWI_READY=0
	,TWI_BUS_BUSY=1
	,TWI_BUS_ERROR=2
};


void I2C_EE_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  
  // Configure I2C1 pins: SCL and SDA 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void TWI_delay(void)
{ 
   uint8_t i=10; 
   while(i--); 
   
}
/**************************************************************************
CYCLECOUNTER / 72000000 
***************************************************************************/
void DelayMs(uint16_t ms)
{
	uint16_t iq0;
	uint16_t iq1;
	for(iq0 = ms; iq0 > 0; iq0--)
	{
		for(iq1 = 11998; iq1 > 0; iq1--); // ( (6*iq1+9)*iq0+15 ) / 72000000 
  
	}
}


uint8_t TWI_Start(void)
{
	SDAH;
	SCLH;
	TWI_delay();
	if(!SDAread)return TWI_BUS_BUSY; 
	SDAL;
	TWI_delay();
	if(SDAread) return TWI_BUS_ERROR;
	SCLL;
	TWI_delay();
	return TWI_READY;
}

/*void TWI_Stop(void)
{
 SCLL;
 TWI_delay();
 SDAL;
 TWI_delay();
 SCLH;
 TWI_delay();
 SDAH;
 TWI_delay();
}*/
void TWI_Stop(void)
{
	SDAL;
	SCLL;
	TWI_delay();
	SCLH;
	TWI_delay();
	SDAH;
	TWI_delay();
}

void TWI_Ack(void)
{ 
	SCLL;
	TWI_delay();
	SDAL;
	TWI_delay();
	SCLH;
	TWI_delay();
	SCLL;
	TWI_delay();
}

void TWI_NoAck(void)
{ 
	SCLL;
	TWI_delay();
	SDAH;
	TWI_delay();
	SCLH;
	TWI_delay();
	SCLL;
	TWI_delay();
}

uint8_t TWI_WaitAck(void)  
{
	SCLL;
	TWI_delay();
	SDAH;   
	TWI_delay();
	SCLH;
	TWI_delay();
	if(SDAread)
	{
		SCLL;
		return 0;
	}
	SCLL;
	return 1;
}

void TWI_SendByte(uint8_t SendByte)
{
	uint8_t i=8;
	while(i--)
	{
		SCLL;
		TWI_delay();
		if(SendByte&0x80)
			SDAH;  
		else 
		SDAL;   
		SendByte<<=1;
		TWI_delay();
		SCLH;
		TWI_delay();
	}
	SCLL;
}


uint8_t TWI_ReceiveByte(void)  
{ 
	uint8_t i=8;
	uint8_t ReceiveByte=0;

	SDAH;    
	while(i--)
	{
		ReceiveByte <<= 1;      
		SCLL;
		TWI_delay();
		SCLH;
		TWI_delay(); 
		if(SDAread)
		{
			ReceiveByte |= 0x01;
		}
	}
	SCLL;
	return ReceiveByte;
}


uint8_t TWI_WriteByte(uint8_t SendByte, uint16_t WriteAddress)
{  
	uint8_t i;

	W_ENANLE;
	DelayMs(1);

	i = TWI_Start();
	if(i)
	return i;
	 
	TWI_SendByte( ADDR_24CXX & 0xFE);

	if(!TWI_WaitAck())
	{
	 TWI_Stop(); 
	 return 0;
	}

	TWI_SendByte(WriteAddress>>8);   //先高位   
	TWI_WaitAck(); 
	TWI_SendByte(WriteAddress);      //再低位
	TWI_WaitAck(); 
	TWI_SendByte(SendByte);       
	TWI_WaitAck();   
	TWI_Stop(); 

	DelayMs(12); 
	W_DISANLE;

	return 3;
}

 

uint8_t TWI_ReadByte( uint16_t ReadAddress)
{  
	uint8_t i,temp;
	i = TWI_Start();
	if(i)
	return i;
	 
	TWI_SendByte((ADDR_24CXX & 0xFE));
	if(!TWI_WaitAck())
	{
		TWI_Stop(); 
		return 0;
	}

	TWI_SendByte(ReadAddress>>8);        
	TWI_WaitAck();
	TWI_SendByte(ReadAddress);        
	TWI_WaitAck();
	TWI_Start();
	TWI_SendByte((ADDR_24CXX & 0xFE)|0x01);    
	TWI_WaitAck();

	//*pDat = TWI_ReceiveByte();
	temp = TWI_ReceiveByte();

	TWI_NoAck();

	TWI_Stop();
	return temp;
}

///***************************************************************************

//***************************************************************************/
//uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data,uint8_t adr,uint8_t nbyte)
//{
//   uint8_t i;
//  
//	W_ENANLE;
//	Delay_ms(1);
// for(;nbyte!=0;nbyte--)
// {
//  i = TWI_Start();
//        if(i)
//     return i;
//         
//        TWI_SendByte( ADDR_24CXX & 0xFE);
//    
//     if(!TWI_WaitAck())
//     {
//       TWI_Stop(); 
//       return 0;
//     }
// 
//        TWI_SendByte(adr);      
//        TWI_WaitAck(); 
//        TWI_SendByte(*psrc_data);          
//        TWI_WaitAck();   
//        psrc_data++;    
//  adr++;   
//        TWI_Stop(); 

//        DelayMs(12); 
//        
// }
// W_DISANLE;
// return 0;
//}


/***************************************************************************

***************************************************************************/
uint8_t I2C_EE_BufferWrite(uint8_t *psrc_data,uint16_t adr,uint8_t nbyte)
{
	uint8_t i;

	W_ENANLE;
	DelayMs(1);

	i = TWI_Start();
	if(i)
		return i;
	 
	TWI_SendByte( ADDR_24CXX & 0xFE);

	if(!TWI_WaitAck())
	{
		TWI_Stop(); 
		return 0;
	}

	TWI_SendByte(adr>>8);      
	TWI_WaitAck(); 
	TWI_SendByte(adr);      
	TWI_WaitAck(); 
	for(;nbyte!=0;nbyte--)
	{
		TWI_SendByte(*psrc_data);          
		TWI_WaitAck();   
		psrc_data++;    
	} 
	TWI_Stop(); 

	DelayMs(12); 

	W_DISANLE;
	return 0;
}

/***************************************************************************

***************************************************************************/
uint8_t I2C_EE_BufferRead(uint8_t *pdin_data,uint16_t adr,uint8_t nbyte)
{
	uint8_t i;
	i = TWI_Start();
	if(i)
	return i;
	 
	TWI_SendByte((ADDR_24CXX & 0xFE));
	if(!TWI_WaitAck())
	{
		TWI_Stop(); 
		return 0;
	}

	TWI_SendByte(adr>>8);       
	TWI_WaitAck();
	TWI_SendByte(adr);       
	TWI_WaitAck();
	TWI_Start();
	TWI_SendByte((ADDR_24CXX & 0xFE)|0x01);   
	TWI_WaitAck();

	while(nbyte!=1)               
	{
		*pdin_data = TWI_ReceiveByte(); 
		TWI_Ack();  
		pdin_data++;  
		nbyte--;  
	};

	*pdin_data = TWI_ReceiveByte();  
	TWI_NoAck();    
	 
	TWI_Stop();   

	return 0;
}

void SaveEpData(unsigned int ads,unsigned post)
{
	unsigned char temp[10];
	
	temp[0] = ads>>8;
	temp[1] = ads;
	temp[2] = post>>8;
	temp[3] = post;

	I2C_EE_BufferWrite(&temp[0],ads,4);
}
unsigned int ReadEpData(unsigned int ads)
{
	unsigned char temp[10];
	unsigned char eads;
	unsigned int post = 0;

	I2C_EE_BufferRead(&temp[0],ads,4);
	
	eads = (temp[0]<<8) | temp[1];

	if( eads ==ads )
	{
		 post = (temp[2]<<8) | temp[3];
	}
	
	return post;
}



/*
void TWI_24CXX_Write(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
{
  uint16_t i;
  for(i=0;i<nLen;i++)
  {
    TWI_WriteByte(*(pDat+i), nAddr+i);
    
   
  }
}

void TWI_24CXX_Read(uint8_t* pDat, uint8_t nAddr, uint8_t nLen)
{
    uint16_t i;
    for(i=0; i<nLen; i++)
      *(pDat+i) = TWI_ReadByte(nAddr+i);
}
*/


