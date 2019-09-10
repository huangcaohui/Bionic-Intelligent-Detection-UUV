#include "usart3.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif	  

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;    

//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 
 
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
void Usart3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART3，GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能无线串口工作模式，GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
	//USART3_TX   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
	//USART3_RX	  GPIOB.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  
	
	//无线串口的M1（PC14）、M0（PC15）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_SetBits(GPIOC,GPIO_Pin_15);	//M0
	GPIO_ResetBits(GPIOC,GPIO_Pin_14);	//M1

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//开启串口空闲中断
	USART_Cmd(USART3, ENABLE);                    //使能串口3
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	//开启串口的 DMA 接收功能
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);	//开启串口的 DMA 发送功能
}

char usart3_tx_buf[USART3_TX_LEN];	//串口发送存储数组
u8 usart3_rx_buf[USART3_RX_LEN];	//串口接收存储数组
u8 action_rx_buf[USART3_RX_LEN];	//串口接收拷贝数组
u8 usart3_rx_flag = 0;	//接收数据标志

void USART3_IRQHandler(void)                	//串口3中断服务程序
{	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //空闲中断
	{	
		memcpy(action_rx_buf, usart3_rx_buf, USART3_RX_LEN);	//将缓存区数组拷贝
		
		if(!usart3_rx_flag)
		{
			usart3_rx_flag = 1;	//接收完成，给主循环发送信号进行处理
		}
		
		Usart3_DMA1_RX_Enable(DMA1_Channel3);	//再开始一次DMA传输
		memset(usart3_rx_buf, 0, USART3_RX_LEN);	//清空寄存器数组
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);	//清除空闲中断标志
    }
} 
