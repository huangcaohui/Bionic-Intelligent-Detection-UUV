#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "sys.h"
#include "timer.h"
#include "delay.h"

#define	TRIG_PORT      GPIOC		//TRIG       
#define	ECHO_PORT      GPIOA		//ECHO 
#define	TRIG_PIN       GPIO_Pin_10   //TRIG       
#define	ECHO_PIN       GPIO_Pin_15	//ECHO 

void UltrasonicWave_Init(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave_StartMeasure(void);                //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��

#endif /* __UltrasonicWave_H */
