#ifndef	__HTTPUTIL_H__
#define	__HTTPUTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"
#include "w5500.h"
#include "config.h"

#include "httpd.h"

#include "socket.h"

#define HTML "<html><head><title>Hello!Myzy!</title>\
							<style type='text/css'>h1 {color: tan}p {color: blue}</style></head>\
							<body bgcolor='Gainsboro'><h1 align='center'>My First Heading</h1>\
							<h2>My First Heading</h2><p align='center'>My first paragraph.</p>\
							<hr /><p>This is a paragraph.</p><table border='1'><tr><td>100</td><td>200</td><td>300</td></tr><tr><td>400</td><td>500</td><td>600</td></tr></table>\
							<br /><input type='button' value='Hello world!'>\
							<br /><tr><td width='80px'><b> LED?:</b></td>\
							<td width='80px'>LED1<input type='checkbox' name='cb_led' value='led1' /></td>\
							<td width='80px'>LED2<input type='checkbox' name='cb_led' value='led2' /></td>\
							<td width='80px'>LED3<input type='checkbox' name='cb_led' value='led3' /></td>\
							<td width='80px'>LED4<input type='checkbox' name='cb_led' value='led4' /></td>\
							<td></td></tr>\
							<br /><p>This is another paragraph.</p></body></html>"

void proc_http(SOCKET s, u_char * buf);
void do_http(void);
void cgi_ipconfig(st_http_request *http_request);

//void trimp(uint8* src, uint8* dst, uint16 len);
uint16 make_msg_response(uint8* buf,int8* msg);

void make_cgi_response(uint16 a,int8* b,int8* c);
void make_pwd_response(int8 isRight,uint16 delay,int8* cgi_response_content, int8 isTimeout);
#endif
