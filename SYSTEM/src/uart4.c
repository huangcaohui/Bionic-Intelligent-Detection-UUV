#include "uart4.h"

void Uart4_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能UART4，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
  
	//UART4_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PC.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.10
   
	//UART4_RX	  GPIOC.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC.11  

	//UART4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口4
	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接收中断
	USART_Cmd(UART4, ENABLE);                    //使能串口4
}

u8 uart4_rx_sta = 0;	
u8 uart4_rx_buf[UART4_RX_LEN]; 		//接收缓冲,最大UART4_RX_LEN字节
u8 uart4_tx_buf[UART4_TX_LEN]; 		//发送缓冲,最大UART4_TX_LEN字节

void UART4_IRQHandler(void)
{
	u8 data;
	static u8 UsartNum = 0;	//定义8位静态变量
	
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)	//接收中断
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
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	//清除接收中断标志          		     
    }
}
