/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Descriptors for Mass Storage Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"

//设备描述符
const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC] =
  {
  	//DeviceDescriptor类别长度 0x12 个字节
    0x12,   /* bLength  */
	//DeviceDescriptor的类别是0x01-是设备描述符
    0x01,   /* bDescriptorType */
	//设备遵循的是USB2.0的协议
    0x00, 0x02,   /* bcdUSB, version 2.00 */
    // 设备所实现的类/子类/协议类别， 0x00表示这里不做描述
    0x00,   /* bDeviceClass : each interface define the device class */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */

	//包的最大长度64bytes
    0x40,   /* bMaxPacketSize0 0x40 = 64 */
	//ST
    0x83,   /* idVendor     (0483) */
    0x04,
	//
    0x20,   /* idProduct */
    0x57,

    0x00,   /* bcdDevice 2.00*/
    0x02,
	//用于描述生产厂商的字符描述的索引号
    1,              /* index of string Manufacturer  */
    /**/
    2,              /* index of string descriptor of product*/
    /* */
    3,              /* */
    /* */
    /* */
	//设备所支持的配置数目:1
    0x01    /*bNumConfigurations */
  };

//设备配置
const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {
	/*********************配置描述符提供了设备特定的配置，描述了设备的接口和端点的性质、供电模式、设备耗电********************/
	//描述符的长度是9个字节
    0x09,   /* bLength: Configuation Descriptor size */
	//描述符类型
    0x02,   /* bDescriptorType: Configuration */

    MASS_SIZ_CONFIG_DESC,
    0x00,
	//配置所支持的接口数目: 1
    0x01,   /* bNumInterfaces: 1 interface */
	//
    0x01,   /* bConfigurationValue: */
    /*      Configuration value */
    0x00,   /* iConfiguration: */
    /*      Index of string descriptor */
    /*      describing the configuration */

	//供电配置:B7(1:保留)  B6(自供电) B5(远程唤醒) B4-B0(保留)
    0xC0,   /* bmAttributes: */


    /*      bus powered */
	//产品的最大功耗:以2MA为单位计算:0x32表示50*2=100mA
    0x32,   /* MaxPower 100 mA */



	//接口
    /******************** Descriptor of Mass Storage interface ********************/
    /* 09 */
	//描述符的长度是9个字节
    0x09,   /* bLength: Interface Descriptor size */

    0x04,   /* bDescriptorType: */
    /*      Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MASS STORAGE Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent*/
    0x50,   /* nInterfaceProtocol */
    4,          /* iInterface: */



	//端点
    /* 18 */
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    0x81,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00,   /*Polling interval in milliseconds */

    /* 25 */
    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    0x02,   /*Endpoint address (OUT, address 2) */
    0x02,   /*Bulk endpoint type */
    0x40,   /*Maximum packet size (64 bytes) */
    0x00,
    0x00     /*Polling interval in milliseconds*/
    /*32*/
  };


const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
  {
    MASS_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  }
  ;      /* LangID = 0x0409: U.S. English */
const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
  {
    MASS_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    0x03,           /* bDescriptorType = String descriptor */
    /* Manufacturer: "WWW.powermcu.com" */   	
    'W', 0, 'W', 0, 'W', 0, '.', 0, 'P', 0, 'o', 0, 'w', 0, 'e', 0,
    'r', 0, 'M', 0, 'C', 0, 'U', 0, '.', 0, 'C', 0, 'O', 0, 'M', 0,
  };
const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
  {
    MASS_SIZ_STRING_PRODUCT,
    0x03,
  	/* Multi Media Development Board V1.0 */
    'M', 0, 'u', 0, 'l', 0, 't', 0, 'i', 0, ' ', 0, 'M', 0,
    'e', 0, 'd', 0, 'i', 0, 'a', 0, ' ', 0, 'D', 0, 'e', 0,	
    'v', 0, 'e', 0, 'l', 0, 'o', 0, 'p', 0, 'm', 0, 'e', 0,
	'n', 0, 't', 0, ' ', 0, 'B', 0, 'o', 0, 'a', 0, 'r', 0,
	'd', 0, ' ', 0, 'V', 0, '1', 0, '.', 0, '0'
  };

uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL] =
  {
    MASS_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number*/
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };
const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE] =
  {
    MASS_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0: "ST Mass" */
    'S', 0, 'T', 0, ' ', 0, 'M', 0, 'a', 0, 's', 0, 's', 0
  };

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
