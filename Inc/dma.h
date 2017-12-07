#ifndef __dma_H
#define __dma_H

#include "system_conf.h"

void DMA_Init(void);

void DMA1_Channel1_IRQHandler(void);
void DMA1_Channel2_3_IRQHandler(void);

#endif /* __dma_H */
