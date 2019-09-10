#ifndef __USART3_H
#define __USART3_H

#include "stdio.h"
#include "string.h"
#include "sys.h" 
#include "dma1.h"

#define USART3_RX_LEN	6	//���ڽ����ֽڳ���
#define USART3_TX_LEN	32	//���ڷ����ֽڳ���

extern char usart3_tx_buf[USART3_TX_LEN];	//���ڷ��ʹ洢����
extern u8 usart3_rx_buf[USART3_RX_LEN];	//���ڽ��մ洢����
extern u8 action_rx_buf[USART3_RX_LEN];	//���ڽ��տ�������
extern u8 usart3_rx_flag;	//�������ݱ�־
	  	
void Usart3_Init(u32 bound);

#endif
