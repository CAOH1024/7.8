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
#define huart5_FRAME_HEADER_1 0xAA
#define huart5_FRAME_HEADER_2 0x55
#define huart5_FRAME_TAIL 0x0D
typedef struct {
    uint8_t label;  //ʶ���������𣬡�0����ɫ��1����ɫ��
    uint16_t x;     //�����Ͻ�Ϊԭ�㣬����Ϊ���������꡾Ĭ�����800��
    uint16_t y;     //�����꡾Ĭ�����480��
    uint16_t w;     //����Ŀ���
    uint16_t h;     //����ĸ߶�
	uint32_t count;
}huart5_TargetInfo;
extern huart5_TargetInfo huart5_targets[10];//������ṹ�嶨��ɿ��Ա��ⲿʹ�õ�
extern huart5_TargetInfo huart5_targets_num[10][10];  // 7 ����ɫ��ÿ����ɫ���洢 10 ����
extern uint8_t Serial_Rx_HEXPackage[2];
extern uint8_t Serial_Tx_HEXPackage[6];
void usart1_sendpackage(uint8_t data);
extern uint8_t color_update_flag;
extern uint16_t Serial_RxFlag;
void Serial1_ReceiveHEXPackage(void);
void usart1_send(u8 data);
void uart_init(u32 bound);
void USART1_IRQHandler(void);
void handle_packet(uint8_t *buffer, uint8_t length);
int calculate_distance(int x1, int y1, int x2, int y2);
void Data_Process(double *x, double *y);

#endif	   
















