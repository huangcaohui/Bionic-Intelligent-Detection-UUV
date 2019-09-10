#ifndef __USART4_H
#define __USART4_H

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart3.h"

#define UART4_RX_LEN		1024				//最大接收缓存字节数
#define UART4_TX_LEN		1024				//最大发送缓存字节数

extern u8  uart4_rx_buf[UART4_RX_LEN]; 		//接收缓冲,最大UART4_RX_LEN字节
extern u8  uart4_tx_buf[UART4_TX_LEN]; 		//发送缓冲,最大UART4_TX_LEN字节
extern u8 uart4_rx_sta;

void Uart4_Init(u32 bound);
	
#endif
