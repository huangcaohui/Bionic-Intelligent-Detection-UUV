#ifndef __USART3_H
#define __USART3_H

#include "stdio.h"
#include "string.h"
#include "sys.h" 
#include "dma1.h"

#define USART3_RX_LEN	6	//串口接收字节长度
#define USART3_TX_LEN	32	//串口发送字节长度

extern char usart3_tx_buf[USART3_TX_LEN];	//串口发送存储数组
extern u8 usart3_rx_buf[USART3_RX_LEN];	//串口接收存储数组
extern u8 action_rx_buf[USART3_RX_LEN];	//串口接收拷贝数组
extern u8 usart3_rx_flag;	//接收数据标志
	  	
void Usart3_Init(u32 bound);

#endif
