#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"
#include "sys.h" 

#define USART2_RX_LEN		1024				//最大接收缓存字节数
#define USART2_TX_LEN		1024				//最大发送缓存字节数

extern u8  usart2_rx_buf[USART2_RX_LEN]; 		//接收缓冲,最大USART2_RX_LEN字节
extern u8  usart2_tx_buf[USART2_TX_LEN]; 		//发送缓冲,最大USART2_TX_LEN字节
extern u16 usart2_rx_sta;   						//接收数据状态

void Usart2_Init(u32 bound);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM6_Set(u8 sta);

#endif
