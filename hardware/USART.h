#ifndef __USART_H
#define __USART_H
/***********************************************
��˾����Ȥ�Ƽ�����ݸ�����޹�˾
Ʒ�ƣ�WHEELTEC
������wheeltec.net
�Ա����̣�shop114407458.taobao.com 
����ͨ: https://minibalance.aliexpress.com/store/4455017
�汾��V1.0
�޸�ʱ�䣺2023-01-04

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update��2023-01-04

All rights reserved
***********************************************/
#include "sys.h"
#include "stdio.h"	

extern uint8_t Serial_Rx_HEXPackage[6];
extern uint8_t Serial_Tx_HEXPackage[3];
void usart1_sendpackage(uint8_t data);
extern uint16_t Serial_RxFlag;
void usart1_send(u8 data);
void uart_init(u32 bound);
void Data_Process(double *x, double *y);

#endif	   
















