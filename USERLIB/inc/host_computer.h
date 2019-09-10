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
	float longitude; //����
	float latitude;	//γ��
}lng_lat;	//��γ������

typedef struct
{
	float speed;	//����
	float course;	//����
	float acceleration;	//���ٶ�
}sail_data;	//��������

typedef struct
{
	float windForce;	//����
	float windDirect;	//����
}wind_data;	//�����������

typedef struct
{
	float pressure;	//��ѹ
	float temperature;	//�¶�
	float humidity;	//ʪ��
}env_data;	//�����������

typedef struct
{
	float water_temp;	//ˮ��
	float water_depth;	//ˮ��
	float PH;	//PHֵ
	float turbidity;	//ˮ�Ƕ�
}water_data;	//ˮ��������

extern aim_lng_lat aimData;
extern lng_lat locData;
extern sail_data	sailData;
extern wind_data	windData;
extern env_data	envData;
extern water_data	waterData;

void usart3_rxData_handel(u8 *usart_rx_data);
void DMA1_Data_Transf(int label);

#endif
