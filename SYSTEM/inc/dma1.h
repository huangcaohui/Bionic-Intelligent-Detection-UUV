#ifndef __DMA1_H
#define __DMA1_H

#include "sys.h"
#include "usart3.h"

void Usart3_RX_DMA1_Init(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);
void Usart3_TX_DMA1_Init(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);
void Usart2_TX_DMA1_Init(DMA_Channel_TypeDef*DMA_CHx, u32 cpar, u32 cmar);
void Usart3_DMA1_RX_Enable(DMA_Channel_TypeDef*DMA_CHx);
void Usart3_DMA1_TX_Enable(DMA_Channel_TypeDef*DMA_CHx);

#endif
