#include "usart2.h"

void Usart2_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
  
	//USART2_TX   GPIOA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
	//USART2_RX	  GPIOA.3初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  

	//USART2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接收中断
	USART_Cmd(USART2, ENABLE);                    //使能串口2
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);	//开启串口的 DMA 发送功能
}

void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能,TIM6时钟使能    
	
	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE); //使能指定的TIM6中断,允许更新中断
	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	usart2_rx_sta=0;		//清零
	TIM6_Set(0);			//关闭定时器6
}

//串口发送缓存区 	
__align(8) u8 usart2_tx_buf[USART2_TX_LEN]; 	//发送缓冲,最大USART2_TX_LEN字节
//串口接收缓存区
u8 usart2_rx_buf[USART2_RX_LEN]; 	//接收缓冲,最大USART2_RX_LEN个字节.

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 usart2_rx_sta=0;

//定时器6中断服务程序		    
void TIM6_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		usart2_rx_sta|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志    
		TIM6_Set(0);			//关闭TIM6  
	}	    
}

//设置TIM6的开关
//sta:0，关闭;1,开启;
void TIM6_Set(u8 sta)
{
	if(sta)
	{
		TIM_SetCounter(TIM6, 0);//计数器清空
		TIM_Cmd(TIM6, ENABLE);  //使能TIMx	
	}
	else 
	{
		TIM_Cmd(TIM6, DISABLE);//关闭定时器6	   
	}
}

void USART2_IRQHandler(void)                	//串口3中断服务程序
{	
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		res =USART_ReceiveData(USART2);	
		
		if(usart2_rx_sta<USART2_RX_LEN)		//还可以接收数据
		{
			TIM_SetCounter(TIM6, 0);	//计数器清空，若两次数据超过10ms则会产生定时器中断使接收完成 
			
			if(usart2_rx_sta == 0)
			{
				TIM6_Set(1);	//使能定时器6的中断 
			}
			
			usart2_rx_buf[usart2_rx_sta++]=res;		//记录接收到的值	 
		}
		else 
		{
			usart2_rx_sta|=1<<15;	//强制标记接收完成
		} 
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	//清除接收中断标志
	}
} 
