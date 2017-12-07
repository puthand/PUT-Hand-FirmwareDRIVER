#include "dma.h"

static void DMA_ADC();
static void DMA_USART();

static LL_DMA_InitTypeDef DMA_InitStruct;

void DMA_Init(void)
{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	DMA_ADC();
	DMA_USART();
}

void DMA_ADC()
{
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_PERIPH_TO_MEMORY;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&ADC_data;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_HALFWORD;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_CIRCULAR;
	DMA_InitStruct.NbData = 2;
	DMA_InitStruct.PeriphOrM2MSrcAddress = LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_HALFWORD;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMA_REQUEST_0;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_MEDIUM;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_1, &DMA_InitStruct);

	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);

	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
}

static void DMA_USART()
{
	DMA_InitStruct.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
	DMA_InitStruct.MemoryOrM2MDstAddress = (uint32_t)&USART_TX_buffer;
	DMA_InitStruct.MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE;
	DMA_InitStruct.MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT;
	DMA_InitStruct.Mode = LL_DMA_MODE_NORMAL;
	DMA_InitStruct.NbData = 0;
	DMA_InitStruct.PeriphOrM2MSrcAddress = LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT);
	DMA_InitStruct.PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE;
	DMA_InitStruct.PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT;
	DMA_InitStruct.PeriphRequest = LL_DMA_REQUEST_4;
	DMA_InitStruct.Priority = LL_DMA_PRIORITY_HIGH;
	LL_DMA_Init(DMA1, LL_DMA_CHANNEL_2, &DMA_InitStruct);

	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_2);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
}

void DMA1_Channel1_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		ADC_Current = ADC_data[0];
		ADC_Pos = ADC_data[1];

		LL_DMA_ClearFlag_TC1(DMA1);
	}
}

void DMA1_Channel2_3_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_HT2(DMA1))
	{
		LL_USART_EnableIT_TXE(USART2); //enable usart irq to monitor transfer completion

		LL_DMA_ClearFlag_HT2(DMA1);
	}

	if(LL_DMA_IsActiveFlag_TC2(DMA1))
	{
		LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);

		volatile int i = 0;
		i = 2;

		LL_DMA_ClearFlag_TC2(DMA1);
	}
}
