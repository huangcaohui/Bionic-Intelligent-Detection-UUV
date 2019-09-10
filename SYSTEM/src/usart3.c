#include "usart3.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif	  

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;    

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 
 
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
void Usart3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ�����ߴ��ڹ���ģʽ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
   
	//USART3_RX	  GPIOB.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11  
	
	//���ߴ��ڵ�M1��PC14����M0��PC15��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(GPIOC,GPIO_Pin_15);	//M0
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);	//M1

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//�������ڿ����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���3
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	//�������ڵ� DMA ���չ���
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);	//�������ڵ� DMA ���͹���
}

char usart3_tx_buf[USART3_TX_LEN];	//���ڷ��ʹ洢����
u8 usart3_rx_buf[USART3_RX_LEN];	//���ڽ��մ洢����
u8 action_rx_buf[USART3_RX_LEN];	//���ڽ��տ�������
u8 usart3_rx_flag = 0;	//�������ݱ�־

void USART3_IRQHandler(void)                	//����3�жϷ������
{	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //�����ж�
	{	
		memcpy(action_rx_buf, usart3_rx_buf, USART3_RX_LEN);	//�����������鿽��
		
		if(!usart3_rx_flag)
		{
			usart3_rx_flag = 1;	//������ɣ�����ѭ�������źŽ��д���
		}
		
		Usart3_DMA1_RX_Enable(DMA1_Channel3);	//�ٿ�ʼһ��DMA����
		memset(usart3_rx_buf, 0, USART3_RX_LEN);	//��ռĴ�������
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);	//��������жϱ�־
    }
} 
