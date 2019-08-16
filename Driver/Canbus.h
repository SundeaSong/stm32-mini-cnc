
#ifndef _CANBUS_H_
#define _CANBUS_H_

#ifdef __cplusplus		   //定义对CPP进行C处理 //开始部分
extern "C" {
#endif
 
#include "stm32f10x.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

//-----CAN端口分配，移植时修改PB8 PB9----------
  #define RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOB
  #define GPIO_Remap_CAN             GPIO_Remap1_CAN1
  #define GPIO_CAN                   GPIOB
  #define GPIO_Pin_CAN_RX            GPIO_Pin_8
  #define GPIO_Pin_CAN_TX            GPIO_Pin_9


#ifdef __cplusplus
extern "C"
{
#endif

//
//extern volatile unsigned int g_faceDatLen;
//
extern  __IO uint32_t ret; /* for return of the interrupt handling */
extern volatile u8 g_CanRecvFlag;
extern CanRxMsg RxMessage;


// 函数声明

void CAN_BUS_init(void);
unsigned char CanbusSendData(u8 reader_addr,u8 *pdata,u8 len);

#ifdef __cplusplus		   //定义对CPP进行C处理 //结束部分
}
#endif

#endif
