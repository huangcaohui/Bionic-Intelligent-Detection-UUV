#include "usart2.h"

void Usart2_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART2��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
	//USART2_RX	  GPIOA.3��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3  

	//USART2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���2
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//�������ڵ� DMA ���͹���
}

void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��,TIM6ʱ��ʹ��    
	
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE); //ʹ��ָ����TIM6�ж�,��������ж�
	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	usart2_rx_sta=0;		//����
	TIM6_Set(0);			//�رն�ʱ��6
}

//���ڷ��ͻ����� 	
__align(8) u8 usart2_tx_buf[USART2_TX_LEN]; 	//���ͻ���,���USART2_TX_LEN�ֽ�
//���ڽ��ջ�����
u8 usart2_rx_buf[USART2_RX_LEN]; 	//���ջ���,���USART2_RX_LEN���ֽ�.

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 usart2_rx_sta=0;

//��ʱ��6�жϷ������		    
void TIM6_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		usart2_rx_sta|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx�����жϱ�־    
		TIM6_Set(0);			//�ر�TIM6  
	}	    
}

//����TIM6�Ŀ���
//sta:0���ر�;1,����;
void TIM6_Set(u8 sta)
{
	if(sta)
	{
		TIM_SetCounter(TIM6, 0);//���������
		TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx	
	}
	else 
	{
		TIM_Cmd(TIM6, DISABLE);//�رն�ʱ��6	   
	}
}

void USART2_IRQHandler(void)                	//����3�жϷ������
{	
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART2);	
		
		if(usart2_rx_sta<USART2_RX_LEN)		//�����Խ�������
		{
			TIM_SetCounter(TIM6, 0);	//��������գ����������ݳ���10ms��������ʱ���ж�ʹ������� 
			
			if(usart2_rx_sta == 0)
			{
				TIM6_Set(1);	//ʹ�ܶ�ʱ��6���ж� 
			}
			
			usart2_rx_buf[usart2_rx_sta++]=res;		//��¼���յ���ֵ	 
		}
		else 
		{
			usart2_rx_sta|=1<<15;	//ǿ�Ʊ�ǽ������
		} 
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	//��������жϱ�־
	}
} 
