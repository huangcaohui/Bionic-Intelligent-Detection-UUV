#include "UltrasonicWave.h"  

float UltrasonicWave_Distance, distance;      //计算出的距离    

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC10接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //初始化外设GPIOC 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PA15接ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //设为输入
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //初始化GPIOA
	
	//GPIOA.15	  中断线以及中断初始化配置
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);

 	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


void EXTI15_10_IRQHandler(void)
{
	delay_us(10);	//延时10us
	if(EXTI_GetITStatus(EXTI_Line15) != RESET)
	{
		TIM_SetCounter(TIM5, 0);
		TIM_Cmd(TIM5, ENABLE);	//开启时钟
		
		while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	//等待低电平
			
		TIM_Cmd(TIM5, DISABLE);	//定时器5失能
			
		if(TIM_GetCounter(TIM5)*17 > 60) 
		{
			UltrasonicWave_Distance=TIM_GetCounter(TIM5)*17/10.0;	//计算距离&&UltrasonicWave_Distance<150
		}
			
		if(UltrasonicWave_Distance > 4) 
			distance = UltrasonicWave_Distance;
		
		EXTI_ClearITPendingBit(EXTI_Line15);  //清除EXTI7线路挂起位
	}
}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(void)
{
	GPIO_SetBits(TRIG_PORT,TRIG_PIN);	//送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中
	delay_us(25);		              	//延时20US
	
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	delay_us(90);
}
