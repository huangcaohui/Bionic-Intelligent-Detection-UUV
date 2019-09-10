#ifndef __USART4_H
#define __USART4_H

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart3.h"

#define UART4_RX_LEN		1024				//�����ջ����ֽ���
#define UART4_TX_LEN		1024				//����ͻ����ֽ���

extern u8  uart4_rx_buf[UART4_RX_LEN]; 		//���ջ���,���UART4_RX_LEN�ֽ�
extern u8  uart4_tx_buf[UART4_TX_LEN]; 		//���ͻ���,���UART4_TX_LEN�ֽ�
extern u8 uart4_rx_sta;

void Uart4_Init(u32 bound);
	
#endif
