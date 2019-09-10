#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"
#include "dma1.h"
#include "adc.h"
#include "led.h"
#include "usart3.h"
#include "ds18b20.h"
#include "UltrasonicWave.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

#endif

