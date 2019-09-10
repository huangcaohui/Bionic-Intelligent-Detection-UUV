#include "uart4.h"

void Uart4_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��UART4��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
	//UART4_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC.10
   
	//UART4_RX	  GPIOC.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC.11  

	//UART4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������4
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���4
}

u8 uart4_rx_sta = 0;	
u8 uart4_rx_buf[UART4_RX_LEN]; 		//���ջ���,���UART4_RX_LEN�ֽ�
u8 uart4_tx_buf[UART4_TX_LEN]; 		//���ͻ���,���UART4_TX_LEN�ֽ�

void UART4_IRQHandler(void)
{
	u8 data;
	static u8 UsartNum = 0;	//����8λ��̬����
	
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)	//�����ж�
    {    
		if(uart4_rx_sta == 0)
		{
			data = USART_ReceiveData(UART4);	
			if(uart4_rx_buf[0] == 0xf7)
			{
				uart4_rx_buf[UsartNum] = data;
				UsartNum++;
				if(UsartNum >= 8)
				{
					UsartNum = 0;
					uart4_rx_sta = 1;
				}
			}
			else if(data == 0xf7)
			{
				uart4_rx_buf[0] = data;
				UsartNum = 1;
			}
		}	
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	//��������жϱ�־          		     
    }
}
