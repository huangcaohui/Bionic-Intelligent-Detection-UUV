#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"
#include "sys.h" 

#define USART2_RX_LEN		1024				//�����ջ����ֽ���
#define USART2_TX_LEN		1024				//����ͻ����ֽ���

extern u8  usart2_rx_buf[USART2_RX_LEN]; 		//���ջ���,���USART2_RX_LEN�ֽ�
extern u8  usart2_tx_buf[USART2_TX_LEN]; 		//���ͻ���,���USART2_TX_LEN�ֽ�
extern u16 usart2_rx_sta;   						//��������״̬

void Usart2_Init(u32 bound);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM6_Set(u8 sta);

#endif
