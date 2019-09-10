#include "host_computer.h"

aim_lng_lat aimData;
lng_lat locData;
sail_data	sailData;
wind_data	windData;
env_data	envData;
water_data	waterData;

//对串口接收到的上位机数据进行处理
void usart3_rxData_handel(u8 *usart_rx_data)
{
//	int i;
	
	if(usart_rx_data[0] != 0x3f || usart_rx_data[5] != 0x4f)	//头尾校验错误
	{
		USART_ClearITPendingBit(USART3, USART_IT_IDLE);	//清除空闲中断标志
		DMA_ClearFlag(DMA1_FLAG_GL3);	//清除通道3传输完成标志
		DMA_ClearITPendingBit(DMA1_FLAG_GL3);	//清除通道3传输完成标志
		memset(usart3_rx_buf, 0, USART3_RX_LEN);	//清空dma寄存器数组
		usart3_rx_flag = 0;	//串口中断外部循环标志清零
		Usart3_DMA1_RX_Enable(DMA1_Channel3);	//重新开始一次DMA传输
		return;
	}
	
	if(usart_rx_data[1] != 1)
	{
		memset(usart_rx_data, 0, USART3_RX_LEN);	//清空缓存区数组
		return;
	}
	
//	for(i = 0; i < USART3_RX_LEN; i++)
//	{
//		printf("%x\t", usart_rx_data[i]);
//	}
//	printf("\r\n");
	
	switch(usart_rx_data[2])
	{
		case 1: go_up(); break;	//上浮
		case 2: go_dive(); break;	//下潜
		case 3: hover(); break;	//悬停
		case 4: roll(); break;	//翻滚
		case 5: auto_voyage(); break;	//自主航行
		case 6: artifical(usart_rx_data[3], usart_rx_data[4]); break;	//人工
		case 7: aimData.aim_lng = usart_rx_data[3]; aimData.aim_lat = usart_rx_data[4]; break;
		case 8: aimData.aim_lng = aimData.aim_lng + usart_rx_data[3]/1e2 + usart_rx_data[4]/1e4; break;
		case 9: aimData.aim_lat = aimData.aim_lat + usart_rx_data[3]/1e2 + usart_rx_data[4]/1e4; break;
		case 10:aimData.aim_lng = aimData.aim_lng + usart_rx_data[3]/1e6;
				aimData.aim_lat = aimData.aim_lat + usart_rx_data[4]/1e6; break;
	}
	
//	printf("Signal:%d\r\nlng:%f\r\nlat:%f\r\n", usart_rx_data[2], aimData.aim_lng, aimData.aim_lat);
	
	TIM_SetCompare1(TIM8, left_speed);
	TIM_SetCompare2(TIM8, right_speed);
	TIM_SetCompare3(TIM8, back_speed);
	TIM_SetCompare4(TIM8, steer_angle);
	
	memset(usart_rx_data, 0, USART3_RX_LEN);	//清空缓存区数组
}

void DMA1_Data_Transf(int label)
{
	memset(usart3_tx_buf, '\0', sizeof(usart3_tx_buf));
	switch(label)
	{
		case 1: 
			snprintf(usart3_tx_buf, USART3_TX_LEN, "%s %.6f %.6f %s", "AA", locData.longitude, locData.latitude, "BB"); break;
		case 2: 
			snprintf(usart3_tx_buf, USART3_TX_LEN, "%s %.2f %.2f %.2f %s", "CC", sailData.speed, sailData.course, sailData.acceleration, "DD"); break;
		case 3: 
			snprintf(usart3_tx_buf, USART3_TX_LEN, "%s %.2f %.2f %s", "EE", windData.windForce, windData.windDirect, "FF"); break;
		case 4:
			snprintf(usart3_tx_buf, USART3_TX_LEN, "%s %.2f %.2f %.2f %s", "GG", envData.pressure, envData.temperature, envData.humidity, "HH"); break;
		case 5: 
			snprintf(usart3_tx_buf, USART3_TX_LEN, "%s %.2f %.2f %.2f %.2f %s", "II", waterData.water_temp, waterData.water_depth, waterData.PH, waterData.turbidity, "JJ"); break;
	}
}
