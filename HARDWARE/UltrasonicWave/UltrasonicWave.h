#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "sys.h"
#include "timer.h"
#include "delay.h"

#define	TRIG_PORT      GPIOC		//TRIG       
#define	ECHO_PORT      GPIOA		//ECHO 
#define	TRIG_PIN       GPIO_Pin_10   //TRIG       
#define	ECHO_PIN       GPIO_Pin_15	//ECHO 

void UltrasonicWave_Init(void);               //对超声波模块初始化
void UltrasonicWave_StartMeasure(void);                //开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间

#endif /* __UltrasonicWave_H */
