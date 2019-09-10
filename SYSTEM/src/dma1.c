#include "dma1.h"

u16 Usart3_RX_LEN;//����DMAÿ�����ݽ��յĳ���
u16 Usart3_TX_LEN;//����DMAÿ�����ݷ��͵ĳ���

//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void Usart3_RX_DMA1_Init(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
	Usart3_RX_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬽��������ݶ�ȡ���ڴ���
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	  	
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA_CHx, ENABLE);
}

void Usart3_TX_DMA1_Init(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
    DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	Usart3_TX_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	  	
	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA_CHx, ENABLE);
}

void Usart2_TX_DMA1_Init(DMA_Channel_TypeDef*DMA_CHx, u32 cpar, u32 cmar)
{
	DMA_InitTypeDef DMA_InitStructure;
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	
} 

//���¿�ʼһ��DMA����
void Usart3_DMA1_RX_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 	
	DMA_Cmd(DMA_CHx, DISABLE);  //�ر�USART3 RX DMA1 ��ָʾ��ͨ�� 
	
	//���IDLE
	USART3->SR;	
	USART3->DR;
	
 	DMA_SetCurrDataCounter(DMA_CHx, Usart3_RX_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART3 RX DMA1 ��ָʾ��ͨ�� 
}

//���¿�ʼһ��DMA����
void Usart3_DMA1_TX_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 	
	DMA_Cmd(DMA_CHx, DISABLE);  //�ر�USART3 TX DMA1 ��ָʾ��ͨ�� 
	
	//���IDLE
	USART3->SR;	
	USART3->DR;
	
 	DMA_SetCurrDataCounter(DMA_CHx, Usart3_TX_LEN);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��USART3 TX DMA1 ��ָʾ��ͨ�� 
}

//�����ж�
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3))	//�ȴ�ͨ��3�������
	{	
		DMA_ClearFlag(DMA1_FLAG_TC3);	//���ͨ��3������ɱ�־
		DMA_ClearITPendingBit(DMA1_IT_TC3);	//���ͨ��3������ɱ�־
	}
}

//�����ж�
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2))	//�ȴ�ͨ��2�������
	{	
		DMA_ClearFlag(DMA1_FLAG_TC2);//���ͨ��2������ɱ�־
		DMA_ClearITPendingBit(DMA1_IT_TC2);	//���ͨ��2������ɱ�־
	}
}
