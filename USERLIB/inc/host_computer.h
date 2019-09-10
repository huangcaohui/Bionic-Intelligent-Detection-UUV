#ifndef __HOST_COMPUTER_H
#define __HOST_COMPUTER_H

#include "sys.h"
#include "usart3.h"
#include "motor.h"

typedef struct
{
	float aim_lng;
	float aim_lat;
	
}aim_lng_lat;

typedef struct
{
	float longitude; //经度
	float latitude;	//纬度
}lng_lat;	//经纬度数据

typedef struct
{
	float speed;	//地速
	float course;	//航向
	float acceleration;	//加速度
}sail_data;	//航行数据

typedef struct
{
	float windForce;	//风力
	float windDirect;	//风向
}wind_data;	//环境监测数据

typedef struct
{
	float pressure;	//气压
	float temperature;	//温度
	float humidity;	//湿度
}env_data;	//环境监测数据

typedef struct
{
	float water_temp;	//水温
	float water_depth;	//水深
	float PH;	//PH值
	float turbidity;	//水浊度
}water_data;	//水体检测数据

extern aim_lng_lat aimData;
extern lng_lat locData;
extern sail_data	sailData;
extern wind_data	windData;
extern env_data	envData;
extern water_data	waterData;

void usart3_rxData_handel(u8 *usart_rx_data);
void DMA1_Data_Transf(int label);

#endif
