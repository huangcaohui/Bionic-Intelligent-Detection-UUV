#include "UltrasonicWave.h"  

float UltrasonicWave_Distance, distance;      //������ľ���    

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC10��TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIOC 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PA15��ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //��Ϊ����
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	//GPIOA.15	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

 	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//ʹ���ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


void EXTI15_10_IRQHandler(void)
{
	delay_us(10);	//��ʱ10us
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		TIM_SetCounter(TIM5, 0);
		TIM_Cmd(TIM5, ENABLE);	//����ʱ��
		
		while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	//�ȴ��͵�ƽ
			
		TIM_Cmd(TIM5, DISABLE);	//��ʱ��5ʧ��
			
		if(TIM_GetCounter(TIM5)*17 > 60) 
		{
			UltrasonicWave_Distance=TIM_GetCounter(TIM5)*17/10.0;	//�������&&UltrasonicWave_Distance<150
		}
			
		if(UltrasonicWave_Distance > 4) 
			distance = UltrasonicWave_Distance;
		
		EXTI_ClearITPendingBit(EXTI_Line15);  //���EXTI7��·����λ
	}
}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(void)
{
	GPIO_SetBits(TRIG_PORT,TRIG_PIN);	//��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define��
	delay_us(25);		              	//��ʱ20US
	
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	delay_us(90);
}
