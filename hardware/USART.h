#ifndef __USART_H
#define __USART_H
/***********************************************
公司：轮趣科技（东莞）有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：V1.0
修改时间：2023-01-04

Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version: V1.0
Update：2023-01-04

All rights reserved
***********************************************/
#include "sys.h"
#include "stdio.h"	
#define huart5_FRAME_HEADER_1 0xAA
#define huart5_FRAME_HEADER_2 0x55
#define huart5_FRAME_TAIL 0x0D
typedef struct {
    uint8_t label;  //识别物体的类别，【0：蓝色，1：红色】
    uint16_t x;     //以左上角为原点，向右为正，横坐标【默认最大800】
    uint16_t y;     //纵坐标【默认最大480】
    uint16_t w;     //物体的宽度
    uint16_t h;     //物体的高度
	uint32_t count;
}huart5_TargetInfo;
extern huart5_TargetInfo huart5_targets[10];//将这个结构体定义成可以被外部使用的
extern huart5_TargetInfo huart5_targets_num[10][10];  // 7 种颜色，每种颜色最多存储 10 个球
extern uint8_t Serial_Rx_HEXPackage[2];
extern uint8_t Serial_Tx_HEXPackage[6];
extern uint8_t color_update_flag;
extern uint16_t Serial_RxFlag;
void Serial1_ReceiveHEXPackage(void);
void usart1_send(u8 data);
void uart_init(u32 bound);
void USART1_IRQHandler(void);
void handle_packet(uint8_t *buffer, uint8_t length);
int calculate_distance(int x1, int y1, int x2, int y2);
#endif	   
















